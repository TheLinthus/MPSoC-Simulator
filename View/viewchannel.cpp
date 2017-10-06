#include "viewchannel.h"

namespace View {

Channel::Channel(int x, int y, bool v, Core::Channel *c)
    : x((v ? 10 : 100) + (x * 200))
    , y((v ? 100 : 10) + (y * 200))
    , w(v ? 25 : 100)
    , h(v ? 100 : 25)
    , vertical(v)
    , over(false)
    , channel(c)
{
    setAcceptHoverEvents(true);
    connect(channel, SIGNAL(loadChanged(int)), this, SLOT(change()));

    setToolTip(QString("channel load: %1").arg(channel->val()));
}

QRectF Channel::boundingRect() const {
    return QRectF(x,y,w,h);
}

void Channel::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
    over = true;
    QGraphicsDropShadowEffect * effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(50);
    effect->setColor(Qt::white);
    effect->setOffset(0,0);
    effect->setBlurRadius(10);
    setGraphicsEffect(effect);
}

void Channel::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
    over = false;
    graphicsEffect()->deleteLater();
}

void Channel::mousePressEvent(QGraphicsSceneMouseEvent *) {

}

void Channel::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();

    QPainterPath path;
    path.addRect(rect);

    if (over) {

    }

    painter->setPen(Qt::NoPen);

    int g, b;

    if (channel->val() >= 100) {
        g = b = 0;
    } else if (channel->val() <= 50) {
        g = 255;
        b = (50 - channel->val()) * 5.1;
    } else {
        g = 255 - (channel->val() - 50) * 5.1;
        b = 0;
    }
    painter->setBrush(QBrush(QColor(255,g,b))); // Branco ao vermelho, percentual da carga no canal
    painter->drawPath(path);
}

void Channel::change() {
    this->update();
    setToolTip(QString("channel load: %1").arg(channel->val()));
}

} // namespace View
