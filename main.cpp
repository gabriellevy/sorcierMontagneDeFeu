#include "ldoelh.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LDOELH w;
    w.LancerHistoire(LDOELH::SORCIER_MONTAGNE_FEU, nullptr, "", "52", true);
    w.show();

    return a.exec();
}
