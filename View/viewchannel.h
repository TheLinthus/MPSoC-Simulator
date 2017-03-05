#ifndef VIEW_CHANNEL_H
#define VIEW_CHANNEL_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QtGui>
#include <QStyle>
#include <QGraphicsEffect>

namespace View {

class Channel : public QGraphicsItem {
public:
    Channel(int x, int y, bool v);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    bool vertical;
    int x, y, w, h;
    bool over;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};

} // namespace View

#endif // VIEW_CHANNEL_H
