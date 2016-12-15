#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QTimer>
#include <QVector>

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

void MainWindow::on_pushButton_2_clicked() {
}

void MainWindow::on_addMPSoCButton_clicked()
{
    int x = QInputDialog::getInt(this,"X","Entre o tamanho X:",1,2,16);;
    int y = QInputDialog::getInt(this,"Y","Entre o tamanho Y:",1,2,16);;

    Core::MPSoC* m = new Core::MPSoC(x,y);
    ui->widget->setMPSoC(m);

    ci = 0;
    timer->start(10000);
}

void MainWindow::increment() {
    int x1, x2, y1, y2;
    QColor *colors[6] = {
        new QColor("blue"),
        new QColor("cyan"),
        new QColor("green"),
        new QColor("yellow"),
        new QColor("red"),
        new QColor("magenta")
    };
    do {
        x1 = qrand() % ui->widget->getGridWidth();
        y1 = qrand() % ui->widget->getGridHeight();
    } while (!ui->widget->getMpsoc()->getCore(x1,y1)->isIdle());
    ui->widget->getMpsoc()->getCore(x1,y1)->run(new Core::AppNode(5,colors[ci]));
    do {
        x2 = qrand() % ui->widget->getGridWidth();
        y2 = qrand() % ui->widget->getGridHeight();
    } while (!ui->widget->getMpsoc()->getCore(x2,y2)->isIdle());
    ui->widget->getMpsoc()->getCore(x2,y2)->run(new Core::AppNode(5,colors[ci]));

    qreal atob, btoa;
    atob = (qreal)(qrand() % 400) / 10.0;
    btoa = (qreal)(qrand() % 400) / 10.0;

    QVector<Core::Channel *> patch = ui->widget->getMpsoc()->getPatch(x1,y1,x2,y2);
    foreach (Core::Channel *c, patch) {
        c->add(atob);
    }

    patch = ui->widget->getMpsoc()->getPatch(x2,y2,x1,y1);
    foreach (Core::Channel *c, patch) {
        c->add(btoa);
    }

    ci++;
    if (ci >= 6) timer->stop();

}
