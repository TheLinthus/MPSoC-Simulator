#include "appload.h"

namespace Core {

AppLoad::AppLoad(int volume, qreal load, QObject *parent)
    : QObject(parent),
      volume(volume),
      load(load)
{

}

int AppLoad::getVolume() const
{
    return volume;
}

qreal AppLoad::getLoad() const
{
    return load;
}

} // namespace Core
