#include "gensorcmontagnefee.h"
#include "../destinLib/gestionnairecarac.h"
#include "ldoelh.h"
#include "../destinLib/aleatoire.h"
#include "../destinLib/perso.h"
#include "../destinLib/execeffet.h"
#include "../destinLib/lancerde.h"

GenSorcMontagneFeu::GenSorcMontagneFeu():GenHistoire () {}

Hist* GenSorcMontagneFeu::GenererHistoire()
{
    GenererPersos();

    //GenererFonctionsCallback();

    GenererCaracs();

    GenererEvtsAccueil();

    FinGenerationHistoire();

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
    //GetUniversSkaven()->GenererTousLesClans();
    // initilisée via le perso
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

    GenererCombatBidon();
}
