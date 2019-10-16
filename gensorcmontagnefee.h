#ifndef GENSORCMONTAGNEFEE_H
#define GENSORCMONTAGNEFEE_H

#include "../destinLib/genhistoire.h"

class LDOELH;
struct Creature;

using namespace std;

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
    void GenererNumeros21_30();

    // numéro spécial qui peut être appelé de plusieurs endroits différents et a un effet généré pour chacun de ces cas
    // c'est donc un numéro qui existe en plusieurs exemplaires
    Effet* GenererNumeros161();
    static int Num161_COUNTER;
    void GenererEffetsGeneriques();

    // fonction gameplay générales (défis fantastiques en général ??) => faire une classe mère GenDefisFantastiques ?
    void TenterLaChanceGoTo(QString texteMalchanceux, QString effet_malchanceux_id,
                       QString texteChanceux, QString effet_chanceux_id,
                       function<void()> malchanceuxCallback = nullptr, function<void()> chanceuxCallback = nullptr);

    LancerDe* AjouterCombat(Effet* effet, QVector<Creature> creatures);
    void AjouterCombatAvecFuite(Effet* effet, QVector<Creature> creatures, QString texteFuite, QString idFuite);

};

#endif // GENSORCMONTAGNEFEE_H
