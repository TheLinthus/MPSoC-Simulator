#ifndef VIEW_APPNODE_H
#define VIEW_APPNODE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsEffect>
#include <QDebug>
namespace View {

class AppNode : public QGraphicsObject {
    Q_OBJECT
public:
    AppNode(const qreal x, const int y, const int s, const int n);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    qreal x;
    int y;
    int s;
    int n;
    bool over;

    QGraphicsDropShadowEffect *hoverEffect;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};

} // namespace View

#endif // VIEW_APPNODE_H
