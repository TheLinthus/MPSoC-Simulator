#include "applicationcontroller.h"
#include "singleton.h"

#define PARENT_INDEX rx.cap(i + 1).toInt()
#define CHILD_INDEX rx.cap(i + 2).toInt()
#define PARENT_TO_CHILD_VOLUME rx.cap(i + 3).toInt()
#define PARENT_TO_CHILD_LOAD rx.cap(i + 4).toDouble()
#define CHILD_TO_PARENT_VOLUME rx.cap(i + 5).toInt()
#define CHILD_TO_PARENT_LOAD rx.cap(i + 6).toDouble()

namespace Core {

Core::ApplicationController::ApplicationController(QObject *parent) :
    QObject(parent)
{

}

Core::ApplicationController *ApplicationController::createInstance() {
    return new ApplicationController();
}

Core::ApplicationController::~ApplicationController() {

}

Core::ApplicationController* ApplicationController::instance() {
    return Singleton<Core::ApplicationController>::instance(Core::ApplicationController::createInstance);
}

bool ApplicationController::addFromFile(const QString &path) {
    QFile file(path);

    if (file.exists() &&  file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QRegExp rx("(\\d+)\\s*\\[\\s*(\\d+)\\s*,\\s*(\\d+)\\s*,\\s*(\\d+)\\s*,\\s*(\\d+)\\s*,\\s*(\\d+)\\s*\\]");
        rx.indexIn(file.readAll());

        file.close();

        Core::Application *app = new Application(this);

        for (int i = 0; i < rx.captureCount(); i+=7) {
            app->addNode(PARENT_INDEX, 0); // Add parent Node to application
            app->addNode(CHILD_INDEX, 0); // Add child Node to application

            app->addNodeConnection(PARENT_INDEX, CHILD_INDEX, PARENT_TO_CHILD_VOLUME, PARENT_TO_CHILD_LOAD);
            app->addNodeConnection(CHILD_INDEX, PARENT_INDEX, CHILD_TO_PARENT_VOLUME, CHILD_TO_PARENT_LOAD);
        }

        applicationsList.append(app);

        return true;
    } else {
        return false;
    }
    return false;
}

void ApplicationController::saveToFile(int index) {

}

void ApplicationController::add(Application app) {

}

void ApplicationController::remove(int index) {

}

Application *ApplicationController::getApplication(int index) {
    return applicationsList.at(index);
}

Application *ApplicationController::getRunning(int index) {
    return runningList.at(index);
}

void ApplicationController::run(int index) {

}

void ApplicationController::kill(int index) {

}

int ApplicationController::applicationsCount() {
    return applicationsList.count();
}

int ApplicationController::runningCount() {
    return runningList.count();
}

} // namespace Core
