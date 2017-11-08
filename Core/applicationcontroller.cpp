#include "applicationcontroller.h"
#include "singleton.h"

#define PARENT_INDEX match.captured(1).toInt()
#define CHILD_INDEX match.captured(2).toInt()
#define PARENT_TO_CHILD_VOLUME match.captured(3).toInt()
#define PARENT_TO_CHILD_LOAD match.captured(4).toDouble()
#define CHILD_TO_PARENT_VOLUME match.captured(5).toInt()
#define CHILD_TO_PARENT_LOAD match.captured(6).toDouble()

#ifdef Q_OS_MAC
#define APPLICATIONSPATH "../Resources/Applications"
#else
#define APPLICATIONSPATH "../Applications"
#endif

namespace Core {

Core::ApplicationController::ApplicationController(QObject *parent) :
    QObject(parent),
    applicationsDir(QApplication::applicationDirPath()) {
    applicationsDir.cd(APPLICATIONSPATH);
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

Core::Application* ApplicationController::readAppFromFile(const QString &path, const QString name) {
    QFile file(applicationsDir.absoluteFilePath(path));

    if (file.exists() &&  file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QRegularExpression rx(getFileVersion(Basic));
        QRegularExpressionMatchIterator it = rx.globalMatch(file.readAll());

        file.close();

        Core::Application *app = new Core::Application();

        if (!it.hasNext()) {
            qWarning() << "File " << path << " doesn't have valid Application data";
            return 0;
        }

        bool root = false;
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            if (!root) {
                app->addNode(PARENT_INDEX, 0); // Add parent as Root
                root = true;
            }
            if (!app->addNode(CHILD_INDEX, 0, PARENT_INDEX)) {
                qWarning() << "Application " << name << " have inconsistent Application data (assignment to undefined node)";
            }

            app->addNodeConnection(PARENT_INDEX, CHILD_INDEX, PARENT_TO_CHILD_VOLUME, PARENT_TO_CHILD_LOAD);
            app->addNodeConnection(CHILD_INDEX, PARENT_INDEX, CHILD_TO_PARENT_VOLUME, CHILD_TO_PARENT_LOAD);
        }

        app->setName(name);
        app->setFile(path);

        return app;
    } else {
        qWarning() << "File " << path << " doesn't exist or can't be open";
        return 0;
    }
    return 0;
}

bool ApplicationController::saveToFile(ApplicationGroup *group) {
    QString fileName;
    QFile *file;
    if (group->getFile().isEmpty()) {               // Check for existing files to create a new file name
        QString appendix = "";
        fileName = group->getName() + appendix + ".json";
        int i = 1;
        file = new QFile(applicationsDir.absoluteFilePath(fileName));
        while (file->exists()) {
            i++;
            appendix = " " + QString::number(i);
            fileName = group->getName() + appendix + ".json";
            file->deleteLater();
            file = new QFile(applicationsDir.absoluteFilePath(fileName));
        }
        group->setFile(applicationsDir.relativeFilePath(fileName));
    } else {
        file = new QFile(applicationsDir.absoluteFilePath(group->getFile()));
    }

    // Qt auto create files if doesn't exist on open write
    if (!file->open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Error saving file" << file->fileName() << ", could not open:" << file->errorString();
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

    if (!file->write(save.toJson())) {
        qWarning() << "Error saving file" << file->fileName() << ", could not be writen:" << file->errorString();
        return false;
    }
    file->close();

    emit updateDone(getApplicationsList(),getApplicationsGroupList());
    return true;
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

    emit updateDone(getApplicationsList(),getApplicationsGroupList());
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
    foreach (ApplicationGroup *group, applicationsGroupList) {
        if (group->isEnabled()) {
            Application *app = group->get(name);
            if (app != 0) {
                return app;
            }
        }
    }
    return 0;
}

Application *ApplicationController::getRunning(int index) {
    return runningList.at(index);
}

ApplicationGroup *ApplicationController::getApplicationGroup(QString name) {
    return applicationsGroupList.value(name);
}

bool ApplicationController::run(QString name) {
    foreach (ApplicationGroup *group, applicationsGroupList) {
        if (group->isEnabled()) {
            Application *app = group->get(name);
            if (app != 0) {
                runningList.append(app->clone(this));
                return true;
            }
        }
    }
    return false;
}

void ApplicationController::kill(int index) {
    runningList.at(index)->deleteLater();
}

int ApplicationController::applicationsCount() {
    int count = 0;
    foreach (ApplicationGroup *group, applicationsGroupList) {
        if (group->isEnabled()) {
            count += group->getApplicationsCount();
        }
    }
    return count;
}

int ApplicationController::runningCount() {
    return runningList.count();
}

void ApplicationController::updateAvailabilityList() {
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
            Core::Application *app = readAppFromFile(
                        applicationsDir.relativeFilePath(array.at(i).toObject().value("file").toString()),
                        array.at(i).toObject().value("name").toString());
            if (app)
                group->add(app);
        }
    }

    emit progressUpdate(loadMax);
    emit updateDone(getApplicationsList(),getApplicationsGroupList());
}

} // namespace Core
