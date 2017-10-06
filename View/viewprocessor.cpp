#include "viewprocessor.h"

namespace View {

Processor::Processor(int x, int y, Core::Processor* m)
    : x(x*200)
    , y(y*200)
    , over(false)
    , processor(m)
{
    setAcceptHoverEvents(true);
    connect(processor, SIGNAL(changed()), this, SLOT(change()));
}

QRectF Processor::boundingRect() const {
    return QRectF(x,y,100,100);
}

void Processor::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
    over = true;
    QGraphicsDropShadowEffect * effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(50);
    effect->setColor(Qt::white);
    effect->setOffset(0,0);
    effect->setBlurRadius(10);
    setGraphicsEffect(effect);
}

void Processor::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
    over = false;
    graphicsEffect()->deleteLater();
}

void Processor::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();

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
    int cores = processor->nOfThreads();
    if (cores > 1) {
        // multicore paint
        for (int i = 0; i < cores; i++) {
            if (processor->isIdle(i))
                painter->setBrush(Qt::white);
            else
                painter->setBrush(QBrush(processor->runningNode(i)->getColor()));
            painter->drawPie(x-25,y-25,150,150,5760/cores*i,5760/cores);
        }
    } else {
        // single core paint
        if (processor->isIdle())
            painter->setBrush(Qt::white);
        else
            painter->setBrush(QBrush(processor->runningNode()->getColor()));
        painter->drawPath(path);
    }

    painter->setClipping(false);

    // Outline black
    painter->setPen(QPen(Qt::black,3));
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);
}

void Processor::change() {
    this->update();
}

} // namespace View
