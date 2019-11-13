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
#include "equipement.h"
#include "heros.h"

GenSorcMontagneFeu::GenSorcMontagneFeu():GenHistoire ("Le sorcier de la montagne de feu") {}

GenSorcMontagneFeu* GenSorcMontagneFeu::GetGenSorcMontagneFeu()
{
    return static_cast<GenSorcMontagneFeu*>(ME);
}

void GenSorcMontagneFeu::GenererEffetsGeneriques()
{
    AjouterEffetNarration("Vous êtes mort", "", "mort");
}

Hist* GenSorcMontagneFeu::GenererHistoire()
{
    GenererPersos();

    //GenererFonctionsCallback();

    GenererEvtsAccueil();
    GenererNumeros1_10();
    GenererNumeros11_20();
    GenererNumeros21_30();
    GenererNumeros31_40();
    GenererNumeros41_50();
    GenererEffetsGeneriques();

    FinGenerationHistoire();

    GenererCaracs();

    return m_HistoireGeneree;
}

void GenSorcMontagneFeu::GenererPersos()
{
    DPerso* perso = new Heros();
    perso->InitialiserPerso();
    IPerso::AjouterPersoJouable(perso);
}

void GenSorcMontagneFeu::GenererCaracs()
{
    // note : ce serait mieux de pouvoir tirer au dé pour déterminer ces valeurs
    int habilete = 6 + Aleatoire::GetAl()->D6();
    GestionnaireCarac::GetGestionnaireCarac()->AjouterCaracNombre(LDOELH::HABILETE, habilete, 0, habilete);

    int endurance = 12 + Aleatoire::GetAl()->D6() + Aleatoire::GetAl()->D6();
    GestionnaireCarac::GetGestionnaireCarac()->AjouterCaracNombre(LDOELH::ENDURANCE, endurance, 0, endurance);

    int chance = 6 + Aleatoire::GetAl()->D6();
    GestionnaireCarac::GetGestionnaireCarac()->AjouterCaracNombre(LDOELH::CHANCE, chance, 0, chance);

    GestionnaireCarac::GetGestionnaireCarac()->AjouterCaracNombre(LDOELH::REPAS, 10, 0, 10);

    Equipement::GetEquipementDepart();
}


bool TenterLaChance(int resDe)
{
    QString resTxt = "Résultat des dés : " + QString::number(resDe) + "\n\n";
    qDebug() << resTxt;

    GestionnaireCarac::AJouterValeurACaracId(LDOELH::CHANCE, -1);
    IPerso::GetPersoInterface()->RafraichirAffichage();

    return ( resDe < IPerso::GetPersoCourant()->GetValeurCaracAsInt(LDOELH::CHANCE));
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
                                           new Creature(nomCreature, habileteCreature, enduranceCreature, {Aveugle})});

    return effet161;
}

void GenSorcMontagneFeu::GenererEvtsAccueil()
{
    this->AjouterEvt("Debut", "Génération du perso par les choix");
    //AjouterEffetNarration("", ":/images/couverture.PNG", "couverture");

    //GenererCombatBidon();
}
