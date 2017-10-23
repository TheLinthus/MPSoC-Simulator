#ifndef VIEW_MPSOC_H
#define VIEW_MPSOC_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QtGui>
#include <QStyle>
#include <QGraphicsEffect>
#include <Core/processor.h>
#include <Core/application.h>

namespace View {

class Processor : public QGraphicsObject {
    Q_OBJECT
public:
    Processor(int x, int y, Core::Processor* m);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public slots:
    void change();

protected:
    // TODO - add event handlers

private:
    int x;
    int y;
    bool over;

    Core::Processor *processor;

    QGraphicsDropShadowEffect *hoverEffect;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    void updateToolTip();
};

} // namespace View

#endif // VIEW_MPSOC_H
