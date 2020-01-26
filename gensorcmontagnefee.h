#ifndef GENSORCMONTAGNEFEE_H
#define GENSORCMONTAGNEFEE_H

#include "../destinLib/gen/genhistoire.h"

class LDOELH;
class Combat;

using namespace std;

class GenSorcMontagneFeu : public GenHistoire
{
public:
    GenSorcMontagneFeu();

    virtual Hist* GenererHistoire();

    static GenSorcMontagneFeu* GetGenSorcMontagneFeu();

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
    void GenererNumeros31_40();
    void GenererNumeros41_50();
    void GenererNumeros51_60();

    // numéro spécial qui peut être appelé de plusieurs endroits différents et a un effet généré pour chacun de ces cas
    // c'est donc un numéro qui existe en plusieurs exemplaires
    shared_ptr<Effet> GenererNumeros161();
    static int Num161_COUNTER;
    void GenererEffetsGeneriques();

    // fonction gameplay générales (défis fantastiques en général ??) => faire une classe mère GenDefisFantastiques ?
    void TenterLaChanceGoTo(QString texteMalchanceux, QString effet_malchanceux_id,
                       QString texteChanceux, QString effet_chanceux_id,
                       function<void()> malchanceuxCallback = nullptr, function<void()> chanceuxCallback = nullptr);

};

#endif // GENSORCMONTAGNEFEE_H
