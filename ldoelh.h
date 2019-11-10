#ifndef LDOELH_H
#define LDOELH_H

#include "../destinLib/univers.h"
#include <QMainWindow>

class Combat;

/**
 * @brief la classe livre dont on est le héros
 */
class LDOELH : public Univers
{
    Q_OBJECT

public:
    explicit LDOELH(ModeAffichage modeAffichage = ModeAffichage::ema_Jeu,
                    QWidget *parent = nullptr);

    Combat* m_GestCombat = nullptr;

    static QString SORCIER_MONTAGNE_FEU;

    // caractérisiques principales (au sens jdr)
    static QString HABILETE;
    static QString ENDURANCE;
    static QString CHANCE;
    static QString REPAS;
    void InitialiserCaracsBase();
};

#endif // LDOELH_H
