#include "appnode.h"

namespace Core {

AppNode::AppNode(qreal lifespan, QColor *color, QObject *parent)
    : lifespan(lifespan)
    , color(color)
    , QObject(parent)
{

}

QColor *AppNode::getColor() {
    return color;
}

} // namespace Core
