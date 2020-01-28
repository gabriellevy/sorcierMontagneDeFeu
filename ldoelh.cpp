#include "ldoelh.h"
#include "ui_univers.h"
#include "gensorcmontagnefee.h"
#include "combat.h"

using std::shared_ptr;
using std::make_shared;

QString LDOELH::ENDURANCE = "Endurance";
QString LDOELH::HABILETE = "Habileté";
QString LDOELH::CHANCE = "Chance";
QString LDOELH::REPAS = "Repas";

QString LDOELH::SORCIER_MONTAGNE_FEU = "Le sorcier de la montagne de feu";

LDOELH::LDOELH(ModeAffichage modeAffichage,
               QWidget *parent)
: Univers(parent, modeAffichage)
{
    this->AppliquerTheme(QColor(240, 214, 113));

    m_GestCombat = make_shared<Combat>();

    m_GensHistoire[LDOELH::SORCIER_MONTAGNE_FEU] = make_shared<GenSorcMontagneFeu>();
}

void LDOELH::InitialiserCaracsBase()
{

}
