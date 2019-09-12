#include "ldoelh.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LDOELH w;
    w.LancerHistoire(LDOELH::SORCIER_MONTAGNE_FEU);
    w.show();

    return a.exec();
}
