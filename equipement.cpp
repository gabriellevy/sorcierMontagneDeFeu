#include "equipement.h"
#include "../destinLib/gestionnairecarac.h"
#include "../destinLib/carac.h"

Equipement::Equipement(QString id):m_Id(id)
{

}

QString Equipement::MAILLET_METAL = "Maillet en métal";
QString Equipement::CISEAU_LAME_ARGENT = "Ciseau à lame d'argent";
QString Equipement::EPEE = "Épée";
QString Equipement::ARMURE_CUIR = "Armure de cuir";
QString Equipement::LANTERNE = "Lanterne";
QString Equipement::POTION_ADRESSE = "Potion d'adresse";
QString Equipement::POTION_VIGUEUR = "Potion de vigueur";
QString Equipement::POTION_BONNE_FORTUNE = "Potion de bonne fortune";
QString Equipement::EPEE_MAGIQUE = "Épée magique";

QMap<QString, QString> Equipement::EQUIPEMENTS = {
    { Equipement::MAILLET_METAL, Equipement::MAILLET_METAL },
    { Equipement::CISEAU_LAME_ARGENT, Equipement::CISEAU_LAME_ARGENT },
    { Equipement::EPEE, Equipement::EPEE },
    { Equipement::ARMURE_CUIR, Equipement::ARMURE_CUIR },
    { Equipement::LANTERNE, Equipement::LANTERNE },
    { Equipement::POTION_ADRESSE, Equipement::POTION_ADRESSE },
    { Equipement::POTION_VIGUEUR, Equipement::POTION_VIGUEUR },
    { Equipement::EPEE_MAGIQUE, Equipement::EPEE_MAGIQUE + "\nHabileté +2" },
    { Equipement::POTION_BONNE_FORTUNE, Equipement::POTION_BONNE_FORTUNE }
};

void Equipement::GetEquipementDepart()
{
    QMap<QString, QString>::iterator i;
    for (i = Equipement::EQUIPEMENTS.begin(); i != Equipement::EQUIPEMENTS.end(); ++i) {

        GestionnaireCarac::GetGestionnaireCarac()->AjouterCarac(
                    new Carac(
                        i.key(),
                        i.key(),
                        "",
                        //":/images/equipement/Épée.jpg",
                        ":/images/equipement/" + i.key(),
                        i.value(),
                        MODE_AFFICHAGE::ma_Img
                        )
                    );

    }

    GestionnaireCarac::SetValeurACaracId(Equipement::EPEE, "1");
    GestionnaireCarac::SetValeurACaracId(Equipement::ARMURE_CUIR, "1");
    GestionnaireCarac::SetValeurACaracId(Equipement::LANTERNE, "1");
    // note : en théorie on choisit parmi les 3 types de potion !
    GestionnaireCarac::SetValeurACaracId(Equipement::POTION_VIGUEUR, "1");
}


QList<QString> Equipement::GetEquipementsHeros()
{
    QList<QString> equiptActuelHeros = {};
    QMap<QString, QString>::iterator it_eqpts = Equipement::EQUIPEMENTS.begin();
    QMap<QString, QString>::iterator it_eqpts_end = Equipement::EQUIPEMENTS.end();

    for ( ;it_eqpts != it_eqpts_end;++it_eqpts )
    {
        if ( GestionnaireCarac::IsCaracTrue(it_eqpts.key()) )
            equiptActuelHeros.push_back(it_eqpts.key());
    }

    return equiptActuelHeros;
}

bool Equipement::HerosACetEquipement(QString equipementId)
{
    QList<QString> eqptsHeros = GetEquipementsHeros();
    return (eqptsHeros.indexOf(equipementId) != -1);
}
