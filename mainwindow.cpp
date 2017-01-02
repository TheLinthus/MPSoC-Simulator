
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer(this))
{
    connect(timer, SIGNAL(timeout()), this, SLOT(increment()));
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_addMPSoCButton_clicked()
{
    int x = QInputDialog::getInt(this,"X","Entre o tamanho X:",1,2,16);;
    int y = QInputDialog::getInt(this,"Y","Entre o tamanho Y:",1,2,16);;

    Core::MPSoC* m = new Core::MPSoC(x,y);
    ui->widget->setMPSoC(m);

    ci = 0;
    timer->start(350);
}

void MainWindow::increment() {
    int x1, y1;
    QColor *colors[6] = {
        new QColor("blue"),
        new QColor("cyan"),
        new QColor("green"),
        new QColor("yellow"),
        new QColor("red"),
        new QColor("magenta")
    };
    x1 = qrand() % ui->widget->getGridWidth();
    y1 = qrand() % ui->widget->getGridHeight();
    if (!ui->widget->getMpsoc()->getCore(x1,y1)->isIdle())
        return;
    Core::AppNode *node = new Core::AppNode(qrand() % 80 + 5,colors[ci%6]);
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
}
