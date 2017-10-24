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

    QMap<QString, Core::Application *> applicationsList;
    QList<Core::Application *> runningList;

public:
    ~ApplicationController();
    static ApplicationController* instance();

    bool addFromFile(const QString &path, const QString name, const QString groupName = "");
    void saveToFile(QString name);
    void add(QString name, Application *app);
    void remove(QString name);
    QStringList getApplicationsList() const;
    Core::Application *getApplication(QString name);
    Core::Application *getRunning(int index);
    void run(QString name);
    void kill(int index);
    int applicationsCount();
    int runningCount();

public slots:
    void updateAvailabilityList();

signals:
    void progressUpdate(int);
    void progressMaxUpdate(int);
    void updateDone();
};

} // namespace Core

#endif // APPLICATIONCONTROLLER_H
