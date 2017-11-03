#ifndef CORE_APPLICATION_H
#define CORE_APPLICATION_H

#include <QObject>
#include <QVector>
#include <QDate>
#include <QMap>
#include <QColor>
#include "appload.h"
#include "appnode.h"

namespace Core {

class Application : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Application A graph of an application to be scheduled in future emulation
     * @param parent
     */
    explicit Application(QObject *parent = 0);
    ~Application();

    /**
     * @brief addNode Add a new AppNode to the Application
     * @return
     */
    void addNode(int index, int lifespan);
    void addNodeConnection(const int from, int to, const int volume, const qreal load);

    AppNode *getNode(int index);
    AppLoad *getLoad(int from, int to);

    bool exists(int index);
    void removeNode(int index);
    QColor getColor();
    void setColor(QColor color);
    QString getName(bool withParent = true) const;
    void setName(const QString &value);
    QString getFile() const;
    void setFile(const QString &value);

    Application *clone(QObject *parent);

protected:
    QColor color;
    QString name;
    QString file;

private:
    QMap<int, AppNode *> nodes;
    QMap<int, QMap<int, AppLoad *> *> connections;

signals:

public slots:
};

class MasterApplication : public Application
{
    Q_OBJECT

public:
    explicit MasterApplication();
    ~MasterApplication();
};

class ApplicationGroup : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationGroup(QObject *parent = 0);
    ~ApplicationGroup();

    QString getFile() const;
    void setFile(const QString &value);
    QString getName() const;
    void setName(const QString &value);
    QString getAuthor() const;
    void setAuthor(const QString &value);
    QDate getDate() const;
    void setDate(const QDate &value);
    bool isEnabled() const;
    void setEnabled(bool value);
    QStringList getApplicationsList() const;
    int getApplicationsCount() const;
    void add(Application *app);
    void remove(const QString &app);
    Application *get(const QString &app);
    bool contains(const QString &app);

private:
    QString file;
    QString name;
    QString author;
    QDate date;
    bool enabled;

    QMap<QString, Application *> applicationsList;
};

} // namespace Core

#endif // CORE_APPLICATION_H
