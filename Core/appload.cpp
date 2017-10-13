#include "appload.h"

namespace Core {

AppLoad::AppLoad(int volume, qreal load, QObject *parent)
    : QObject(parent),
      volume(volume),
      load(load)
{

}

} // namespace Core
