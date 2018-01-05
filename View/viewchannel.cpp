#include "viewchannel.h"

namespace View {

Channel::Channel(Core::Channel *c, bool vertical, bool out)
    : vertical(vertical)
    , out(out)
    , x((vertical ? (out ? 10 : 40) : 102) + qMin(c->getAx(), c->getBx()) * 200)
    , y((vertical ? 102 : (out ? 10 : 40)) + qMin(c->getAy(), c->getBy()) * 200)
    , w(vertical ? 25 : 96)
    , h(vertical ? 96 : 25)
    , over(false)
    , channel(c)
    , hoverEffect(new QGraphicsDropShadowEffect())
{
    setAcceptHoverEvents(true);
    connect(channel, SIGNAL(loadChanged(int)), this, SLOT(change()));

    setZValue(0);

    hoverEffect->setBlurRadius(25);
    hoverEffect->setColor(QColor(Qt::white));
    hoverEffect->setOffset(0,0);
    hoverEffect->setEnabled(false);
    setGraphicsEffect(hoverEffect);

    updateToolTip();
}

QRectF Channel::boundingRect() const {
    return QRectF(x,y,w,h);
}

void Channel::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
    hoverEffect->setEnabled(true);
    over = true;
}

void Channel::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
    over = false;
    hoverEffect->setEnabled(false);
}

void Channel::mousePressEvent(QGraphicsSceneMouseEvent *) {

}

void Channel::updateToolTip() {
    QString toolTip("(%1, %2) to (%3, %4)<br>Channel load: %5");
    toolTip = toolTip.arg(channel->getAx()).arg(channel->getAy()).arg(channel->getBx()).arg(channel->getBy()).arg(channel->val());
    setToolTip(toolTip);
}

void Channel::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();

    QPainterPath path;
    path.addRect(rect);

    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(QColor(255,255,255,127)));

    if (over) {
        if (vertical) {
            painter->drawRect(rect.adjusted(-2,0,2,0));
        } else {
            painter->drawRect(rect.adjusted(0,-2,0,2));
        }
    }

    int g, b;

    qreal val = channel->val();

    if (val > 100) {
        painter->setBrush(QBrush(QColor(255,186,0)));
        painter->drawRect(rect.adjusted(-6,-6,6,6));
    }

    if (val >= 100) {
        g = b = 0;
    } else if (val <= 50) {
        g = 255;
        b = (50 - val) * 5.1;
    } else {
        g = 255 - (val - 50) * 5.1;
        b = 0;
    }
    painter->setBrush(QBrush(QColor(255,g,b)));
    painter->drawPath(path);

    int color = 255 - (g / 2) - (b / 2);
    painter->setBrush(QBrush(QColor(color,color,color)));

    QPolygonF arrow;
    QPainterPath arrows;

    arrow << QPointF(10.0, 12.5);
    arrow << QPointF(25.0, 2.5);
    arrow << QPointF(30.0, 2.5);
    arrow << QPointF(22.5, 10.0);
    arrow << QPointF(40.0, 10.0);
    arrow << QPointF(40.0, 15.0);
    arrow << QPointF(22.5, 15.0);
    arrow << QPointF(30.0, 22.5);
    arrow << QPointF(25.0, 22.5);

    arrow.translate(-48.0,-12.5);
    arrows.addPolygon(arrow);
    arrow.translate(45,0);
    arrows.addPolygon(arrow);

    painter->translate(x,y);
    if (vertical) {
        painter->rotate(90);
        painter->translate(0,-25);
    }
    painter->translate(48,12.5);
    if (out) {
        painter->scale(-1,-1);
    }

    painter->drawPath(arrows);
}

void Channel::change() {
    this->update();
    updateToolTip();
}

} // namespace View
