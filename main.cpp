#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationDomain("tecnomissoes.com.br");
    QCoreApplication::setOrganizationName("Tecno Missões");
    QCoreApplication::setApplicationName("MPSoC Emulator");
    QCoreApplication::setApplicationVersion("1.0.0");

    MainWindow w;
    w.show();

    qInfo("Start!");
    return a.exec();
}
