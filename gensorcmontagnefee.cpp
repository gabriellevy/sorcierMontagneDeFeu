#include "gensorcmontagnefee.h"
#include "../destinLib/gestionnairecarac.h"
#include "ldoelh.h"
#include "../destinLib/aleatoire.h"
#include "../destinLib/perso.h"
#include "../destinLib/execeffet.h"
#include "../destinLib/lancerde.h"
#include "../destinLib/setcarac.h"
#include "../destinLib/choix.h"
#include <functional>
#include "combat.h"

GenSorcMontagneFeu::GenSorcMontagneFeu():GenHistoire () {}

GenSorcMontagneFeu* GenSorcMontagneFeu::GetGenSorcMontagneFeu()
{
    return static_cast<GenSorcMontagneFeu*>(ME);
}

Hist* GenSorcMontagneFeu::GenererHistoire()
{
    GenererPersos();

    //GenererFonctionsCallback();

    GenererEvtsAccueil();
    GenererNumeros1_10();
    GenererNumeros11_20();
    GenererNumeros21_30();
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
    int habilete = 6 + Aleatoire::GetAl()->D6();
    GestionnaireCarac::GetGestionnaireCarac()->AjouterCaracNombre(LDOELH::HABILETE, habilete, 0, habilete);

    int endurance = 12 + Aleatoire::GetAl()->D6() + Aleatoire::GetAl()->D6();
    GestionnaireCarac::GetGestionnaireCarac()->AjouterCaracNombre(LDOELH::ENDURANCE, endurance, 0, endurance);

    int chance = 6 + Aleatoire::GetAl()->D6();
    GestionnaireCarac::GetGestionnaireCarac()->AjouterCaracNombre(LDOELH::CHANCE, chance, 0, chance);
}


bool TenterLaChance(int resDe)
{
    QString resTxt = "Résultat des dés : " + QString::number(resDe) + "\n\n";
    qDebug() << resTxt;

    GestionnaireCarac::AJouterValeurACaracId(LDOELH::CHANCE, -1);
    IPerso::GetPersoInterface()->RafraichirAffichage();

    return ( resDe < GestionnaireCarac::GetCaracValueAsInt(LDOELH::CHANCE));
}



void GenSorcMontagneFeu::TenterLaChanceGoTo(QString texteMalchanceux, QString effet_malchanceux_id,
                   QString texteChanceux, QString effet_chanceux_id,
                   std::function<void()> malchanceuxCallback, std::function<void()> chanceuxCallback)
{
    std::function<ResExecutionLancerDe*(int)> tenter =
            [texteMalchanceux, effet_malchanceux_id, texteChanceux, effet_chanceux_id, malchanceuxCallback, chanceuxCallback](int i) {
        bool chanceux = TenterLaChance(i);

        QString resTxt = "";

        if ( chanceux ) {
            if ( chanceuxCallback != nullptr )
                chanceuxCallback();
            resTxt += texteChanceux;
            Univers::ME->GetExecHistoire()->EffetActuel()->m_GoToEffetId = effet_chanceux_id;

        } else {
            if ( malchanceuxCallback != nullptr )
                malchanceuxCallback();
            resTxt += texteMalchanceux;
            Univers::ME->GetExecHistoire()->EffetActuel()->m_GoToEffetId = effet_malchanceux_id;
        }
        return new ResExecutionLancerDe(resTxt, false);
    };

    // tenter la chance se fait toujours avec deux dés
    m_GenerateurEvt->AjouterLancerDe("Tentez votre chance", 2, tenter);
}


void GenSorcMontagneFeu::GenererNumeros1_10()
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
           ":/images/1.PNG", "1");
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
    Combat::GetCombat()->AjouterCombatAvecFuite(effet8, {new Creature("BARBARE", 7, 6)},
                           "Il y a une porte dans le mur d'en face, situé au nord. Vous pouvez vous enfuir par là pendant le combat",
                           "189");
    effet8->m_GoToEffetId = "273";

    // 9
    AjouterEffetNarration("Stupéfait que votre mensonge ait réussi, vous décidez de pousser un peu plus loin votre chance. "
                          "Vous pouvez soit examiner les outils du Squelette, soit faire semblant de chercher des feuilles de plans de travail, "
                          "en fouillant les tiroirs des divers établis. \n"
                          "Vous entendez un bruit qui vient de derrière la porte située au nord, et vous réalisez qu'il va falloir vous dépêcher ! ",
                          "", "9");
    AjouterChoixGoToEffet(" Si vous choisissez les outils", "34");
    AjouterChoixGoToEffet("Si vous fouillez les tiroirs", "322");

    //10
    /*Effet* effet10 = */AjouterEffetNarration("Vous êtes revenu à la bifurcation et vous prenez la direction du nord. ",
                          "", "10");
    effet7->m_GoToEffetId = "77";
}

void GenSorcMontagneFeu::GenererEffetsGeneriques()
{
    AjouterEffetNarration("Vous êtes mort", "", "mort");
}

void GenSorcMontagneFeu::GenererNumeros11_20()
{
    //11
    Effet* effet11 = AjouterEffetNarration("Vous suivez le passage vers l'ouest jusqu'à ce qu'il tourne vers le sud. "
                          "Juste avant ce coude, il y a une pancarte qui indique : « En "
                          "construction. » Devant vous, les premières marches d'un escalier qui "
                          "descend. Trois marches, seulement ont été construites. Sur le sol sont "
                          "posés des pelles, des pioches et d'autres outils et lorsque vous avez "
                          "tourné le coin, ils se mettent soudain à s'agiter et à travailler pour "
                          "continuer de bâtir l'escalier. Vous voyez maintenant tous ces outils "
                          "creuser et piocher comme s'ils étaient tenus par des ouvriers invisibles. "
                          "Une chanson fredonnée s'élève et vous reconnaissez l'air de « Le "
                          "travail, c'est la Santé ». Devant ce spectacle, vous éclatez de rire. La "
                          "scène en effet est cocasse. Vous vous asseyez pour observer ces outils "
                          "magiques ; vous parlez même à certains d'entre eux. Prenez 2 points "
                          "d'ENDURANCE et 1 point d'HABILETÉ pendant que vous vous "
                          "détendez. Puis, revenez dans le passage en remontant vers le "
                          "croisement ; ",
           ":/images/11.PNG", "11");
    effet11->AjouterAjouteurACarac(LDOELH::HABILETE, "1");
    effet11->AjouterAjouteurACarac(LDOELH::ENDURANCE, "2");
    AjouterChoixGoToEffet("là, vous pouvez choisir d'aller au nord", "366");
    AjouterChoixGoToEffet("ou au sud", "250");

    //12
    /*Effet* effet12 = */AjouterEffetNarration(
                "Au moment où vous tirez la poignée, un bruit métallique assourdissant "
                "retentit dans le passage. Vous la repoussez frénétiquement pour arrêter "
                "le signal d'alarme, mais il a déjà produit son effet. Vous entendez des "
                "bruits de pas qui s'approchent dans le couloir.",
           "", "12");
    Effet* effet161_12 = GenererNumeros161();
    AjouterChoixGoToEffet("vous pouvez soit retourner à la bifurcation", "256", "", effet161_12);
    AjouterChoixGoToEffet("soit pousser la poignée", "364", "", effet161_12);

    //13
    Effet* effet13 = AjouterEffetNarration(
                "Votre tête vous fait mal et vous vous sentez tout étourdi en vous "
                "relevant. Les quatre hommes se met-tent en mouvement et s'avancent "
                "vers vous en file indienne, leurs armes prêtes. Vous cherchez votre "
                "chemin à tâtons le long du mur pour essayer d'atteindre la porte du sud, "
                "et il s'en faudra d'un cheveu que vous n'y arriviez. Votre pied glisse sur "
                "un caillou et vous tombez par terre. Avant d'avoir pu vous relever, les "
                "créatures sont sur vous.",
           "", "13");
    effet13->m_GoToEffetId = "282";

    //14
    /*Effet* effet14 = */AjouterEffetNarration(
                "Il n'y a pas trace de passage secret ; en revanche, vous entendez des "
                "bruits de pas qui viennent vers vous.",
           "", "14");
    Effet* effet161_14 = GenererNumeros161();
    effet161_14->m_GoToEffetId = "117";

    //15
    Effet* effet15 = AjouterEffetNarration(
                "Tandis que vous êtes assis sur le banc en train de manger, vous vous "
                "détendez profondément et les courbatures de votre corps semblent """
                "disparaître d'elles-mêmes. Ce lieu de repos est enchanté. Vous avez "
                "droit à deux points d'ENDURANCE en plus de ceux que vous rend "
                "votre repas (mais seulement si ce local n'excède pas vos points "
                "d'ENDURANCE de départ ; vous pouvez également reprendre 1 point "
                "D’HABILETÉ si vous en avez perdu. Lorsque vous êtes l'ict à repartir, "
                "avancez le long du couloir""",
           "", "15");
    effet15->AjouterAjouteurACarac(LDOELH::ENDURANCE, 2);
    effet15->AjouterAjouteurACarac(LDOELH::HABILETE, 1);
    effet15->m_GoToEffetId = "367";

    //16
    Effet* effet16 = AjouterEffetNarration(
                "Vous tirez votre épée du fourreau; l'Ogre vous a entendu et se prépare à "
                "l'attaque",
           "", "16");
    Combat::GetCombat()->AjouterCombatAvecFuite(effet16, {new Creature("OGRE", 8, 10)},
                           "Après le deuxième assaut, vous pouvez fuir le long du corridor.", "269");
    effet16->m_GoToEffetId = "50";

    //17
    /*Effet* effet17 = */AjouterEffetNarration(
                "A l'aide de l'épieu et du maillet (ou d'un maillet de fortune si vous n'en "
                "avez pas), vous formez une croix et vous avancez vers le Vampire en "
                "l'acculant dans un coin. Le Vampire siffle et essaye de vous attraper, "
                "mais il ne peut s'approcher de vous. Il sera cependant difficile de lui "
                "enfoncer l'épieu dans le coeur. Tandis que vous marchez vers lui, vous "
                "trébuchez et tombez. Par un coup de chance, l'épieu est projeté en "
                "avant, et atteint le monstre hurlant.",
                "", "17");
    TenterLaChanceGoTo("vous êtes "
                       "malchanceux", "17_b", "vous avez eu de la "
                       "chance, vous avez tué le Vampire, vous pouvez chercher son "
                       "trésor", "327");

    //17_b
    Effet* effet17_b = AjouterEffetNarration(
                "le Vampire est simplement écorché par le coup (enlevezlui "
                                       "3 points d'ENDURANCE) et il vous rejette en arrière vers la porte "
                                       "située à l'ouest.",
                "", "17_b");
    // je pars du principe que le combat est entamé (et les créatures associées créées)
    effet17_b->m_CallbackDisplay = [] {
        Combat::GetCombat()->GetEnnemiActuel()->m_Endurance -= 3;
    };
    AjouterChoixGoToEffet("Si vous fuyez par cette porte", "380");
    AjouterChoixGoToEffet("Si vous continuez à combattre", "144");

    //18
    AjouterEffetNarration(
                "Vous marchez vers l'ouest le long du couloir. Au bout d'une "
                "cinquantaine de mètres, le passage s'oriente vers le nord. Après avoir "
                "fait deux ou trois pas dans ce couloir, vous entendez un bruit "
                "d'éboulement sous vos pieds et vous essayez de sauter en arrière tandis "
                "que le sol se dérobe.",
                "", "18");
    TenterLaChanceGoTo("vous êtes malchanceux, vous n'avez pas été assez rapide et vous tombez "
                       "dans une fosse de deux mètres de profondeur. Vous perdez 1 point "
                       "d'ENDURANCE.", "348", "vous êtes chanceux, vous "
                       "avez réussi à faire un bond en arrière avant qu'un trou ne se forme.", "261",
                       [] {
        GestionnaireCarac::RetirerValeurACaracId(LDOELH::ENDURANCE, 1);
    });

    //19
    Effet* effet19 = AjouterEffetNarration(
                "Ces deux créatures malfaisantes sont des LUTINS. Ils vous attaquent "
                "un par un.",
                "", "19");
    Combat::GetCombat()->AjouterCombat(effet19, {new Creature( "Premier LUTIN", 5, 5), new Creature("Deuxième LUTIN", 5, 6)});
    effet19->m_GoToEffetId = "317";

    //19
    Effet* effet20 = AjouterEffetNarration(
                "La bagarre commence. Vous avez votre épée, ils ont leurs haches. Ils "
                "vous affrontent un par un.",
                "", "20");
    Combat::GetCombat()->AjouterCombatAvecFuite(
        effet20, {
        new Creature( "Premier NAIN", 7, 6),
        new Creature("Deuxième NAIN", 7, 7),
        new Creature("Troisième NAIN", 4, 6),
        new Creature("Quatrième NAIN", 5, 5)
        },
        "Si le combat tourne mal, vous pouvez fuir par la porte.- mais n'oubliez pas votre pénalité de fuite.",
        "371");
    effet20->m_GoToEffetId = "376";
}

void GenSorcMontagneFeu::GenererNumeros21_30()
{
    //21
    AjouterEffetNarration(
                "Le sang verdâtre des farfadets morts s'écoule de leurs corps en "
                "dégageant une odeur repoussante. Vous contournez les cadavres et vous "
                "examinez le coffre. Il est solide, fait de chêne et de fer, et bien fermé. ",
           "", "21");
    AjouterChoixGoToEffet("Vous pouvez essayer de briser la serrure à l'aide de votre épée", "339");
    AjouterChoixGoToEffet("ou le laisser et sortir par la porte ouverte", "293");
    //22
    Effet* effet22 = AjouterEffetNarration(
                "Vous furetez un peu partout à la recherche d'une trace de porte secrète, "
                "mais vous n'en trouvez pas. Tandis que vous faites une pause pour "
                "réfléchir à la situation, un mince jet de gaz s'échappe du plafond en "
                "sifflant.Vous toussez à vous en étouffer pour essayer de libérer vos "
                "poumons, mais rien à faire, vous tombez à genoux, la tête vous tourne "
                "et vous vous écroulez sur le sol en perdant connaissance. Lorsque vous "
                "revenez à vous, vous vous trouvez dans un endroit inconnu. ",
           "", "22");
    effet22->m_GoToEffetId = "4";

    //23
    AjouterEffetNarration(
                "Le couloir aboutit à une porte bien solide. Vous écoutez au panneau, "
                "mais vous n'entendez rien. ",
           "", "23");
    AjouterChoixGoToEffet("Allez-vous entrer dans la pièce", "326");
    AjouterChoixGoToEffet("ou retourner à la bifurcation", "229");

    //24
    Effet* effet24 = AjouterEffetNarration(
                "Après avoir subi votre troisième blessure, vous remarquez que votre "
                "force décline. Vous perdez 1 point d'HABILETÉ. Vous en déduisez "
                "qu'il s'agit, là encore, d'un pouvoir magique de la repoussante créature "
                "et vous vous sentez parcouru d'un frisson de panique. Allez-vous "
                "continuer ou prendre la fuite ? ",
           "", "24");
    // édition du combat actuel en fonction des nouvelles infos :
    effet24->m_CallbackDisplay = [] {
        GestionnaireCarac::RetirerValeurACaracId(LDOELH::HABILETE, 1);
        Combat* combatActuel = Combat::GetCombat();
        combatActuel->AjouterCaracAMonstre(SuceurHabilete);
        combatActuel->AjouterFuiteAuCombat("Si vous choisissez de vous enfuir, échappez vous par "
                                           "la porte située au nord", "360");
        // maj du go to final
        Univers::ME->GetExecHistoire()->GetExecEffetActuel()->GetEffet()->m_GoToEffetId = "135";
    };
    //25
    Effet* effet25 = AjouterEffetNarration(
                "Les peintures sont des portraits d'hommes. Un frisson vous parcourt "
                "l'échiné lorsque vous lisez sur une plaque le nom de celui qui se trouve "
                "sur le mur ouest - il s'agit de Zagor, le Sorcier dont vous cherchez le "
                "trésor. En regardant ce portrait, vous vous rendez compte que vous "
                "vous mesurez à un adversaire redoutable. Vous avez le sentiment d'être "
                "observé et vous remarquez les yeux perçants qui suivent chacun de vos "
                "mouvements. Vous vous sentez attiré vers ce portrait et votre peur "
                "s'accroît. Vous perdez un point d'HABILETÉ. Avez-vous le courage "
                "d'essayer de combattre le Sorcier ? ",
           "", "25");
    effet25->AjouterRetireurACarac(LDOELH::HABILETE, 1);
    Choix* fuite = AjouterChoixGoToEffet("Vous pouvez soit sortir tout droit "
                          "par la porte nord mais il s'agira alors d'une fuite", "90");
    fuite->AjouterRetireurACarac(LDOELH::ENDURANCE, 2);
    AjouterChoixGoToEffet("soit chercher dans votre sac à dos une arme que vous pourriez utiliser "
                          "pour combattre le pouvoir du Sorcier", "340");


    //26
    Effet* effet26 = AjouterEffetNarration(
                "Vous vous rappelez le petit livre à la reliure de cuir de Di Maggio, et "
                "vous prononcez silencieusement, du bout des lèvres, la formule "
                "magique enfermée dans ses pages. \n\n"
                "Vous poussez un grand cri en direction du Dragon, et il s'immobilise. Il "
                "penche la tête de côté et vous regarde d'un air soupçonneux. Vous lui "
                "jetez une pierre à la tête et elle rebondit sur son mufle. La bête laisse "
                "échapper un cri de colère et prend une profonde inspiration qui "
                "provoque un rugissement dans sa gorge. Le Dragon souffle et vous "
                "apercevez entre ses dents une autre boule de feu en train de se former. "
                "Vous vous tenez prêt, et lorsque la boule de feu jaillit de sa gueule, "
                "vous vous écriez : \n\n"
                "Ekil Erif Ekam Erif Erif Erif Di Maggio \n\n"
                "La boule de feu s'arrête aussitôt. Avec un cri de douleur, le Dragon "
                "essaye d'éloigner les flammes de son museau, mais elles continuent de "
                "le brûler. En poussant de terribles hurlements, le monstre fait volte-face "
                "et bondit dans les ténèbres, secouant sa tête en tous sens. ",
           "", "26");
    effet26->m_GoToEffetId = "371";

    //27
    Effet* effet27 = AjouterEffetNarration(
                "L'épée est une épée magique, et elle vous aidera à combattre. Aussi "
                "longtemps que vous vous servirez de cette épée, vous pourrez "
                "augmenter de 2 points votre total de départ en matière d'HABILETÉ. "
                "Vous pouvez également ajouter deux points à votre total actuel "
                "d'HABILETÉ. Vous gagnez également 2 points de CHANCE pour "
                "avoir trouvé cette épée.",
           "", "27");
    effet27->AjouterAjouteurACarac(LDOELH::HABILETE, 2);
    effet27->AjouterAjouteurACarac(LDOELH::CHANCE, 2);
    effet27->AjouterSetCaracTrue(GenSorcMontagneFeu::EPEE_MAGIQUE);
    AjouterChoixGoToEffet("Si vous jetez votre ancienne épée", "319");
    Choix* garderEpee = AjouterChoixGoToEffet("Si vous préférez conserver votre propre épée", "319");
    garderEpee->AjouterRetireurACarac(LDOELH::HABILETE, 2);
    garderEpee->AjouterChangeurDeCarac(GenSorcMontagneFeu::EPEE_MAGIQUE, "");

    //28
    Effet* effet28 = AjouterEffetNarration(
                "L'énorme Géant est étendu raide mort ! Vous fouillez sa caverne et n'y "
                "trouvez pas grand-chose d'utile en dehors d'une bourse qu'il porte à sa "
                "ceinture et qui » ontient 8 Pièces d'Or. Vous êtes un peu inquiet en ce "
                "qui concerne la deuxième chaise. A qui appartient-elle ? Vous décidez "
                "de quitter la caverne par le chemin que vous aviez pris. Mais avant cela, ajoutez 2 points de CHANCE et 2 autres "
                "d'HABILETÉ pour votre victoire.",
           "", "28");
    effet28->AjouterAjouteurACarac(LDOELH::HABILETE, 2);
    effet28->AjouterAjouteurACarac(LDOELH::CHANCE, 2);
    effet28->m_GoToEffetId = "351";

    //29
    Effet* effet29 = AjouterEffetNarration(
                "A part les bottes auxquelles vous décidez de n'accorder aucune "
                "attention, il y a peu de choses dans la caverne. Vous choisissez de "
                "rebrousser chemin dans la direction d'où vous êtes venu.",
           "", "29");
    effet29->m_GoToEffetId = "375";

    //30
    Effet* effet30 = AjouterEffetNarration(
                "Une pierre se détache du roc et révèle une anfractuo-sité dans laquelle "
                "est cachée une corde.",
           "", "30");
    AjouterChoixGoToEffet("Si vous voulez tirer sur la corde", "67");
    AjouterChoixGoToEffet("Si vous pensez qu'il est plus prudent de n'y point toucher, vous "
                          "pouvez revenir au croisement", "267");
}

void GenSorcMontagneFeu::GenererNumeros31_40()
{
    //31
    Effet* effet31 = AjouterEffetNarration(
                "Si vous êtes en possession de la pierre précieuse arrachée à l'oeil du "
                "Cyclope, vous la tenez devant le Sorcier. Son regard menaçant se "
                "transforme alors en une expression de douleur. De toute évidence, il "
                "ressent le pouvoir de la pierre. Soudain, ses yeux deviennent blancs et "
                "son visage flasque. Votre confiance en vous-même se renforce lorsque "
                "vous réalisez que vous venez de gagner votre première vraie bataille. "
                "Prenez 2 points d'HABILETÉ. Rangez la pierre précieuse dans votre "
                "sac à dos et sortez par la porte nord.",
           "", "31");
    effet31->AjouterAjouteurACarac(LDOELH::HABILETE, 2);
    effet31->m_GoToEffetId = "90";

    //32
    Effet* effet32 = AjouterEffetNarration(
                "Vous lancez le Fromage à travers la pièce en direction des Rats qui se "
                "battent pour le dévorer, en échangeant des coups de pattes et de dents. "
                "Ayant ainsi détourné leur attention, il ne vous reste plus qu'à traverser "
                "la pièce et à sortir par la porte qui s'ouvre dans le mur nord. Prenez 2 points de CHANCE pour prix de votre bonne "
                "fortune.",
           "", "32");
    effet31->AjouterAjouteurACarac(LDOELH::CHANCE, 2);
    effet31->m_GoToEffetId = "124";

    //33
    Effet* effet33 = AjouterEffetNarration(
                "La créature endormie se réveille en sursaut. Elle bondit et se rue sur "
                "vous, sans arme. Vous devriez pouvoir la vaincre avec votre épée, mais "
                "ses dents acérées semblent plutôt dangereuses."
                "FARFADET HABILETÉ: 6 ENDURANCE: 4 "
                "Si vous êtes vainqueur, vous pouvez prendre la boîte.",
           "", "33");
    Combat::GetCombat()->AjouterCombatAvecFuite(
                effet33,
                {new Creature("FARFADET", 6, 4)},
                "Vous avez le droit de prendre fuite par la porte", "320");
    effet33->m_GoToEffetId = "147";

    // GESTION EQUIPEMENT !!! et attention à l'épée magique plus haut...
}

QString GenSorcMontagneFeu::EPEE_MAGIQUE = "Épée magique";

int GenSorcMontagneFeu::Num161_COUNTER = 0;
Effet* GenSorcMontagneFeu::GenererNumeros161()
{
    QString id = "161_" + QString::number(GenSorcMontagneFeu::Num161_COUNTER++);
    Effet* effet161 = AjouterEffetNarration(
                "En cherchant des portes et des passages secrets, vous avez sondé, martelé, "
                "raclé la paroi rocheuse, et tous ces bruits ont résonné dans les couloirs "
                "de la montagne. Diverses créatures vagabondent librement dans les "
                "souterrains, et ce vacarme a attiré l'attention de l'un des monstres "
                "suivants.", "", id);

    QString nomCreature = "??";
    int habileteCreature = -1;
    int enduranceCreature = -1;

    switch (Aleatoire::GetAl()->D6()) {
    case 1 :
        nomCreature = "LUTIN";
        habileteCreature = 5;
        enduranceCreature = 3;
        break;
    case 2 :
        nomCreature = "FARFADET";
        habileteCreature = 6;
        enduranceCreature = 3;
        break;
    case 3 :
        nomCreature = "DIABLOTIN";
        habileteCreature = 6;
        enduranceCreature = 4;
        break;
    case 4 :
        nomCreature = "RAG GÉANT";
        habileteCreature = 5;
        enduranceCreature = 4;
        break;
    case 5 :
        nomCreature = "SQUELETTE";
        habileteCreature = 6;
        enduranceCreature = 5;
        break;
    case 6 :
        nomCreature = "TROLL";
        habileteCreature = 8;
        enduranceCreature = 4;
        break;
    }

    Combat::GetCombat()->AjouterCombat(effet161, {
          new Creature(nomCreature, habileteCreature, enduranceCreature)});

    return effet161;
}

void GenSorcMontagneFeu::GenererEvtsAccueil()
{
    this->AjouterEvt("Debut", "Génération du perso par les choix");
    //AjouterEffetNarration("", ":/images/couverture.PNG", "couverture");

    //GenererCombatBidon();
}
