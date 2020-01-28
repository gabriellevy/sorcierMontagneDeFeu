#ifndef COMBAT_H
#define COMBAT_H

#include "../destinLib/abs/lancerde.h"

enum CapaciteCreature {
    SuceurHabilete, // retire un point d'habileté au joueur chaque fois qu'il le blesse 3 fois
    Aveugle, // la créature est aveuglée :
    /*
     * Vous pouvez ajouter 2 points au chiffre obtenu en lançant le dé lorsque
        vous déterminez votre Force d'Attaque.
        Chaque assaut victorieux lui enlève 3 points d'ENDURANCE, car,
        comme il ne peut pas vous voir, il lui est impossible de se défendre
        efficacement.
        Chaque fois qu'il vous inflige une blessure, jetez un dé. Si le chiffre
        obtenu est impair, il vous a blessé normalement. Si ce chiffre est 2 ou 4,
        il ne vous a infligé qu'une blessure à 1 point. Si vous faites un 6, vous
        avez paré le coup et vous n'êtes pas blessé.*/
    GoToAuPremierSang /* utilisé pour les ennemis qui nous emmènent à un autre numéro
        dès qu'on les touche (le combat est alors considéré comme temporaireemnt 'fini'
        quitte à recommencer au numéro en question*/
};

struct Creature {
    Creature(QString nom, int hab, int end, QVector<CapaciteCreature> capacites = {})
        :m_Nom(nom), m_Habilete(hab), m_Endurance(end), m_CapacitesCreature(capacites) {}
    Creature() {}

    QString m_Nom = "chais pas";
    int m_Habilete = -1;
    int m_Endurance = -1;

    QVector<CapaciteCreature> m_CapacitesCreature = {};

    bool ACetteCapacite(CapaciteCreature capaciteCreature) {
        for (int var = 0; var < m_CapacitesCreature.length(); ++var) {
            if ( m_CapacitesCreature[var] == capaciteCreature )
                return true;
        }
        return false;
    }
};

enum PhaseCombat {
    AucunCombatEnCours,
    AttaqueJoueur,
    AttaqueEnnemi
};

struct Creature;

/**
 * @brief gère les combats en général
 * Utilisée lors de la construction  des combats en phase génération.
 * Une fois en pahse d'exécution elle contient les données du combat actuel si il y en a un.
 * => pourrait être un de ces jours divisé en GenCombat, COmbat, et ExecCombat si tout cela devient trop bordellique
 */
class Combat
{
    static Combat* ME;

public:
    Combat();

    static Combat* GetCombat();

    std::shared_ptr<LancerDe> AjouterCombat(
            std::shared_ptr<Effet> effet, QVector<std::shared_ptr<Creature>> creatures);
    void AjouterCombatAvecFuite(
            std::shared_ptr<Effet> effet, QVector<std::shared_ptr<Creature>> creatures, QString texteFuite, QString idFuite);
    std::shared_ptr<Creature> GetEnnemiActuel();

    // caractéristiques "gameplay" nécessaires pour le système mais pas affichées
    int m_NumDeCombat;// quand il y a plusieurs monstres à affronter l'un derrière l'autre (premier = 0)
    int m_ResAttaqueJoueur;
    int m_ResAttaqueEnnemi;
    PhaseCombat m_PhaseCombat = AucunCombatEnCours;
    QVector<std::shared_ptr<Creature>> m_Ennemis = {};
    int m_NbBlessuresRecues;

    // fonctions "runtime" : ne peuvent être appelée que lors de l'exécution du combat, et donc qu'il y a au moins une créature vivante
    void CommencerCombat(QVector<std::shared_ptr<Creature>> creatures);
    void FinirCombat();
    bool TourDeCombat(int resDes, QString &resTxt);
    QString GetIntituleCombat(int indexCombat = -1);
    void AjouterCaracAMonstre(CapaciteCreature capaciteCreature);
    void AjouterFuiteAuCombat(QString texteFuite, QString idFuite);
};

#endif // COMBAT_H
