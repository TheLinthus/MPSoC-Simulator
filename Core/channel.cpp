#include "channel.h"

Core::Channel::Channel(QObject *parent)
    : QObject(parent)
    , load(0)
{

}

void Core::Channel::add(qreal value)
{
    load += value;
    if (load < 0) load = 0;
    emit loadChanged(load);
    if (load > 100) {
        emit overloaded();
    }
}

qreal Core::Channel::val()
{
    return load;
}

void Core::Channel::reset()
{
    load = 0;
}
