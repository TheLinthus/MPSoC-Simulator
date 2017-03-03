#ifndef VIEW_MPSOC_H
#define VIEW_MPSOC_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QtGui>
#include <QStyle>
#include <QGraphicsEffect>

namespace View {

class MPSoC : public QGraphicsItem {
public:
    MPSoC(int x, int y);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:

    // TODO - add event handlers

private:
    int x;
    int y;
    bool over;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};

} // namespace View

#endif // VIEW_MPSOC_H
