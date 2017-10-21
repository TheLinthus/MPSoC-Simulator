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

bool ApplicationController::addFromFile(const QString &path, const QString name, const QString groupName) {
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

        if (groupName.isEmpty()) {
            add(name,app);
        } else {
            add("[" + groupName + "] " + name,app);
        }

        return true;
    } else {
        return false;
    }
    return false;
}

void ApplicationController::saveToFile(QString name) {

}

void ApplicationController::add(QString name, Application *app) {
    if (applicationsList.contains(name)) {
        int n = 2;
        while (applicationsList.contains(name + " " + n)) {
            n++;
        }
        applicationsList.insert(name + " " + n, app);
    } else {
        applicationsList.insert(name, app);
    }
}

void ApplicationController::remove(QString name) {

}

QList<QString> ApplicationController::getApplicationsList() const {
    return applicationsList.keys();
}

Application *ApplicationController::getApplication(QString name) {
    return applicationsList.value(name);
}

Application *ApplicationController::getRunning(int index) {
    return runningList.at(index);
}

void ApplicationController::run(QString name) {

}

void ApplicationController::kill(int index) {

}

int ApplicationController::applicationsCount() {
    return applicationsList.count();
}

int ApplicationController::runningCount() {
    return runningList.count();
}

void ApplicationController::updateAvailabilityList() {
    QDir applicationsDir("Applications");
    applicationsDir.setNameFilters(QStringList()<<"*.json");

    int loadMax = applicationsDir.count();
    int loadValue = 0;

    emit progressMaxUpdate(loadMax);

    foreach (QString s, applicationsDir.entryList()) {
        emit progressUpdate(loadValue);
        QString text;
        QFile file("Applications\\"+s);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        text = file.readAll();
        file.close();

        loadValue++;

        QJsonDocument json = QJsonDocument::fromJson(text.toUtf8());
        if (json.isNull()) {
            qWarning() << "Error File";
            continue;
        }

        QJsonObject obj = json.object();
        QString appsName = obj.value("name").toString();
        QJsonArray array = obj.value("applications").toArray();

        loadMax += array.size();
        emit progressMaxUpdate(loadMax);

        for (int i = 0; i < array.size(); i++) {
            emit progressUpdate(loadValue);
            loadValue++;
            addFromFile(array.at(i).toObject().value("file").toString(), array.at(i).toObject().value("name").toString(), appsName);
        }
    }

    emit progressUpdate(loadMax);
    emit updateDone();
}

} // namespace Core
