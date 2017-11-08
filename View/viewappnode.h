#ifndef VIEW_APPNODE_H
#define VIEW_APPNODE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsEffect>
#include <QDebug>
#include <Core/appnode.h>

namespace View {

class AppNode : public QGraphicsObject {
    Q_OBJECT
public:
    AppNode(int n, Core::AppNode *appNode);

    int getN() const;
    Core::AppNode *getAppNode();

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    int n;
    bool over;

    Core::AppNode *appNode;

    QGraphicsDropShadowEffect *hoverEffect;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};

} // namespace View

#endif // VIEW_APPNODE_H
