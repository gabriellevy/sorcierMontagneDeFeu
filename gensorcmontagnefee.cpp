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
    GenererEffetsGeneriques();

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

void GenSorcMontagneFeu::AjouterCombat(Effet* effet, QString nomMonstre, int habileteMonstre, int enduranceDepartMonstre)
{
    QString texteCombatBase = nomMonstre + " - HABILITÉ : " + QString::number(habileteMonstre) +
                                         " ENDURANCE : ";
    effet->m_Texte += "\n\n" + texteCombatBase;

    std::function<ResExecutionLancerDe*(int)> lancerHabiliteJoueur = [texteCombatBase, nomMonstre, habileteMonstre, enduranceDepartMonstre](int i) {
        QString resTxt = "";
        QString phaseActuelle = GestionnaireCarac::GetCaracValue(LDOELH::PHASE_COMBAT);
        QString phaseJoueur = "phaseJoueur";
        QString phaseEnnemi = "phaseEnnemi";
        bool combatContinue = true;
        int enduranceActuelleMonstre = GestionnaireCarac::GetCaracValueAsInt(LDOELH::ENDURANCE_ENNEMI);
        if ( enduranceActuelleMonstre <= 0)
            enduranceActuelleMonstre = enduranceDepartMonstre;

        std::function<QString()> getIntituleCombat = [texteCombatBase]() {
            return "\n\n" + texteCombatBase + GestionnaireCarac::GetCaracValue(LDOELH::ENDURANCE_ENNEMI);
        };

        if ( phaseActuelle == "")
            phaseActuelle = phaseJoueur;

        if ( phaseActuelle == phaseJoueur) {
            // le joueur attaque (lance les dés)
            int resTotal = GestionnaireCarac::GetCaracValueAsInt(LDOELH::HABILETE) + i;
            resTxt  = "Votre force d'attaque : "  + QString::number(resTotal);
            GestionnaireCarac::SetValeurACaracId(LDOELH::RES_ATTAQUE_JOUEUR, resTotal);

            // au tour du monstre :
            GestionnaireCarac::SetValeurACaracId(LDOELH::PHASE_COMBAT, phaseEnnemi);
        } else {
            // le monstre attaque et on résoud le duel
            int resEnnemi = habileteMonstre + i;
            resTxt  = nomMonstre + ", force d'attaque  : "  + QString::number(resEnnemi);
            GestionnaireCarac::SetValeurACaracId(LDOELH::RES_ATTAQUE_ENNEMI, resEnnemi);

            int resJoueur = GestionnaireCarac::GetCaracValueAsInt(LDOELH::RES_ATTAQUE_JOUEUR);
            // conclusion de la phase de combat :
            if (resJoueur > resEnnemi) {
                // le joueur a l'avantage :
                enduranceActuelleMonstre -= 2;
                GestionnaireCarac::SetValeurACaracId(LDOELH::ENDURANCE_ENNEMI, enduranceActuelleMonstre);
                if ( enduranceActuelleMonstre <= 0) {
                    // l'ennemi est mort :
                    resTxt += "\nL'ennemi est mort. Victoire !";
                    combatContinue = false;
                } else {
                    // l'ennemi est blessé :
                    resTxt += "\nL'ennemi est blessé.";
                    resTxt += getIntituleCombat();
                }
            }
            else if (resJoueur < resEnnemi)
            {
                // le monstre a l'avantage :
                int endurance = GestionnaireCarac::RetirerValeurACaracId(LDOELH::ENDURANCE, 2);
                if ( endurance <= 0) {
                    // perdu... :
                    resTxt += "\nVous êtes mort.";
                    combatContinue = false;
                    Univers::ME->GetExecHistoire()->GetExecEffetActuel()->GetEffet()->m_GoToEffetId = "mort";
                } else {
                    // joueur blessé :
                    resTxt += "\nVous êtes blessé.";
                    resTxt += getIntituleCombat();
                }
            } else {
                // égalité
                resTxt += "\nÉgalité.";
                resTxt += getIntituleCombat();
            }

            // au tour du joueur
            GestionnaireCarac::SetValeurACaracId(LDOELH::PHASE_COMBAT, phaseJoueur);
            IPerso::GetPersoInterface()->RafraichirAffichage();
        }
        return new ResExecutionLancerDe(resTxt, combatContinue);
    };

    m_GenerateurEvt->AjouterLancerDe("Combat", 2, lancerHabiliteJoueur);
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
    AjouterEffetNarration("Vos deux jours de marche sont enfin terminés. Après avoir ôté votre "
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
    AjouterChoixGoToEffet("ou vers l'est", "278");

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
    // 3
    AjouterEffetNarration("La cloche tinte faiblement, et quelques instants plus tard, vous voyez un vieil homme tout "
                          "desséché grimper dans une barque amarrée à la rive nord. Il rame lentement dans votre direction, "
                          "puis s'avance vers vous en clopinant. Il vous demande 3 Pièces d'Or. Vous protestez contre ce "
                          "tarif trop élevé, et il marmonne une vague excuse en invoquant «l'inflation». Au bout d'un moment, "
                          "vos protestations le mettent en colère. ",
                          "", "3");
    AjouterChoixGoToEffet("Allez vous lui payer les trois Pièces d'Or", "272");
    AjouterChoixGoToEffet("ou le menacer", "127");

    // 4
    AjouterEffetNarration("Vous vous trouvez dans un couloir nord-sud. Au nord, le passage s'oriente vers l'est à quelques mètres devant vous.  ",
                          "", "4");
    AjouterChoixGoToEffet("Si vous voulez explorer l'endroit", "46");
    AjouterChoixGoToEffet("Au sud, le couloir tourne également vers l'est. Si vous préférez aller de ce côté", "332");

    // 5
    AjouterEffetNarration("Sur le mur est du passage, il y a une porte de bois brut. En écoutant à la porte, vous entendez quelqu'un chantonner gaiement.",
                          "", "5");
    AjouterChoixGoToEffet("Voulez-vous frapper à la porte et entrer", "97");
    AjouterChoixGoToEffet(" ou continuerezvous vers le nord", "292");

    // 6
    Effet* effet6 = AjouterEffetNarration("La haute porte n'a pas de poignée. Vous essayez de l'enfoncer, mais sans résultat. La porte ne bougera pas. 3"
                          "Vous décidez d'abandonner et vous repassez par l'ouverture du couloir est-ouest à quelque distance en arrière. ",
                          "", "6");
    effet6->m_GoToEffetId = "89";

    //7
    Effet* effet7 = AjouterEffetNarration("Vous êtes sur la berge nord d'une rivière au fort courant, dans une grande caverne souterraine. ",
                          "", "7");
    effet7->m_GoToEffetId = "214";

    //8
    Effet* effet8 = AjouterEffetNarration(
                "Le passage devant vous aboutit à une porte solide. Vous essayez d'écouter, mais vous n'entendez rien. "
                "Vous tournez alors la poignée, la porte s'ouvre et vous entrez dans dans la pièce. Tandis que vous y jetez un coup d'oeil, "
                "vous entendez un grand cri derrière vous et vous Vous retournez aussitôt : "
                "un homme aux allures de sauvage bondit sur vous en brandissant une hache d'armes. C'est un BARBARE fou, et il vous faut le combattre. ",
                "", "8");
    AjouterCombat(effet8, "BARBARE", 7, 6);
    //        plus la possibilité de se barrer en cours de combat !
    //AjouterChoixGoToEffet("Il y a une porte dans le mur d'en face, situé au nord. Vous pouvez vous enfuir par là pendant le combat", "189");
    effet8->m_GoToEffetId = "273";
}

void GenSorcMontagneFeu::GenererEffetsGeneriques()
{
    AjouterEffetNarration("Vous êtes mort", "", "mort");
}

void GenSorcMontagneFeu::GenererEvtsAccueil()
{
    this->AjouterEvt("Debut", "Génération du perso par les choix");
    //AjouterEffetNarration("", ":/images/couverture.PNG", "couverture");

    //GenererCombatBidon();
}
