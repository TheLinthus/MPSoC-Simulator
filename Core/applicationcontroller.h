#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <Core/application.h>

#define apps Core::ApplicationController::instance()

namespace Core {

class ApplicationController : public QObject
{
    Q_OBJECT

private:
    ApplicationController(QObject* parent = 0);
    static ApplicationController* createInstance();

    QMap<QString, Core::ApplicationGroup *> applicationsGroupList;
    QList<Core::Application *> runningList;

    QString getFileVersion(int v) const;

    enum ApplicationFileVersion {
        Basic
    };

public:
    ~ApplicationController();
    static ApplicationController* instance();

    Application *readAppFromFile(const QString &path, const QString name);
    bool saveToFile(ApplicationGroup *group);
    void add(ApplicationGroup *group);
    QStringList getApplicationsList(const QString &group = "") const;
    QStringList getApplicationsGroupList() const;
    Core::Application *getApplication(QString name);
    Core::Application *getRunning(int index);
    ApplicationGroup *getApplicationGroup(QString name);
    bool run(QString name);
    void kill(int index);
    int applicationsCount();
    int runningCount();

public slots:
    void updateAvailabilityList();

signals:
    void progressUpdate(int);
    void progressMaxUpdate(int);
    void updateDone(QStringList,QStringList);
};

} // namespace Core

#endif // APPLICATIONCONTROLLER_H
