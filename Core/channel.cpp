#include "channel.h"

Core::Channel::Channel(int ax, int ay, int bx, int by, QObject *parent)
    : QObject(parent)
    , ax(ax)
    , ay(ay)
    , bx(bx)
    , by(by)
{

}

void Core::Channel::add(AppLoad *load) {
    connect(load, SIGNAL(removeMe(AppLoad*)), this, SLOT(removeLoad(AppLoad*)));
    loads.append(load);

    emit loadChanged(val());
    if (val() > 100) {
        emit overloaded();
    }
}

qreal Core::Channel::val() const {
    qreal val = 0.0;
    for (AppLoad *load : loads) {
        val += load->getLoad();
    }
    return val;
}

void Core::Channel::reset() {
    loads.clear();
    emit loadChanged(0);
}

int Core::Channel::getBy() const {
    return by;
}

int Core::Channel::getBx() const {
    return bx;
}

int Core::Channel::getAy() const {
    return ay;
}

int Core::Channel::getAx() const {
    return ax;
}

void Core::Channel::removeLoad(Core::AppLoad *load) {
    int i = loads.indexOf(load);
    if (i < 0) { // Already removed
        return;
    }
    loads.remove(i);
    emit loadChanged(val());
}
