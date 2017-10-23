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

class Channel : public QGraphicsObject {
    Q_OBJECT

public:
    Channel(int x, int y, bool v, Core::Channel* c);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

public slots:
    void change();

private:
    bool vertical;
    int x, y, w, h;
    bool over;

    Core::Channel* channel;

    QGraphicsDropShadowEffect *hoverEffect;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *) override;
    void updateToolTip();
};

} // namespace View

#endif // VIEW_CHANNEL_H
