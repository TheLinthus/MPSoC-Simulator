#include "mpsoccontroller.h"
#include "singleton.h"

namespace Core {

Core::MPSoCController::MPSoCController(QObject *parent) :
    QObject(parent)
{

}

Core::MPSoCController *MPSoCController::createInstance() {
    return new MPSoCController();
}

Core::MPSoCController::~MPSoCController() {

}

Core::MPSoCController* MPSoCController::instance() {
    return Singleton<Core::MPSoCController>::instance(Core::MPSoCController::createInstance);
}

Core::MPSoC *MPSoCController::add(int h, int w) {
    mpsocList.append(new MPSoC(h, w, this));
    return mpsocList.last();
}

int MPSoCController::count() const {
    return mpsocList.count();
}

void MPSoCController::remove(Core::MPSoC *mpsoc) {
    mpsocList.removeOne(mpsoc);
    delete mpsoc;
}

} // namespace Core
