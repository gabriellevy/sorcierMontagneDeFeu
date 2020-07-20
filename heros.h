#ifndef HEROS_H
#define HEROS_H
#include "../destinLib/gestcarac.h"

class Heros : public DPerso
{
public:
    Heros();

    virtual QString GetValeurCarac(QString id);
    virtual int GetValeurCaracAsInt(QString id);

    static Heros* GetHerosJoue();
};

#endif // HEROS_H
