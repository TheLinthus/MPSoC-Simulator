#include "viewmpsoc.h"

namespace View {

MPSoC::MPSoC(int x, int y)
    : x(x*200)
    , y(y*200)
    , over(false)
{
    setAcceptHoverEvents(true);
}

QRectF MPSoC::boundingRect() const {
    return QRectF(x,y,100,100);
}

void MPSoC::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    over = true;
    QGraphicsDropShadowEffect * effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(50);
    effect->setColor(Qt::white);
    effect->setOffset(0,0);
    effect->setBlurRadius(10);
    setGraphicsEffect(effect);
}

void MPSoC::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    over = false;
    graphicsEffect()->deleteLater();
}

void MPSoC::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QRectF rect = boundingRect();

    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);

    QPainterPath path;
    path.addRoundedRect(rect,10,10);

    if (over) {

    }

    painter->setClipPath(path);
    painter->setPen(Qt::NoPen);

    painter->setBrush(QBrush(Qt::white));
    painter->drawPath(path);

    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(QBrush(Qt::red));
    int cores = qrand() % 8;
    if (cores > 1) {
        // multicore paint
        for (int i = 0; i < cores; i++) {
            if (qrand() % 2)
                painter->setBrush(QBrush(QColor(qrand() % 16 * 16, qrand() % 16 * 16, qrand() % 16 * 16)));
            else
                painter->setBrush(Qt::white);
            painter->drawPie(x-25,y-25,150,150,5760/cores*i,5760/cores);
        }
    } else {
        // single core paint
    }

    painter->setClipping(false);

    // Outline black
    painter->setPen(QPen(Qt::black,3));
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);
}

} // namespace View
