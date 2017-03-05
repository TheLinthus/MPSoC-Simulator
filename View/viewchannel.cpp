#include "viewchannel.h"

namespace View {

Channel::Channel(int x, int y, bool v)
    : x((v ? 10 : 100) + (x * 200))
    , y((v ? 100 : 10) + (y * 2
                          00))
    , w(v ? 10 : 100)
    , h(v ? 100 : 10)
    , vertical(v)
    , over(false)
{
    setAcceptHoverEvents(true);
}

QRectF Channel::boundingRect() const {
    return QRectF(x,y,w,h);
}

void Channel::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    over = true;
    QGraphicsDropShadowEffect * effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(50);
    effect->setColor(Qt::white);
    effect->setOffset(0,0);
    effect->setBlurRadius(10);
    setGraphicsEffect(effect);
}

void Channel::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    over = false;
    graphicsEffect()->deleteLater();
}

void Channel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QRectF rect = boundingRect();

    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);

    QPainterPath path;
    path.addRect(rect);

    if (over) {

    }

    painter->setPen(Qt::NoPen);

    painter->setBrush(QBrush(Qt::white)); // Branco ao vermelho, percentual da carga no canal
    painter->drawPath(path);
}

} // namespace View
