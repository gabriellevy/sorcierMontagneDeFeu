#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H

#include <QString>
#include <QList>
#include <memory>

/**
 * @brief classe de gestion de toutes les caracs qui sont aussi de l'équipement
 */
class Equipement
{
public:
    Equipement(QString id);
    QString m_Id;

    // affecte l'équipement dé départ au personnage et l'affiche
    static void GetEquipementDepart();
    static QList<QString> GetEquipementsHeros();
    static bool HerosACetEquipement(QString equipementId);


    // tous les types d'équipement existant :
    static QString MAILLET_METAL;
    static QString CISEAU_LAME_ARGENT;
    static QString EPEE;
    static QString ARMURE_CUIR;
    static QString LANTERNE;
    static QString POTION_ADRESSE;
    static QString POTION_VIGUEUR;
    static QString POTION_BONNE_FORTUNE;
    static QString EPEE_MAGIQUE;// + 2 en habileté
    static QString CLEF_BRONZE9;// clef de bronze gravée du chiffre 9

    // liste de tous els équipements disponibles dans le jeu et de leur description
    // (permet de déterminer ce qui est un équipement dans la liste des caracs)
    static QMap<QString, QString> EQUIPEMENTS;
};

#endif // EQUIPEMENT_H
