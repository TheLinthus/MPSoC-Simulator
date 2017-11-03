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

bool ApplicationController::addAppFromFile(const QString &path, const QString name, ApplicationGroup *group) {
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

bool ApplicationController::saveToFile(ApplicationGroup *group) {
    QDir applicationsDir("Applications");
    QString fileName;
    if (group->getFile().isEmpty()) {               // Check for existing files to create a new file name
        QString appendix = "";
        fileName = group->getName() + appendix + ".json";
        int i = 1;
        applicationsDir.setNameFilters(QStringList(fileName));
        while (applicationsDir.count() != 0) {
            i++;
            appendix = " " + QString::number(i);
            fileName = group->getName() + appendix + ".json";
            applicationsDir.setNameFilters(QStringList(fileName));
        }
        group->setFile(applicationsDir.relativeFilePath(fileName));
    }

    QFile file(applicationsDir.absolutePath() + "/" + group->getFile());
    // Qt auto create files if doesn't exist on open write
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Error saving file" << file.fileName() << ", could not open:" << file.errorString();
        return false;
    }

    QJsonObject groupObj;

    groupObj["name"] = group->getName();
    groupObj["author"] = group->getAuthor();
    groupObj["date"] = group->getDate().toString(Qt::ISODate);
    groupObj["enabled"] = group->isEnabled();

    QJsonArray array;
    foreach (QString item, group->getApplicationsList()) {
        Application *app = getApplication(item);
        QJsonObject appObj;

        appObj["name"] = app->getName(false);   // Get app name without parent's name
        appObj["file"] = app->getFile();        // Get stored file path

        array.append(appObj);
    }
    groupObj["applications"] = array;

    QJsonDocument save(groupObj);

    if (!file.write(save.toJson())) {
        qWarning() << "Error saving file" << file.fileName() << ", could not be writen:" << file.errorString();
        return false;
    }
    file.close();

    emit updateDone(getApplicationsList(),getApplicationsGroupList());
    return true;
}

void ApplicationController::add(Application *app, ApplicationGroup *group) {
    group->add(app);
    applicationsList.insert(app->getName(), app);
}

void ApplicationController::add(ApplicationGroup *group) {
    QString appendix = "", name = group->getName() + appendix;
    int i = 1;
    while (applicationsGroupList.contains(name)) {
        i++;
        appendix = " " + QString::number(i);
        name = group->getName() + appendix;
    }
    group->setName(name);
    applicationsGroupList.insert(name, group);
}

QStringList ApplicationController::getApplicationsList(const QString &group) const {
    if (group != 0) {
        if (applicationsGroupList.contains(group)) {
            return applicationsGroupList.value(group)->getApplicationsList();
        }
        return QStringList();
    }
    QStringList list;
    foreach (ApplicationGroup *group, applicationsGroupList) {
        if (group->isEnabled()) {
            list << group->getApplicationsList();
        }
    }
    return list;
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
        group->setFile(applicationsDir.relativeFilePath(file.fileName()));

        add(group);

        QJsonArray array = obj.value("applications").toArray();

        loadMax += array.size();
        emit progressMaxUpdate(loadMax);

        for (int i = 0; i < array.size(); i++) {
            emit progressUpdate(loadValue);
            loadValue++;
            addAppFromFile(
                        applicationsDir.absoluteFilePath(array.at(i).toObject().value("file").toString()),
                        array.at(i).toObject().value("name").toString(),
                        group);
        }
    }

    emit progressUpdate(loadMax);
    emit updateDone(getApplicationsList(),getApplicationsGroupList());
}

} // namespace Core
