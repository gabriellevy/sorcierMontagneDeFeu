#ifndef COMBAT_H
#define COMBAT_H

#include "../destinLib/lancerde.h"

struct Creature {
    Creature(QString nom, int hab, int end):m_Nom(nom), m_Habilete(hab), m_Endurance(end) {}
    Creature() {}

    QString m_Nom = "chais pas";
    int m_Habilete = -1;
    int m_Endurance = -1;
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

    LancerDe* AjouterCombat(Effet* effet, QVector<Creature*> creatures);
    void AjouterCombatAvecFuite(Effet* effet, QVector<Creature*> creatures, QString texteFuite, QString idFuite);
    Creature* GetEnnemiActuel();

    // caractéristiques "gameplay" nécessaires pour le système mais pas affichées
    int m_NumDeCombat;// quand il y a plusieurs monstres à affronter l'un derrière l'autre (premier = 0)
    int m_ResAttaqueJoueur;
    int m_ResAttaqueEnnemi;
    PhaseCombat m_PhaseCombat = AucunCombatEnCours;
    QVector<Creature*> m_Ennemis = {};
    int m_NbBlessuresRecues;

    // fonctions "runtime"
    void CommencerCombat(QVector<Creature*> creatures);
    void FinirCombat();
    bool TourDeCombat(int resDes, QString &resTxt);
    QString GetIntituleCombat(int indexCombat = -1);
};

#endif // COMBAT_H
