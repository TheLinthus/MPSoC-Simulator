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

QString ApplicationController::getFileVersion(int v) const {
    switch (v) {
        case ApplicationFileVersion::Basic:
        default:
            return QString("(\\d+)\\s*\\[\\s*(\\d+)\\s*,\\s*(\\d+)\\s*,\\s*(\\d+)\\s*,\\s*(\\d+)\\s*,\\s*(\\d+)\\s*\\]");
    }
    return QString();
}

Core::ApplicationController::~ApplicationController() {

}

Core::ApplicationController* ApplicationController::instance() {
    return Singleton<Core::ApplicationController>::instance(Core::ApplicationController::createInstance);
}

bool ApplicationController::addFromFile(const QString &path, const QString name, ApplicationGroup *group) {
    QFile file(path);

    if (file.exists() &&  file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QRegExp rx(getFileVersion(Basic));
        rx.indexIn(file.readAll());

        file.close();

        Core::Application *app = new Core::Application(group);

        for (int i = 0; i < rx.captureCount(); i+=7) {
            app->addNode(PARENT_INDEX, 0); // Add parent Node to application
            app->addNode(CHILD_INDEX, 0); // Add child Node to application

            app->addNodeConnection(PARENT_INDEX, CHILD_INDEX, PARENT_TO_CHILD_VOLUME, PARENT_TO_CHILD_LOAD);
            app->addNodeConnection(CHILD_INDEX, PARENT_INDEX, CHILD_TO_PARENT_VOLUME, CHILD_TO_PARENT_LOAD);
        }

        app->setName(name);
        add(app, group);

        return true;
    } else {
        qWarning() << "File " << path << " doesn't exist or can't be open";
        return false;
    }
    return false;
}

void ApplicationController::saveToFile(QString name) {

}

void ApplicationController::add(Application *app, ApplicationGroup *group) {
    group->add(app);
    applicationsList.insert(app->getName(), app);
}

QStringList ApplicationController::getApplicationsList(const QString &group) const {
    if (group != 0) {
        if (applicationsGroupList.contains(group)) {
            return applicationsGroupList.value(group)->getApplicationsList();
        }
        return QStringList();
    }
    return QStringList(applicationsList.keys());
}

QStringList ApplicationController::getApplicationsGroupList() const {
    return QStringList(applicationsGroupList.keys());
}

Application *ApplicationController::getApplication(QString name) {
    return applicationsList.value(name);
}

Application *ApplicationController::getRunning(int index) {
    return runningList.at(index);
}

ApplicationGroup *ApplicationController::getApplicationGroup(QString name) {
    return applicationsGroupList.value(name);
}

void ApplicationController::run(QString name) {
    runningList.append(applicationsList.value(name)->clone(this));
}

void ApplicationController::kill(int index) {
    runningList.at(index)->deleteLater();
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
        QFile file(applicationsDir.absoluteFilePath(s));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Could not open file" << file.fileName();
            loadValue++;
            continue;
        }
        text = file.readAll();
        file.close();

        loadValue++;

        QJsonDocument json = QJsonDocument::fromJson(text.toUtf8());
        if (json.isEmpty() || json.isNull()) {
            qWarning() << "File " << file.fileName() << " doesn't have a valid Json";
            continue;
        }

        QJsonObject obj = json.object();

        ApplicationGroup *group = new ApplicationGroup(this);

        group->setName(obj.value("name").toString());
        group->setAuthor(obj.value("author").toString());
        group->setDate(QDate::fromString(obj.value("date").toString(), Qt::ISODate));
        group->setEnabled(obj.value("enabled").toBool());
        group->setFile(file.fileName());

        applicationsGroupList.insert(group->getName(), group);

        QJsonArray array = obj.value("applications").toArray();

        loadMax += array.size();
        emit progressMaxUpdate(loadMax);

        for (int i = 0; i < array.size(); i++) {
            emit progressUpdate(loadValue);
            loadValue++;
            addFromFile(
                        applicationsDir.absoluteFilePath(array.at(i).toObject().value("file").toString()),
                        array.at(i).toObject().value("name").toString(),
                        group);
        }
    }

    emit progressUpdate(loadMax);
    emit updateDone();
}

} // namespace Core
