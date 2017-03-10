#include "mpsocbox.h"
#include "ui_mpsocbox.h"

namespace View {

MPSoCBox::MPSoCBox(QWidget *parent) :
    QWidget(parent)
    , gridWidth(1)
    , gridHeight(1)
    , mpsoc(nullptr)
    , ui(new Ui::MPSoCBox)
{
    ui->setupUi(this);
    mpsocScene = new QGraphicsScene(this);
    ui->mpsocGV->setScene(mpsocScene);
    ui->mpsocGV->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
    
}

MPSoCBox::~MPSoCBox() {
    delete ui;
    delete mpsoc;
}

} // namespace View

void View::MPSoCBox::on_closeButton_clicked() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, QWidget::window()->windowTitle(), "Are you sure you want to remove this MPSoC?",
                                QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        this->deleteLater();
    }
}

Core::MPSoC *View::MPSoCBox::getMpsoc() const {
    return mpsoc;
}

int View::MPSoCBox::getGridWidth() const {
    return gridWidth;
}

void View::MPSoCBox::setMPSoC(Core::MPSoC *value) {
    if (mpsoc != nullptr) {
        mpsocScene->clear();
        delete mpsoc;
    }
    gridWidth = value->getWidth();
    gridHeight = value->getHeight();
    mpsocScene->setSceneRect(-50,-50,gridWidth*200,gridHeight*200);
    mpsoc = value;

    mpsocScene->addRect(-50,-50,gridWidth*200,gridHeight*200,QPen(QColor(32,32,32),5), QBrush(QColor(64,64,64)));

    // Desenha MPSoC na scene;
    for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
            Processor *processor = new Processor(mpsoc->getCore(i,j));
            mpsocScene->addItem(processor);
            if (mpsoc->getCore(i,j)->getChannel(Core::South)) {
                mpsocScene->addItem(new Channel(i,j,true));
            }
            if (mpsoc->getCore(i,j)->getChannel(Core::East)) {
                mpsocScene->addItem(new Channel(i,j,false));
            }

        }
    }
}

void View::MPSoCBox::setHeuristic(QString heuristic) {
    QString text;
    QFile file(QString("Heuristics\\%1.js").arg(heuristic));
    if (!file.exists()) {
        qWarning() << "Heuristic Files doesn't exist";
    }
    qInfo() << "Reading" << QString("Heuristics\\%1.js").arg(heuristic) << "...";
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    text = file.readAll();
    file.close();

    engine = new QScriptEngine();
    engine->evaluate(text);
    qInfo() << "Loaded Heuristic" << engine->evaluate("name").toString() << ". From" << engine->evaluate("author").toString();
    qInfo() << "Test chossing a core:" << engine->evaluate("selectCore()").toString();

    ui->titleLabel->setText(QString("%1x%2 MPSoC, %3").arg(gridWidth).arg(gridHeight).arg(engine->evaluate("name").toString()));
}

void View::MPSoCBox::resizeEvent(QResizeEvent *) {
    ui->mpsocGV->setMaximumHeight(ui->mpsocGV->width());
    ui->mpsocGV->fitInView(mpsocScene->sceneRect(),Qt::KeepAspectRatio);
}

int View::MPSoCBox::getGridHeight() const {
    return gridHeight;
}
