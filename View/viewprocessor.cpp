#include "viewprocessor.h"

namespace View {

Processor::Processor(Core::Core *core)
    : core(core)
    , x(core->getX()*200)
    , y(core->getY()*200)
    , over(false)
{
    setAcceptHoverEvents(true);
}

QRectF Processor::boundingRect() const {
    return QRectF(x,y,100,100);
}

void Processor::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    over = true;
    QGraphicsDropShadowEffect * effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(50);
    effect->setColor(Qt::white);
    effect->setOffset(0,0);
    effect->setBlurRadius(10);
    setGraphicsEffect(effect);
}

void Processor::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    over = false;
    graphicsEffect()->deleteLater();
}

void Processor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
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
    int threads = core->nOfThreads();
    if (threads > 1) {
        // multicore paint
        for (int i = 0; i < threads; i++) {
            if (core->runningNode(i) == nullptr)
                painter->setBrush(Qt::white);
            else
                painter->setBrush(QBrush(core->runningNode(i)->getColor()));
            painter->drawPie(x-25,y-25,150,150,5760/threads*i,5760/threads);
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
