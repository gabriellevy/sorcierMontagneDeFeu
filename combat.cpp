#include "combat.h"
#include "../destinLib/gestionnairecarac.h"
#include "gensorcmontagnefee.h"
#include "ldoelh.h"
#include "../destinLib/choix.h"
#include "../destinLib/execeffet.h"

QString Combat::PHASE_COMBAT = "PhaseCombat";
QString Combat::RES_ATTAQUE_JOUEUR = "ResAttaqueJoueur";
QString Combat::RES_ATTAQUE_ENNEMI = "ResAttaqueEnnemi";
QString Combat::ENDURANCE_ENNEMI = "EnduranceEnnemi";
QString Combat::NUM_DE_COMBAT = "NumDeCombat";

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
    m_PhaseCombat = PhaseCombat::AttaqueJoueur;
    m_NbBlessuresRecues = 0;
}

Creature* Combat::GetEnnemiActuel()
{
    return m_Ennemis[m_NumDeCombat];
}

LancerDe* Combat::AjouterCombat(Effet* effet, QVector<Creature*> creatures)
{
    QString texteToutCombat = "";
    QVector<QString> textesChaqueCombat = {};
    GenSorcMontagneFeu* genSorcMontagneFeu = GenSorcMontagneFeu::GetGenSorcMontagneFeu();

    for (int i = 0 ; i < creatures.length(); ++i) {
        QString texteCombCreat = creatures[i]->m_Nom + " - HABILITÉ : " +
                QString::number(creatures[i]->m_Habilete) +
                " ENDURANCE : ";
        textesChaqueCombat.push_back(texteCombCreat);
        texteToutCombat += ( texteCombCreat + QString::number(creatures[i]->m_Endurance) + "\n\n");
    }

    effet->m_Texte += "\n\n" + texteToutCombat;

    std::function<ResExecutionLancerDe*(int)> lancerHabilite = [textesChaqueCombat, creatures](int i) {
        Combat* combatActuel = Combat::GetCombat();
        if ( combatActuel->m_Ennemis.length() == 0)
            combatActuel->CommencerCombat(creatures);

        QString resTxt = "";
        QString phaseJoueur = "phaseJoueur";
        QString phaseEnnemi = "phaseEnnemi";
        bool combatContinue = true;
        Creature* creature = combatActuel->GetEnnemiActuel();

        std::function<QString()> getIntituleCombat = [textesChaqueCombat, combatActuel]() {
            return "\n\n" + textesChaqueCombat[combatActuel->m_NumDeCombat] + QString::number(combatActuel->GetEnnemiActuel()->m_Endurance);
        };

        if ( combatActuel->m_PhaseCombat == PhaseCombat::AttaqueJoueur) {
            // le joueur attaque (lance les dés)
            int resTotal = GestionnaireCarac::GetCaracValueAsInt(LDOELH::HABILETE) + i;
            resTxt  = "Votre force d'attaque : "  + QString::number(resTotal);
            combatActuel->m_ResAttaqueJoueur = resTotal;

            // au tour du monstre :
            combatActuel->m_PhaseCombat = PhaseCombat::AttaqueEnnemi;
        } else {
            // le monstre attaque et on résoud le duel
            int resEnnemi = combatActuel->GetEnnemiActuel()->m_Habilete + i;
            resTxt  = combatActuel->GetEnnemiActuel()->m_Nom + ", force d'attaque  : "  + QString::number(resEnnemi);
            combatActuel->m_ResAttaqueEnnemi = resEnnemi;

            // conclusion de la phase de combat :
            if (combatActuel->m_ResAttaqueJoueur > combatActuel->m_ResAttaqueEnnemi) {
                // le joueur a l'avantage :
                creature->m_Endurance -= 2;
                if ( creature->m_Endurance <= 0) {
                    // l'ennemi est mort :
                    resTxt += "\nL'ennemi est mort. Victoire ! ";

                    // est-ce qu'il y a d'autres monstres à tuer ?
                    combatActuel->m_NumDeCombat++;
                    if ( combatActuel->m_NumDeCombat < creatures.length()) {
                        // ennemi suivant
                        combatContinue = true;
                        resTxt += "Mais il reste encore des ennemis à combatte.";

                    } else {
                        combatContinue = false;
                        combatActuel->FinirCombat();
                    }
                } else {
                    // l'ennemi est blessé :
                    resTxt += "\nL'ennemi est blessé.";
                    resTxt += getIntituleCombat();
                }
            }
            else if (combatActuel->m_ResAttaqueJoueur < resEnnemi)
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
            combatActuel->m_PhaseCombat = PhaseCombat::AttaqueJoueur;
            IPerso::GetPersoInterface()->RafraichirAffichage();
        }
        return new ResExecutionLancerDe(resTxt, combatContinue);
    };

    return genSorcMontagneFeu->m_GenerateurEvt->AjouterLancerDe("Combattre", 2, lancerHabilite);
}
