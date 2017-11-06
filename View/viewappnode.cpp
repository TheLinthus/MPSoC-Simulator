#include "viewappnode.h"

namespace View {

AppNode::AppNode(const qreal x, const int y, const int s, const int n) :
    x(x),
    y(y),
    s(s),
    n(n)
{
    setAcceptHoverEvents(true);
}

QRectF AppNode::boundingRect() const {
    return QRectF((x * 150) - ((s * 150) / 2), 150 * y, 100, 100);
}

void AppNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QRectF rect = boundingRect();

    painter->setBrush(QBrush(Qt::white));
    painter->setPen(QPen(Qt::black, 3));

    painter->drawEllipse(rect);

    painter->setFont(QFont("Geneva", 36, QFont::Bold));
    painter->setPen(QPen(Qt::black));

    QTextOption options;
    options.setAlignment(Qt::AlignCenter);

    painter->drawText(rect, QString::number(n), options);
}

void AppNode::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
    qDebug() << boundingRect();
    if (hoverEffect != 0) {
        hoverEffect = new QGraphicsDropShadowEffect();
        hoverEffect->setBlurRadius(25);
        hoverEffect->setColor(QColor(Qt::white));
        hoverEffect->setOffset(0,0);
        setGraphicsEffect(hoverEffect);
    }
    over = true;
}

void AppNode::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
    over = false;
    hoverEffect->deleteLater();
}

} // namespace View
