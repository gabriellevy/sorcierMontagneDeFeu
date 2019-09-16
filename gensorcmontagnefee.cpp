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
    GenererNumeros();

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


void GenSorcMontagneFeu::GenererNumeros()
{
    //1
    AjouterEffetNarration("Vos deux jours de marche sont enfin terminés. Après avoir ôté votre"
                          "épée de son fourreau, vous la déposez sur le sol et vous poussez un"
                          "soupir de soulagement en vous asseyant sur un rocher couvert de"
                          "mousse pour prendre quelques instants de repos. Vous vous étirez, vous"
                          "vous frottez les yeux, puis vous levez votre regard vers la montagne au"
                          "sommet de feu. La montagne elle-même paraît menaçante. Son flanc"
                          "escarpé, face à vous, semble avoir été déchiqueté par les griffes de"
                          "quelque créature gigantesque. Il est hérissé d'à-pics rocheux aux angles"
                          "tranchants dont on a peine à croire qu'ils aient été façonnés par la"
                          "nature. Au sommet, on aperçoit une couleur d'un rouge sinistre - sans"
                          "doute l'effet d'une étrange végétation - qui a donné son nom à la"
                          "montagne. Personne, peut-être, ne saura jamais ce qui pousse là-haut,"
                          "car il est certainement impossible d'escalader ce pic.\n\n"
                          "Votre quête commence maintenant. De l'autre côté de la clairière, il y a"
                          "l'entrée d'une caverne sombre. Vous ramassez votre épée, vous vous"
                          "relevez et vous pensez à tous les dangers qui vous attendent. Puis, avec"
                          "détermination, vous remettez l'épée dans son fourreau et vous vous"
                          "avancez vers l'entrée de la caverne. Vous jetez un coup d'oeil dans les"
                          "ténèbres et vous apercevez des parois suintantes et sombres ainsi que"
                          "des flaques d'eau sur le sol de pierre. L'air est froid et humide. Vous"
                          "allumez votre lanterne et vous faites prudemment quelques pas dans"
                          "l'obscurité. Des toiles d'araignées vous balaient le visage et vous"
                          "entendez le bruit que font sur le sol des pattes minuscules ; ce sont"
                          "probablement des rats qui prennent la fuite. Vous entrez dans la"
                          "caverne. Après avoir parcouru quelques mètres, vous arrivez à une"
                          "bifurcation.",
           "", "1");
    AjouterChoixGoToEffet("Irez-vous vers l'ouest", "71");
    AjouterChoixGoToEffet("ou vers l'est", "278");

    //2

}

void GenSorcMontagneFeu::GenererEvtsAccueil()
{
    this->AjouterEvt("Debut", "Génération du perso par les choix");
    AjouterEffetNarration("", ":/images/couverture.PNG", "couverture");

    //GenererCombatBidon();
}
