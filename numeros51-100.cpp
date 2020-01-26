#include "gensorcmontagnefee.h"
#include "ldoelh.h"
#include "combat.h"
#include "equipement.h"
#include "../destinLib/gestionnairecarac.h"
#include "../destinLib/exec/execeffet.h"
#include "../destinLib/abs/lancerde.h"
#include "heros.h"

void GenSorcMontagneFeu::GenererNumeros51_60()
{
    // 51
    Effet* effet51 = AjouterEffetNarration(
                "Vous buvez la Potion et vous voyez alors une expression d'étonnement "
                "naître sur le visage du Troll. Il s'avance vers vous, il essaie de vous "
                "toucher, mais vous faites un pas de côté et il ne rencontre que le vide. Il "
                "gesticule en tous sens pour tenter de vous saisir, mais vous parvenez "
                "facilement à l'esquiver. Il finit par abandonner et retourne dans sa "
                "chambre juste à temps, au moment où vous sentez que vous réapparaissez. "
                "Prenez 2 points de CHANCE. Vous pouvez continuer votre "
                "chemin en direction du nord..",
                "", "51");
    effet51->AjouterAjouteurACarac(LDOELH::CHANCE, 2);
    effet51->m_GoToEffetId = "287";

    // 52
    AjouterEffetNarration(
                "Vous vous trouvez à un croisement à angle droit en forme de T. Un "
                "passage vers l'est part d'un corridor nord-sud.",
           "", "52");
    AjouterChoixGoToEffet("Si vous allez au sud", "391");
    AjouterChoixGoToEffet("Si vous voulez essayer de découvrir des passages secrets dans le couloir qui mène vers le sud", "362");
    AjouterChoixGoToEffet("Si vous allez au nord", "354");
    AjouterChoixGoToEffet("Si vous voulez découvrir des passages secrets dans le couloir qui mène vers le nord", "234");
    AjouterChoixGoToEffet("Si vous prenez la direction de l'est", "291");
}
