#include "mpsoc_se.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationDomain("tecnomissoes.com.br");
    QCoreApplication::setOrganizationName("Tecno Missões");
    QCoreApplication::setApplicationName("MPSoC Simulation Environment");
    QCoreApplication::setApplicationVersion("1.0.0");

    MPSoC_Simulator w;
    w.show();

    return a.exec();
}
