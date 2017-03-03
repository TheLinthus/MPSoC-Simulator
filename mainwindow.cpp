
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer(this))
{
    //connect(timer, SIGNAL(timeout()), this, SLOT(increment()));
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_addMPSoCButton_clicked() {
    bool ok = false;
    int x = QInputDialog::getInt(this,windowTitle(),"Entre o tamanho X:",2,2,16,1,&ok);
    if (!ok) return;
    int y = QInputDialog::getInt(this,windowTitle(),"Entre o tamanho Y:",2,2,16,1,&ok);
    if (!ok) return;

    Core::MPSoC* m = new Core::MPSoC(x,y);

    QHBoxLayout* layout =  ui->mpsocsLayout;

    View::MPSoCBox* newmpsoc = new View::MPSoCBox();
    newmpsoc->setMPSoC(m);

    layout->insertWidget(layout->count() - 1, newmpsoc);


    //ui->widget->setMPSoC(m);

    ci = 0;
    //timer->start(350);
}

/*void MainWindow::increment() {
    int x1, y1;
    QColor colors[6] = {
        QColor("blue"),
        QColor("cyan"),
        QColor("green"),
        QColor("yellow"),
        QColor("red"),
        QColor("magenta")
    };
    x1 = qrand() % ui->widget->getGridWidth();
    y1 = qrand() % ui->widget->getGridHeight();
    if (!ui->widget->getMpsoc()->getCore(x1,y1)->isIdle())
        return;
    Core::AppNode *node = new Core::AppNode(qrand() % 80 + 5);
    node->setColor(colors[ci%6]);
    connect(timer, SIGNAL(timeout()), node, SLOT(tick()));
    connect(timer, SIGNAL(timeout()), ui->widget, SLOT(repaint()));
    ui->widget->getMpsoc()->getCore(x1,y1)->run(node);

    //qreal atob, btoa;
    //atob = (qreal)(qrand() % 400) / 10.0;
    //btoa = (qreal)(qrand() % 400) / 10.0;

    //QVector<Core::Channel *> patch = ui->widget->getMpsoc()->getPatch(x1,y1,x2,y2);
    //foreach (Core::Channel *c, patch) {
    //    c->add(atob);
    //}

    //patch = ui->widget->getMpsoc()->getPatch(x2,y2,x1,y1);
    //foreach (Core::Channel *c, patch) {
    //    c->add(btoa);
    //}

    ci++;
}*/

void MainWindow::on_pushButton_clicked() {
    QString osInfo = QSysInfo::prettyProductName();
    QString path = QStandardPaths::locate(QStandardPaths::AppDataLocation, "", QStandardPaths::LocateDirectory);
    qDebug() << "You are running on " << osInfo << "\nThis application's data folder is in " << path;
    QDesktopServices::openUrl(QUrl("file:///"+path));
}
