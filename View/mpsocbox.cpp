#include "mpsocbox.h"
#include "ui_mpsocbox.h"

namespace View {

MPSoCBox::MPSoCBox(QWidget *parent) :
    QWidget(parent)
    , gridWidth(1)
    , gridHeight(1)
    , mpsoc(nullptr)
    , heuristic(nullptr)
    , heuristicName(QString("Null"))
    , ui(new Ui::MPSoCBox)
{
    ui->setupUi(this);
    mpsocScene = new QGraphicsScene(this);
    ui->mpsocGV->setScene(mpsocScene);
    ui->mpsocGV->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
}

MPSoCBox::~MPSoCBox() {
    delete ui;
}

} // namespace View

void View::MPSoCBox::on_closeButton_clicked() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, QWidget::window()->windowTitle(), "Are you sure you want to remove this MPSoC?\nIt will resut in reseting all active simulations.",
                                QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        this->deleteLater();
        mpsocs->remove(mpsoc);
    }
}

Core::MPSoC *View::MPSoCBox::getMpsoc() const {
    return mpsoc;
}

int View::MPSoCBox::getGridWidth() const {
    return gridWidth;
}

void View::MPSoCBox::setMPSoC(Core::MPSoC *value) {
    if (mpsoc != nullptr)
        disconnect(mpsoc, SIGNAL(destroyed(QObject*)), this, SLOT(deleteLater()));
    mpsoc = value;
    connect(mpsoc, SIGNAL(destroyed(QObject*)), this, SLOT(deleteLater()));

    connect(mpsoc, SIGNAL(changed()), this, SLOT(drawnMPSoC()));
    connect(mpsoc, SIGNAL(destroyed(QObject*)), this, SLOT(deleteLater()));

    ui->titleLabel->setText(title.arg(QString::number(gridWidth), QString::number(gridHeight), heuristicName));

    drawnMPSoC();
}

void View::MPSoCBox::drawnMPSoC() {
    mpsocScene->clear();

    gridWidth = mpsoc->getWidth();
    gridHeight = mpsoc->getHeight();
    mpsocScene->setSceneRect(-50,-50,gridWidth*200,gridHeight*200);

    mpsocScene->addRect(-50,-50,gridWidth*200,gridHeight*200,QPen(QColor(32,32,32),5), QBrush(QColor(64,64,64)));

    // Desenha MPSoC na scene;
    for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
            Core::Processor * cp = mpsoc->getCore(i,j);
            View::Processor * vp = new View::Processor(i,j,cp);
            mpsocScene->addItem(vp);
            if (Core::Channel * cc = mpsoc->getCore(i,j)->getChannel(Core::South)) {
                View::Channel * vc = new View::Channel(i,j,true,cc);
                mpsocScene->addItem(vc);
            }
            if (Core::Channel * cc = mpsoc->getCore(i,j)->getChannel(Core::East)) {
                View::Channel * vc = new View::Channel(i,j,false,cc);
                mpsocScene->addItem(vc);
            }
        }
    }
}

void View::MPSoCBox::setHeuristic(Core::Heuristic *heuristic) {
    this->heuristic = heuristic;
    heuristicName = heuristic->getName();

    ui->titleLabel->setText(title.arg(QString::number(gridWidth), QString::number(gridHeight), heuristicName));
}

void View::MPSoCBox::fitInView() {
    ui->mpsocGV->setMaximumHeight(ui->mpsocGV->width());
    ui->mpsocGV->fitInView(mpsocScene->sceneRect(),Qt::KeepAspectRatio);
}

void View::MPSoCBox::paintEvent(QPaintEvent *) {
    fitInView();
}

int View::MPSoCBox::getGridHeight() const {
    return gridHeight;
}
