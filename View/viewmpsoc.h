#ifndef VIEW_MPSOC_H
#define VIEW_MPSOC_H

#include <QObject>
#include <QGraphicsItem>

namespace View {

class MPSoC : public QGraphicsItem {
public:
    MPSoC();

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:

    // TODO - add event handlers
};

} // namespace View

#endif // VIEW_MPSOC_H
