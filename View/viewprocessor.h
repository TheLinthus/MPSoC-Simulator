#ifndef VIEW_MPSOC_H
#define VIEW_MPSOC_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QtGui>
#include <QStyle>
#include <QGraphicsEffect>
#include <Core/core.h>

namespace View {

class Processor : public QGraphicsItem {
public:
    Processor(Core::Core *core);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:

    // TODO - add event handlers

private:
    int x;
    int y;
    bool over;

    Core::Core *core;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};

} // namespace View

#endif // VIEW_MPSOC_H
