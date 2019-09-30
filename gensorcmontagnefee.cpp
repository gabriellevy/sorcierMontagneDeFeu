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
    GenererNumeros1_10();
    GenererNumeros11_20();
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

void GenSorcMontagneFeu::AjouterCombatAvecFuite(
        Effet* effet, QString nomMonstre, int habileteMonstre,
        int enduranceDepartMonstre, QString texteFuite, QString idFuite)
{
    LancerDe* combat = AjouterCombat(effet, nomMonstre, habileteMonstre, enduranceDepartMonstre);
    m_GenerateurEvt->AjouterChoixGoToEffet(texteFuite, idFuite, "", combat);
}

LancerDe* GenSorcMontagneFeu::AjouterCombat(Effet* effet, QString nomMonstre, int habileteMonstre, int enduranceDepartMonstre)
{
    QString texteCombatBase = nomMonstre + " - HABILITÉ : " + QString::number(habileteMonstre) +
                                         " ENDURANCE : ";
    effet->m_Texte += "\n\n" + texteCombatBase + QString::number(enduranceDepartMonstre);

    std::function<ResExecutionLancerDe*(int)> lancerHabilite = [texteCombatBase, nomMonstre, habileteMonstre, enduranceDepartMonstre](int i) {
        QString resTxt = "";
        QString phaseActuelle = GestionnaireCarac::GetCaracValue(LDOELH::PHASE_COMBAT);
        QString phaseJoueur = "phaseJoueur";
        QString phaseEnnemi = "phaseEnnemi";
        bool combatContinue = true;
        int enduranceActuelleMonstre = GestionnaireCarac::GetCaracValueAsInt(LDOELH::ENDURANCE_ENNEMI);
        if ( enduranceActuelleMonstre <= 0) {
            enduranceActuelleMonstre = enduranceDepartMonstre;
            GestionnaireCarac::SetValeurACaracId(LDOELH::ENDURANCE_ENNEMI, enduranceActuelleMonstre);
        }

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

    return m_GenerateurEvt->AjouterLancerDe("Combattre", 2, lancerHabilite);
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
    AjouterCombatAvecFuite(effet8, "BARBARE", 7, 6,
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
    Effet* effet10 = AjouterEffetNarration("Vous êtes revenu à la bifurcation et vous prenez la direction du nord. ",
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
    Effet* effet12 = AjouterEffetNarration(
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
    Effet* effet14 = AjouterEffetNarration(
                "Il n'y a pas trace de passage secret ; en revanche, vous entendez des "
                "bruits de pas qui viennent vers vous.",
           "", "14");
    Effet* effet161_14 = GenererNumeros161();
    effet161_14->m_GoToEffetId = "117";

    //15
    /*Effet* effet15 = AjouterEffetNarration(
                "Il n'y a pas trace de passage secret ; en revanche, vous entendez des "
                "bruits de pas qui viennent vers vous.",
           "", "15");
    Effet* effet161_14 = GenererNumeros161();
    effet161_14->m_GoToEffetId = "117";*/
    // !!!! créer une fonction de gain d'endurance qui surveille qu'on ne dépasse pas le niveau de départ

}

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

    AjouterCombat(effet161, nomCreature, habileteCreature,enduranceCreature);

    return effet161;
}

void GenSorcMontagneFeu::GenererEvtsAccueil()
{
    this->AjouterEvt("Debut", "Génération du perso par les choix");
    //AjouterEffetNarration("", ":/images/couverture.PNG", "couverture");

    //GenererCombatBidon();
}
