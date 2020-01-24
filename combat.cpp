#include "combat.h"
#include "../destinLib/gestionnairecarac.h"
#include "gensorcmontagnefee.h"
#include "ldoelh.h"
#include "../destinLib/choix.h"
#include "../destinLib/execeffet.h"
#include "../destinLib/execlancerde.h"
#include "../destinLib/execchoix.h"
#include "../destinLib/aleatoire.h"
#include "heros.h"

Combat::Combat()
{
    ME = this;
}

Combat* Combat::ME = nullptr;
Combat* Combat::GetCombat()
{
    return ME;
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

void Combat::AjouterCombatAvecFuite(
        Effet* effet, QVector<Creature*> creatures, QString texteFuite, QString idFuite)
{
    LancerDe* combat = AjouterCombat(effet, creatures);
    GenSorcMontagneFeu* genSorcMontagneFeu = GenSorcMontagneFeu::GetGenSorcMontagneFeu();
    // fuir implique qu'on perde 2 points d'endurance
    Choix* choixFuite = genSorcMontagneFeu->m_GenerateurEvt->AjouterChoixGoToEffet(texteFuite, idFuite, "", combat);
    choixFuite->AjouterRetireurACarac(LDOELH::ENDURANCE, "2");
}

void Combat::AjouterFuiteAuCombat(QString texteFuite, QString idFuite)
{
    ExecChoix* execChoix = Univers::ME->GetExecHistoire()->GetExecLancerDeActuel()->AjoutChoixGoToEffet(texteFuite, idFuite);
    execChoix->m_Choix->AjouterRetireurACarac(LDOELH::ENDURANCE, "2");
}

void Combat::CommencerCombat(QVector<Creature*> creatures)
{
    m_Ennemis = creatures;
    m_NumDeCombat = 0;
    m_ResAttaqueJoueur = -1;
    m_ResAttaqueEnnemi = -1;
    m_PhaseCombat = PhaseCombat::AttaqueJoueur;
    m_NbBlessuresRecues = 0;
}

void Combat::FinirCombat()
{
    m_Ennemis.clear();
    m_NumDeCombat = 0;
    m_ResAttaqueJoueur = -1;
    m_ResAttaqueEnnemi = -1;
    m_PhaseCombat = PhaseCombat::AucunCombatEnCours;
    m_NbBlessuresRecues = 0;
}

Creature* Combat::GetEnnemiActuel()
{
    return m_Ennemis[m_NumDeCombat];
}

QString Combat::GetIntituleCombat(int indexCreature)
{
    if ( indexCreature == -1 )
        indexCreature = m_NumDeCombat;
    QString msg = "indexCreature trop grand pour ce combat : " + QString::number(indexCreature);
    Q_ASSERT_X(indexCreature < m_Ennemis.length(), msg.toStdString().c_str(), "Combat::GetIntituleCombat");

    Creature* creature = m_Ennemis[indexCreature];
    QString intitutle = creature->m_Nom + " - HABILITÉ : " +
            QString::number(creature->m_Habilete) +
            " ENDURANCE : " + QString::number(creature->m_Endurance);

    return intitutle;
}


void Combat::AjouterCaracAMonstre(CapaciteCreature capaciteCreature)
{
    this->GetEnnemiActuel()->m_CapacitesCreature.push_back(capaciteCreature);
}

bool Combat::TourDeCombat(int resDes, QString &resTxt)
{
    bool combatContinue = true;
    Creature* creature = GetEnnemiActuel();
    Heros* heros = Heros::GetHerosJoue();

    if ( m_PhaseCombat == PhaseCombat::AttaqueJoueur) {
        // le joueur attaque (lance les dés)
        m_ResAttaqueJoueur = IPerso::GetPersoCourant()->GetValeurCaracAsInt(LDOELH::HABILETE) + resDes;

        if ( creature->ACetteCapacite(Aveugle) )
            m_ResAttaqueJoueur += 2;
        resTxt  = "Votre force d'attaque : "  + QString::number(m_ResAttaqueJoueur);

        // au tour du monstre :
        m_PhaseCombat = PhaseCombat::AttaqueEnnemi;
        Univers::ME->GetExecHistoire()->GetExecLancerDeActuel()->ChangerIntituleBouton("Au tour de la créature");

    } else {
        // le monstre attaque et on résoud le duel
        int resEnnemi = GetEnnemiActuel()->m_Habilete + resDes;
        resTxt  = GetEnnemiActuel()->m_Nom + ", force d'attaque  : "  + QString::number(resEnnemi);
        m_ResAttaqueEnnemi = resEnnemi;

        // conclusion de la phase de combat :
        if (m_ResAttaqueJoueur > m_ResAttaqueEnnemi ) {
            // le joueur a l'avantage :
            creature->m_Endurance -= 2;

            if ( creature->ACetteCapacite(Aveugle) )
                creature->m_Endurance -= 1;// blessure supplémentaire

            if (  creature->ACetteCapacite(GoToAuPremierSang)) {
                resTxt += "\nL'ennemi est blessé.";
                combatContinue = false;
                Univers::ME->GetExecHistoire()->GetExecLancerDeActuel()->ChangerIntituleBouton("Cliquez pour voir ce qui se passe");
                FinirCombat();
            }
            else if ( creature->m_Endurance <= 0) {
                // l'ennemi est mort :
                resTxt += "\nL'ennemi est mort. Victoire ! ";

                // est-ce qu'il y a d'autres monstres à tuer ?
                m_NumDeCombat++;
                if ( m_NumDeCombat < m_Ennemis.length()) {
                    // ennemi suivant
                    combatContinue = true;
                    resTxt += "Mais il reste encore des ennemis à combatte.";

                } else {
                    combatContinue = false;
                    FinirCombat();
                }
            } else {
                // l'ennemi est blessé :
                resTxt += "\nL'ennemi est blessé.";
                resTxt += "\n\n" + GetIntituleCombat();
            }
        }
        else if (m_ResAttaqueJoueur < resEnnemi)
        {
            // le monstre a l'avantage :
            int endurancePerdue = 2;

            // possibilité de réduire les blessure :
            if ( creature->ACetteCapacite(Aveugle) ) {
                int resDe = Aleatoire::GetAl()->D6();
                switch (resDe) {
                case 2 :case 4 : endurancePerdue = 1; break;
                case 6 : endurancePerdue = 0; break;
                }
            }

            if ( endurancePerdue == 0 )
            {
                resTxt += "\nVous avez réussi à esquiver l'attaque.";
            }
            else
            {
                int endurance = heros->RetirerValeurACaracId(LDOELH::ENDURANCE, endurancePerdue);
                m_NbBlessuresRecues++;
                if ( m_NbBlessuresRecues%3 == 0 && creature->ACetteCapacite(SuceurHabilete)) {
                    heros->RetirerValeurACaracId(LDOELH::HABILETE, 1);
                }
                if ( endurance <= 0) {
                    // perdu... :
                    resTxt += "\nVous êtes mort.";
                    combatContinue = false;
                    Univers::ME->GetExecHistoire()->GetExecEffetActuel()->GetEffet()->m_GoToEffetId = "mort";
                } else {
                    // joueur blessé :
                    resTxt += "\nVous êtes blessé.";
                    resTxt += "\n\n" + GetIntituleCombat();
                }
            }
        } else {
            // égalité
            resTxt += "\nÉgalité.";
            resTxt += "\n\n" + GetIntituleCombat();
        }

        // au tour du joueur
        m_PhaseCombat = PhaseCombat::AttaqueJoueur;
        IPerso::GetPersoInterface()->RafraichirAffichage();
        Univers::ME->GetExecHistoire()->GetExecLancerDeActuel()->ChangerIntituleBouton("Attaquez");
    }

    return combatContinue;
}

LancerDe* Combat::AjouterCombat(Effet* effet, QVector<Creature*> creatures)
{
    QString texteToutCombat = "";
    GenSorcMontagneFeu* genSorcMontagneFeu = GenSorcMontagneFeu::GetGenSorcMontagneFeu();

    m_Ennemis = creatures;
    for (int i = 0 ; i < m_Ennemis.length(); ++i) {
        texteToutCombat += (GetIntituleCombat(i) + "\n");
    }

    effet->m_Texte += "\n\n" + texteToutCombat;

    std::function<ResExecutionLancerDe*(int)> lancerHabilite = [creatures](int resDes) {
        Combat* combatActuel = Combat::GetCombat();
        if ( combatActuel->m_PhaseCombat == AucunCombatEnCours)
            combatActuel->CommencerCombat(creatures);

        QString resTxt = "";
        bool combatContinue = combatActuel->TourDeCombat(resDes, resTxt);

        return new ResExecutionLancerDe(resTxt, combatContinue);
    };

    return genSorcMontagneFeu->m_GenerateurEvt->AjouterLancerDe("Combattre", 2, lancerHabilite);
}
