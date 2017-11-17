#include "viewappnode.h"

namespace View {

AppNode::AppNode(int n, Core::AppNode *appNode) :
    n(n),
    appNode(appNode),
    hoverEffect(new QGraphicsDropShadowEffect())
{
    setAcceptHoverEvents(true);

    hoverEffect->setBlurRadius(25);
    hoverEffect->setColor(QColor(Qt::white));
    hoverEffect->setOffset(0,0);
    hoverEffect->setEnabled(false);
    setGraphicsEffect(hoverEffect);
}

int AppNode::getN() const {
    return n;
}

Core::AppNode *AppNode::getAppNode() {
    return appNode;
}

QRectF AppNode::boundingRect() const {
    return QRectF(pos().x()-50, pos().y()-50, 100, 100);
}

void AppNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QRectF rect = boundingRect();

    painter->setBrush(QBrush(Qt::white));
    painter->setPen(QPen(Qt::black, 3));

    painter->drawEllipse(rect);

    QTextOption options;
    options.setAlignment(Qt::AlignCenter);

    painter->setFont(QFont("Geneva", 36, QFont::Bold));
    painter->setPen(QPen(Qt::black));

    painter->drawText(rect.adjusted(0,0,0,-50), QString::number(n), options);

    painter->setFont(QFont("Geneva", 18, QFont::Normal));
    painter->drawText(rect.adjusted(0,50,0,0), QString::number(appNode->getLifespan()), options);
}

void AppNode::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
    hoverEffect->setEnabled(true);
    over = true;
}

void AppNode::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
    over = false;
    hoverEffect->setEnabled(false);
}

} // namespace View
