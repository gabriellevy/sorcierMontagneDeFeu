#include "gensorcmontagnefee.h"
#include "../destinLib/gestionnairecarac.h"
#include "ldoelh.h"
#include "../destinLib/aleatoire.h"
#include "../destinLib/perso.h"
#include "../destinLib/execeffet.h"
#include "../destinLib/lancerde.h"
#include "../destinLib/setcarac.h"
#include <functional>

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


bool TenterLaChance(int resDe)
{
    QString resTxt = "Résultat des dés : " + QString::number(resDe) + "\n\n";
    qDebug() << resTxt;

    GestionnaireCarac::AJouterValeurACaracId(LDOELH::CHANCE, -1);
    IPerso::GetPersoInterface()->RafraichirAffichage();

    return ( resDe < GestionnaireCarac::GetCaracValueAsInt(LDOELH::CHANCE));
}

ResExecutionLancerDe* ExecutionCombatDe(int resDe/*, QVector<QString> params*/)
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



void GenSorcMontagneFeu::TenterLaChanceGoTo(QString texteMalchanceux, QString effet_malchanceux_id,
                   QString texteChanceux, QString effet_chanceux_id)
{
    std::function<ResExecutionLancerDe*(int)> tenter = [texteMalchanceux, effet_malchanceux_id, texteChanceux, effet_chanceux_id](int i) {
        bool chanceux = TenterLaChance(i);

        QString resTxt = "";

        if ( chanceux ) {
            resTxt += texteChanceux;
            Univers::ME->GetExecHistoire()->EffetActuel()->m_GoToEffetId = effet_chanceux_id;

        } else {
            resTxt += texteMalchanceux;
            Univers::ME->GetExecHistoire()->EffetActuel()->m_GoToEffetId = effet_malchanceux_id;
        }
        return new ResExecutionLancerDe(resTxt, false);
    };

    // tenter la chance se fait toujours avec deux dés
    m_GenerateurEvt->AjouterLancerDe("Tentez votre chance", 2, tenter);
}


void GenSorcMontagneFeu::GenererNumeros()
{
    //1
    /*AjouterEffetNarration("Vos deux jours de marche sont enfin terminés. Après avoir ôté votre "
                          "épée de son fourreau, vous la déposez sur le sol et vous poussez un "
                          "soupir de soulagement en vous asseyant sur un rocher couvert de "
                          "mousse pour prendre quelques instants de repos. Vous vous étirez, vous "
                          "vous frottez les yeux, puis vous levez votre regard vers la montagne au "
                          "sommet de feu. La montagne elle-même paraît menaçante. Son flanc "
                          "escarpé, face à vous, semble avoir été déchiqueté par les griffes de "
                          "quelque créature gigantesque. Il est hérissé d'à-pics rocheux aux angles "
                          "tranchants dont on a peine à croire qu'ils aient été façonnés par la "
                          "nature. Au sommet, on aperçoit une couleur d'un rouge sinistre - sans "
                          "doute l'effet d'une étrange végétation - qui a donné son nom à la "
                          "montagne. Personne, peut-être, ne saura jamais ce qui pousse là-haut, "
                          "car il est certainement impossible d'escalader ce pic.\n\n"
                          "Votre quête commence maintenant. De l'autre côté de la clairière, il y a "
                          "l'entrée d'une caverne sombre. Vous ramassez votre épée, vous vou "
                          "relevez et vous pensez à tous les dangers qui vous attendent. Puis, ave c"
                          "détermination, vous remettez l'épée dans son fourreau et vous vous "
                          "avancez vers l'entrée de la caverne. Vous jetez un coup d'oeil dans les "
                          "ténèbres et vous apercevez des parois suintantes et sombres ainsi que "
                          "des flaques d'eau sur le sol de pierre. L'air est froid et humide. Vou "
                          "allumez votre lanterne et vous faites prudemment quelques pas dans "
                          "l'obscurité. Des toiles d'araignées vous balaient le visage et vous "
                          "entendez le bruit que font sur le sol des pattes minuscules ; ce sont "
                          "probablement des rats qui prennent la fuite. Vous entrez dans la "
                          "caverne. Après avoir parcouru quelques mètres, vous arrivez à une "
                          "bifurcation.",
           "", "1");
    AjouterChoixGoToEffet("Irez-vous vers l'ouest", "71");
    AjouterChoixGoToEffet("ou vers l'est", "278");*/

    //2
    AjouterEffetNarration("Tentez votre Chance. Si vous êtes chanceux, vous parvenez à vous "
                          "enfuir sans éveiller l'attention de l'Ogre.",
                          "", "2");
    TenterLaChanceGoTo("Vous êtes malchanceux,"
                       "vous poussez un juron en trébuchant sur une petite pierre que vous "
                       "envoyez rouler à travers la caverne. Vous tirez votre épée, au cas où "
                       "l'Ogre vous aurait entendu",
                       "16",
                       "Vous avez eu de la chance, vous rampez le long du couloir jusqu'au croisement.", "269"
                       );

}

void GenSorcMontagneFeu::GenererEvtsAccueil()
{
    this->AjouterEvt("Debut", "Génération du perso par les choix");
    //AjouterEffetNarration("", ":/images/couverture.PNG", "couverture");

    //GenererCombatBidon();
}
