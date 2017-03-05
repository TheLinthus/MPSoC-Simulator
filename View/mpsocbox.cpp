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
}

MPSoCBox::~MPSoCBox()
{
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
            mpsocScene->addItem(new MPSoC(i,j));
            if (mpsoc->getCore(i,j)->getChannel(Core::South)) {
                mpsocScene->addItem(new Channel(i,j,true));
            }
            if (mpsoc->getCore(i,j)->getChannel(Core::East)) {
                mpsocScene->addItem(new Channel(i,j,false));
            }


//            p.setPen(QColor(220,220,220));
//            qreal lifespan = 0;
//            if (mpsoc->getCore(i,j)->isIdle()) {
//                p.setBrush(Qt::transparent);
//            } else {
//                p.setBrush(mpsoc->getCore(i,j)->runningNode()->getColor());
//                lifespan = mpsoc->getCore(i,j)->runningNode()->getLifespan();
//            }
//            p.drawRoundedRect(QRectF(
//                           border.x() + (i * coreSize + i * channelLenght),
//                           border.y() + (j * coreSize + j * channelLenght),
//                           coreSize, coreSize
//                           ),coreSize/8,coreSize/8);
//            QColor channelColor(1.0,1.0,1.0);
//            p.setPen(Qt::white);
//            p.drawText(
//                        border.x() + (i * coreSize + i * channelLenght),
//                        border.y() + (j * coreSize + j * channelLenght),
//                        coreSize, coreSize,
//                        Qt::AlignVCenter | Qt::AlignHCenter,
//                        QString::number(lifespan));
//            if (mpsoc->getCore(i,j)->getChannel(Core::South)) {
//                qreal chVal = mpsoc->getCore(i,j)->getChannel(Core::South)->val();
//                if (chVal == 0) {
//                    channelColor.setRgbF(0.8, 0.8, 0.8);
//                } else if (chVal <= 50) {
//                    channelColor.setRgbF(1.0, 1.0, 1.0 - chVal / 50);
//                } else if (chVal <= 100) {
//                    channelColor.setRgbF(1.0, 1.0 - (chVal - 50) / 50, 0.0);
//                } else {
//                    channelColor.setRgbF(1.0, 0.0, 1.0);
//                }
//                p.setBrush(channelColor);
//                p.setPen(Qt::transparent);
//                p.drawRect(QRectF(  // Drawn Channel Graphical representation at South of Core i,j
//                               border.x() + (i * coreSize + i * channelLenght + coreSize / 8),
//                               border.y() + (j * coreSize + j * channelLenght + coreSize + 1),
//                               channelThickness, channelLenght - 2
//                               ));
//                p.setPen(channelColor);
//                p.drawText(         // Drawn Channel # Load at South of Core i,j
//                            border.x() + (i * coreSize + i * channelLenght + coreSize / 8 + channelThickness * 2),
//                            border.y() + (j * coreSize + j * channelLenght + coreSize),
//                            coreSize + channelLenght - channelThickness * 2 - coreSize / 8, channelLenght,
//                            Qt::AlignVCenter | Qt::AlignLeft,
//                            QString::number(chVal,'f',1));
//            }
//            if (mpsoc->getCore(i,j)->getChannel(Core::East)) {
//                qreal chVal = mpsoc->getCore(i,j)->getChannel(Core::East)->val();
//                if (chVal == 0) {
//                    channelColor.setRgbF(0.8, 0.8, 0.8);
//                } else if (chVal <= 50) {
//                    channelColor.setRgbF(1.0, 1.0, 1.0 - chVal / 50);
//                } else if (chVal <= 100) {
//                    channelColor.setRgbF(1.0, 1.0 - (chVal - 50) / 50, 0.0);
//                } else {
//                    channelColor.setRgbF(1.0, 0.0, 1.0);
//                }
//                p.setBrush(channelColor);
//                p.setPen(Qt::transparent);
//                p.drawRect(QRectF(
//                               border.x() + (i * coreSize + i * channelLenght + coreSize + 1),
//                               border.y() + ((j + 1) * coreSize + j * channelLenght - coreSize / 8 - channelThickness),
//                               channelLenght - 2, channelThickness
//                               ));
//                p.setPen(channelColor);
//                QRectF text(border.x() + (i * coreSize + i * channelLenght + coreSize),
//                            border.y() + (j * coreSize + j * channelLenght),
//                            channelLenght, coreSize - coreSize / 8 - channelThickness);
//                p.setPen(channelColor);
//                p.drawText(
//                            text,
//                            Qt::AlignVCenter | Qt::AlignHCenter,
//                            QString::number(chVal,'f',1));
//            }
        }
    }
}

void View::MPSoCBox::resizeEvent(QResizeEvent *) {
    ui->mpsocGV->setMaximumHeight(ui->mpsocGV->width());
    ui->mpsocGV->fitInView(mpsocScene->sceneRect(),Qt::KeepAspectRatio);
}

int View::MPSoCBox::getGridHeight() const {
    return gridHeight;
}
