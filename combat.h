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
    AttaqueJoueur,
    AttaqueEnnemi
};

struct Creature;

/**
 * @brief gère les combats en général
 * Utilisée lors de la construction  des combats en phase génération.
 * Une fois en pahse d'exécution elle contient les données du combat actuel si il y en a un.
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
    PhaseCombat m_PhaseCombat;
    QVector<Creature*> m_Ennemis = {};
    int m_NbBlessuresRecues;
    static QString PHASE_COMBAT; // qui est en train de frapper
    static QString NUM_DE_COMBAT;
    static QString RES_ATTAQUE_JOUEUR;
    static QString RES_ATTAQUE_ENNEMI;
    static QString ENDURANCE_ENNEMI;

    // fonctions "runtime"
    void CommencerCombat(QVector<Creature*> creatures);
    void FinirCombat();
};

#endif // COMBAT_H
