#ifndef LDOELH_H
#define LDOELH_H

#include "../destinLib/univers.h"
#include <QMainWindow>

struct Creature {
    Creature(QString nom, int hab, int end):m_Nom(nom), m_Habilete(hab), m_Endurance(end) {}
    Creature() {}

    QString m_Nom = "chais pas";
    int m_Habilete = -1;
    int m_Endurance = -1;
};

/**
 * @brief la classe livre dont on est le héros
 */
class LDOELH : public Univers
{
    Q_OBJECT

public:
    explicit LDOELH(ModeAffichage modeAffichage = ModeAffichage::ema_Jeu,
                    QWidget *parent = nullptr);

    static QString SORCIER_MONTAGNE_FEU;

    // caractérisiques principales (au sens jdr)
    static QString HABILETE;
    static QString ENDURANCE;
    static QString CHANCE;
    // caractéristiques "gameplay" nécessaires pour le système mais pas affichées
    static QString PHASE_COMBAT; // qui est en train de frapper
    static QString NUM_DE_COMBAT; // quand il y a plusieurs monstres à affronter l'un derrière l'autre (premier = 0)
    static QString RES_ATTAQUE_JOUEUR;
    static QString RES_ATTAQUE_ENNEMI;
    static QString ENDURANCE_ENNEMI;
    void InitialiserCaracsBase();
};

#endif // LDOELH_H
