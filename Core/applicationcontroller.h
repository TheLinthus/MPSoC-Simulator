#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include <QObject>
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRegularExpression>
#include <Core/application.h>

#define apps Core::ApplicationController::instance()

namespace Core {

class ApplicationController : public QObject
{
    Q_OBJECT

private:
    ApplicationController(QObject* parent = 0);
    static ApplicationController* createInstance();

    int appUIDSequence;

    QMap<QString, Core::ApplicationGroup *> applicationsGroupList;
    QVector<Core::Application *> runningList;

    QDir applicationsDir;

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
    QStringList getRunningApplicationsList() const;
    QStringList getApplicationsGroupList() const;
    Core::Application *getApplication(QString name);
    Core::Application *getRunning(int index);
    ApplicationGroup *getApplicationGroup(QString name);
    Application *run(QString name);
    void kill(int index);
    void clearRunning();
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
