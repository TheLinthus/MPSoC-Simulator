#include "graphicalmpsoc.h"
#include "ui_graphicalmpsoc.h"

GraphicalMPSoC::GraphicalMPSoC(QWidget *parent)
    : QWidget(parent)
    , gridWidth(1)
    , gridHeight(1)
    , mpsoc(nullptr)
{
}

GraphicalMPSoC::~GraphicalMPSoC()
{
    delete mpsoc;
}

void GraphicalMPSoC::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    p.setPen(QPen(QColor(0,0,0,0)));
    p.fillRect(this->rect(), QBrush(QColor(32,32,32)));

    QRectF border(0,0,0,0);
    qreal coreSize, channelLenght, channelThickness;
    int fontSize;

    if (width() / gridWidth < height() / gridHeight) {
        coreSize = (width() - 6) / (gridWidth * 2 - 1);
        border.setWidth(width() - 6);
        border.setHeight((gridHeight * 2 - 1) * coreSize);
        border.setX(3);
        border.setY((height() / 2 - border.height() / 2) + 3);
    } else {
        coreSize = (height() - 6) / (gridHeight * 2 - 1);
        border.setWidth((gridWidth * 2 - 1) * coreSize);
        border.setHeight(height() - 6);
        border.setX((width() / 2 - border.width() / 2) + 3);
        border.setY(3);
    }
    channelLenght = coreSize;
    channelThickness = qMin((qreal)6, (qreal)coreSize / 4.6);
    fontSize = qMax(7, qMin(14,(int) (coreSize / 2.8)));

    if (mpsoc) {
        p.setRenderHint(QPainter::Antialiasing);
        p.setFont(QFont("Courier New", fontSize));

        for (int i = 0; i < gridWidth; i++) {
            for (int j = 0; j < gridHeight; j++) {
                p.setPen(QColor(220,220,220));
                qreal lifespan = 0;
                if (mpsoc->getCore(i,j)->isIdle()) {
                    p.setBrush(Qt::transparent);
                } else {
                    p.setBrush(mpsoc->getCore(i,j)->runningNode()->getColor());
                    lifespan = mpsoc->getCore(i,j)->runningNode()->getLifespan();
                }
                p.drawRoundedRect(QRectF(
                               border.x() + (i * coreSize + i * channelLenght),
                               border.y() + (j * coreSize + j * channelLenght),
                               coreSize, coreSize
                               ),coreSize/8,coreSize/8);
                QColor channelColor(1.0,1.0,1.0);
                p.setPen(Qt::white);
                p.drawText(
                            border.x() + (i * coreSize + i * channelLenght),
                            border.y() + (j * coreSize + j * channelLenght),
                            coreSize, coreSize,
                            Qt::AlignVCenter | Qt::AlignHCenter,
                            QString::number(lifespan));
                if (mpsoc->getCore(i,j)->getChannel(Core::South)) {
                    qreal chVal = mpsoc->getCore(i,j)->getChannel(Core::South)->val();
                    if (chVal == 0) {
                        channelColor.setRgbF(0.8, 0.8, 0.8);
                    } else if (chVal <= 50) {
                        channelColor.setRgbF(1.0, 1.0, 1.0 - chVal / 50);
                    } else if (chVal <= 100) {
                        channelColor.setRgbF(1.0, 1.0 - (chVal - 50) / 50, 0.0);
                    } else {
                        channelColor.setRgbF(1.0, 0.0, 1.0);
                    }
                    p.setBrush(channelColor);
                    p.setPen(Qt::transparent);
                    p.drawRect(QRectF(  // Drawn Channel Graphical representation at South of Core i,j
                                   border.x() + (i * coreSize + i * channelLenght + coreSize / 8),
                                   border.y() + (j * coreSize + j * channelLenght + coreSize + 1),
                                   channelThickness, channelLenght - 2
                                   ));
                    p.setPen(channelColor);
                    p.drawText(         // Drawn Channel # Load at South of Core i,j
                                border.x() + (i * coreSize + i * channelLenght + coreSize / 8 + channelThickness * 2),
                                border.y() + (j * coreSize + j * channelLenght + coreSize),
                                coreSize + channelLenght - channelThickness * 2 - coreSize / 8, channelLenght,
                                Qt::AlignVCenter | Qt::AlignLeft,
                                QString::number(chVal,'f',1));
                }
                if (mpsoc->getCore(i,j)->getChannel(Core::East)) {
                    qreal chVal = mpsoc->getCore(i,j)->getChannel(Core::East)->val();
                    if (chVal == 0) {
                        channelColor.setRgbF(0.8, 0.8, 0.8);
                    } else if (chVal <= 50) {
                        channelColor.setRgbF(1.0, 1.0, 1.0 - chVal / 50);
                    } else if (chVal <= 100) {
                        channelColor.setRgbF(1.0, 1.0 - (chVal - 50) / 50, 0.0);
                    } else {
                        channelColor.setRgbF(1.0, 0.0, 1.0);
                    }
                    p.setBrush(channelColor);
                    p.setPen(Qt::transparent);
                    p.drawRect(QRectF(
                                   border.x() + (i * coreSize + i * channelLenght + coreSize + 1),
                                   border.y() + ((j + 1) * coreSize + j * channelLenght - coreSize / 8 - channelThickness),
                                   channelLenght - 2, channelThickness
                                   ));
                    p.setPen(channelColor);
                    QRectF text(border.x() + (i * coreSize + i * channelLenght + coreSize),
                                border.y() + (j * coreSize + j * channelLenght),
                                channelLenght, coreSize - coreSize / 8 - channelThickness);
                    p.setPen(channelColor);
                    p.drawText(
                                text,
                                Qt::AlignVCenter | Qt::AlignHCenter,
                                QString::number(chVal,'f',1));
                }
            }
        }
    } else {
        p.setPen(Qt::red);
        p.setFont(QFont("Helvetica", 40));
        p.drawText(border, "No MPSoC", Qt::AlignVCenter | Qt::AlignHCenter);
    }
}

Core::MPSoC *GraphicalMPSoC::getMpsoc() const
{
    return mpsoc;
}

int GraphicalMPSoC::getGridWidth() const
{
    return gridWidth;
}

void GraphicalMPSoC::setMPSoC(Core::MPSoC *value)
{
    if (mpsoc != nullptr) {
        disconnect(mpsoc, SIGNAL(changed()), this, SLOT(update()));
        delete mpsoc;
    }
    gridWidth = value->getWidth();
    gridHeight = value->getHeight();
    mpsoc = value;
    connect(mpsoc, SIGNAL(changed()), this, SLOT(update()));
    update();
}

int GraphicalMPSoC::getGridHeight() const
{
    return gridHeight;
}
