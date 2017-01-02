#include "viewmpsoc.h"

namespace View {

MPSoC::MPSoC() {
    int a = 10 + 80;
}

QRectF MPSoC::boundingRect() const {
    return QRectF(0,0,20,20);
}

void MPSoC::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

} // namespace View
