#ifndef GENSORCMONTAGNEFEE_H
#define GENSORCMONTAGNEFEE_H

#include "../destinLib/genhistoire.h"

class LDOELH;

class GenSorcMontagneFeu : public GenHistoire
{
public:
    GenSorcMontagneFeu();

    virtual Hist* GenererHistoire();

protected:
    // génère toutes les caracs qui peuvent être visualisées par le joueur (d'autres caracs peuvent être générées et invisibles n'importe quand dans l'aventure)
    virtual void GenererCaracs();
    virtual void GenererPersos();

private:
    // génération des événement et effets de base
    void GenererEvtsAccueil();
    void GenererNumeros1_10();
    void GenererNumeros11_20();
    void GenererEffetsGeneriques();

    // fonction gameplay générales (défis fantastiques en général ??) => faire une classe mère GenDefisFantastiques ?
    void TenterLaChanceGoTo(QString texteMalchanceux, QString effet_malchanceux_id,
                       QString texteChanceux, QString effet_chanceux_id);

    LancerDe* AjouterCombat(Effet* effet, QString nomMonstre, int habileteMonstre, int enduranceMonstre);
    void AjouterCombatAvecFuite(Effet* effet, QString nomMonstre, int habileteMonstre, int enduranceMonstre, QString texteFuite, QString idFuite);

};

#endif // GENSORCMONTAGNEFEE_H
