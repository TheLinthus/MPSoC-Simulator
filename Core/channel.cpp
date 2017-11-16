#include "channel.h"

Core::Channel::Channel(QObject *parent)
    : QObject(parent)
    , loadAtB(0)
    , loadBtA(0)
{

}

void Core::Channel::add(qreal value, bool AtoB) {
    qreal load;
    if (AtoB) load = loadAtB;
    else load = loadBtA;
    load += value;
    if (load < 0) {
        load = 0;
        qWarning() << "Load " << (AtoB ? "AtB" : "BtA") << " less than 0";
    }
    if (AtoB) loadAtB = load;
    else loadBtA = load;
    emit loadChanged(val());
    if (val() > 100) {
        emit overloaded();
    }
}

qreal Core::Channel::val() const {
    return loadAtB + loadBtA;
}

qreal Core::Channel::valAtB() const {
    return loadAtB;
}

qreal Core::Channel::valBtA() const {
    return loadBtA;
}

void Core::Channel::reset() {
    loadAtB = 0;
    loadBtA = 0;
}
