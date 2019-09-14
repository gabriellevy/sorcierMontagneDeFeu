#include "gensorcmontagnefee.h"
#include "../destinLib/gestionnairecarac.h"
#include "ldoelh.h"
#include "../destinLib/aleatoire.h"
#include "../destinLib/perso.h"
#include "../destinLib/execeffet.h"
#include "../destinLib/lancerde.h"
#include "../destinLib/setcarac.h"

GenSorcMontagneFeu::GenSorcMontagneFeu():GenHistoire () {}

Hist* GenSorcMontagneFeu::GenererHistoire()
{
    GenererPersos();

    //GenererFonctionsCallback();

    GenererEvtsAccueil();

    FinGenerationHistoire();

    GenererCaracs();

    return m_HistoireGeneree;
}

void GenSorcMontagneFeu::GenererPersos()
{
    QString nom = "pas encore déterminé";
    DPerso* perso = new DPerso(nom, nom, nom, "");
    perso->InitialiserPerso();
    IPerso::AjouterPersoJouable(perso);
}

void GenSorcMontagneFeu::GenererCaracs()
{
    GestionnaireCarac::GetGestionnaireCarac()->AjouterCaracNombre(LDOELH::HABILETE);
    int habilete = 6 + Aleatoire::GetAl()->D6();
    GestionnaireCarac::GetGestionnaireCarac()->SetValeurACaracId( LDOELH::HABILETE, habilete);

    GestionnaireCarac::GetGestionnaireCarac()->AjouterCaracNombre(LDOELH::ENDURANCE);
    int endurance = 12 + Aleatoire::GetAl()->D6() + Aleatoire::GetAl()->D6();
    GestionnaireCarac::GetGestionnaireCarac()->SetValeurACaracId( LDOELH::ENDURANCE, endurance);

    GestionnaireCarac::GetGestionnaireCarac()->AjouterCaracNombre(LDOELH::CHANCE);
    int chance = 6 + Aleatoire::GetAl()->D6();
    GestionnaireCarac::GetGestionnaireCarac()->SetValeurACaracId( LDOELH::CHANCE, chance);
}

ResExecutionLancerDe* ExecutionCombatDe(int resDe, QVector<QString> params)
{
    QString resTxt = "je suis dans l'exécution de ExecutionCombatDe. Résultat du dé : " + QString::number(resDe);
    //qDebug()<<res<<endl;
    bool fini = (resDe < 6);
    if ( fini)
        resTxt = "Bravo vous avez gagné !";
    return new ResExecutionLancerDe(resTxt, !fini);
}

void GenSorcMontagneFeu::GenererCombatBidon()
{
    Effet* effet = AjouterEffetNarration("Vous allez lancer un dé j'imagine.",
           "", "lancerDe");

    LancerDe* lancerDe = new LancerDe(effet, "Combattre", 3, ExecutionCombatDe);
    effet->m_LancerDe = lancerDe;


    AjouterEffetNarration("Le combat est fini j'imagine.",
           "", "finiCombat");
}

void GenSorcMontagneFeu::GenererEvtsAccueil()
{
    this->AjouterEvt("Debut", "Génération du perso par les choix");
    AjouterEffetNarration("", ":/images/couverture.PNG", "couverture");

    GenererCombatBidon();
}
