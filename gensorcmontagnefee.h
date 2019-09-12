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
    // génération de l'événement de base
    void GenererEvtsAccueil();

    void GenererCombatBidon();

};

#endif // GENSORCMONTAGNEFEE_H
