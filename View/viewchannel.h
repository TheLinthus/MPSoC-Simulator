#ifndef VIEW_CHANNEL_H
#define VIEW_CHANNEL_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QtGui>
#include <QStyle>
#include <QGraphicsEffect>
#include <Core/channel.h>

namespace View {

class Channel : public QObject, public QGraphicsItem {
    Q_OBJECT

public:
    Channel(int x, int y, bool v, Core::Channel* c);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public slots:
    void change();

private:
    bool vertical;
    int x, y, w, h;
    bool over;

    Core::Channel* channel;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

} // namespace View

#endif // VIEW_CHANNEL_H
