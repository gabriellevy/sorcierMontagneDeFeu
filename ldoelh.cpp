#include "ldoelh.h"
#include "ui_univers.h"
#include "gensorcmontagnefee.h"

QString LDOELH::ENDURANCE = "Endurance";
QString LDOELH::HABILETE = "Habileté";
QString LDOELH::CHANCE = "Chance";
QString LDOELH::PHASE_COMBAT = "PhaseCombat";
QString LDOELH::RES_ATTAQUE_JOUEUR = "ResAttaqueJoueur";
QString LDOELH::RES_ATTAQUE_ENNEMI = "ResAttaqueEnnemi";
QString LDOELH::ENDURANCE_ENNEMI = "EnduranceEnnemi";
QString LDOELH::NUM_DE_COMBAT = "NumDeCombat";


QString LDOELH::SORCIER_MONTAGNE_FEU = "Le sorcier de la montagne de feu";

LDOELH::LDOELH(ModeAffichage modeAffichage,
               QWidget *parent)
: Univers(parent, modeAffichage)
{
    this->AppliquerTheme(QColor(240, 214, 113));

    m_GensHistoire[LDOELH::SORCIER_MONTAGNE_FEU] = new GenSorcMontagneFeu();
}

void LDOELH::InitialiserCaracsBase()
{

}
