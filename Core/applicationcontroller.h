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
#define COLORS_COUNT 18

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
    
    QColor defaultColors[COLORS_COUNT] = {
        QColor(255, 0, 0, 255),
        QColor(0, 255, 0, 255),
        QColor(0, 0, 255, 255),
        QColor(255, 255, 0, 255),
        QColor(255, 0, 255, 255),
        QColor(64, 255, 255, 255),
        QColor(255, 154, 0, 255),
        QColor(0, 255, 154, 255),
        QColor(123, 0, 191, 255),
        QColor(224, 96, 123, 255),
        QColor(191, 255, 0, 255),
        QColor(0, 191, 255, 255),
        QColor(123, 0, 15, 255),
        QColor(0, 155, 0, 255),
        QColor(0, 0, 123, 255),
        QColor(96, 63, 0, 255),
        QColor(63, 0, 63, 255),
        QColor(31, 96, 63, 255)
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
