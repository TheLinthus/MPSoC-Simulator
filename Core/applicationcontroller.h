#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include "core/application.h"

#define apps Core::ApplicationController::instance()

namespace Core {

class ApplicationController : public QObject
{
    Q_OBJECT

private:
    ApplicationController(QObject* parent = 0);
    static ApplicationController* createInstance();

    QList<Core::Application *> applicationsList;
    QList<Core::Application *> runningList;

public:
    ~ApplicationController();
    static ApplicationController* instance();

    bool addFromFile(const QString &path);
    void saveToFile(int index);
    void add(Core::Application app);
    void remove(int index);
    Core::Application *getApplication(int index);
    Core::Application *getRunning(int index);
    void run(int index);
    void kill(int index);
    int applicationsCount();
    int runningCount();

signals:

public slots:
};

} // namespace Core

#endif // APPLICATIONCONTROLLER_H
