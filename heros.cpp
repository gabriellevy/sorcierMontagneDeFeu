#include "heros.h"
#include "equipement.h"

using std::shared_ptr;

Heros::Heros():DPerso("Reynault", "Reynault", "Reynault", "")
{}


QString Heros::GetValeurCarac(QString id)
{
    return DPerso::GetValeurCarac(id);
}

int Heros::GetValeurCaracAsInt(QString id)
{
    int val = DPerso::GetValeurCaracAsInt(id);

    // l'épée magique ajoute 2 à l'habileté :
    if ( Equipement::HerosACetEquipement(Equipement::EPEE_MAGIQUE))
        val += 2;

    return val;
}

Heros* Heros::GetHerosJoue()
{
    return static_cast<Heros*>(DPerso::GetDPersoJoue().get());
}
