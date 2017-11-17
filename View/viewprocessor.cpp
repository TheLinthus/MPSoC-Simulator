#include "viewprocessor.h"

namespace View {

Processor::Processor(int x, int y, Core::Processor* m)
    : x(x*200)
    , y(y*200)
    , over(false)
    , processor(m)
    , hoverEffect(new QGraphicsDropShadowEffect())
{
    setAcceptHoverEvents(true);
    connect(processor, SIGNAL(changed()), this, SLOT(change()));

    setZValue(1);

    hoverEffect->setBlurRadius(25);
    hoverEffect->setColor(QColor(Qt::white));
    hoverEffect->setOffset(0,0);
    hoverEffect->setEnabled(false);
    setGraphicsEffect(hoverEffect);

    updateToolTip();
}

QRectF Processor::boundingRect() const {
    return QRectF(x,y,100,100);
}

void Processor::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
    hoverEffect->setEnabled(true);
    over = true;
}

void Processor::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
    over = false;
    hoverEffect->setEnabled(false);
}

void Processor::updateToolTip() {
    QMetaEnum qenum = QMetaEnum::fromType<Core::Processor::Type>();
    QString toolTip = QString(tr("Processor %1, %2<hr>%3<br>%4 thread(s)<hr>")).arg(
                QString::number(processor->getX()),
                QString::number(processor->getY()),
                QString(qenum.valueToKey(processor->getType())),
                QString::number(processor->nOfThreads()));
    for (int i = 0; i < processor->nOfThreads(); i++) {
        toolTip.append(QString("<b>Thread %1</b>: ").arg(i));
        if (processor->isIdle(i)) {
            toolTip.append(tr("Idle"));
        } else {
            toolTip.append(((Core::Application *) processor->runningNode(i)->parent())->getName());
            toolTip.append(tr("<b> Node </b>"));
            toolTip.append(QString::number(processor->runningNode(i)->getN()));
        }
    }
    setToolTip(toolTip);
}

void Processor::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();

    QPainterPath path;

    path.addRoundedRect(rect,10,10);

    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(QColor(255,255,255,127)));

    if (over) {
        painter->drawRoundedRect(rect.adjusted(-4,-4,4,4),13,13);
    }

    painter->setBrush(QBrush(Qt::white));

    painter->setClipPath(path);

    painter->setPen(QPen(Qt::black, 1));
    int cores = processor->nOfThreads();
    if (cores > 1) {
        // multicore paint
        for (int i = 0; i < cores; i++) {
            if (processor->isIdle(i))
                painter->setBrush(Qt::white);
            else
                painter->setBrush(QBrush(((Core::Application *)processor->runningNode(i)->parent())->getColor()));
            painter->drawPie(x-25,y-25,150,150,5760/cores*i,5760/cores);
        }
    } else {
        // single core paint
        if (processor->isIdle())
            painter->setBrush(Qt::white);
        else
            painter->setBrush(QBrush(((Core::Application *)processor->runningNode()->parent())->getColor()));
        painter->drawPath(path);
    }

    painter->setClipping(false);

    // Outline black
    painter->setPen(QPen(Qt::black,3));
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);

    // Master indicator
    if (processor->isMaster()) {
        painter->setPen(QPen(Qt::black,4));
        painter->setBrush(QBrush(QColor::fromRgb(0xff,0xae,0x2d)));
        painter->drawEllipse(rect.topLeft() + QPoint(10,10), 18, 18);

        painter->setFont(QFont("Geneva", 18, QFont::Bold));
        painter->setPen(QPen(Qt::black));
        painter->drawText(rect.topLeft() + QPointF(0,18), "M");
    }

}

void Processor::change() {
    this->update();
    updateToolTip();
}

} // namespace View
