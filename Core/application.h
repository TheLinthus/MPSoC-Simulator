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

    void setRootNode(AppNode *node);
    void setRootNode(const int index, const int lifespan);
    bool addNode(const int index, const int lifespan, const int to = -1);
    void addNodeConnection(const int from, int to, const int volume, const qreal load);

    AppNode *getNode(const int index);
    AppNode *getRootNode();
    AppLoad *getLoad(const int from, const int to);
    int childCount(const int index) const;
    int connectionCount(int index) const;
    int graphWidth() const;
    int graphHeigh() const;
    QMap<int, AppLoad *> *getConnectionsFrom(const int from) const;
    QMap<int, QMap<int, AppLoad *> *> getConnections() const;
    bool exists(const int index);
    int nodeCount() const;
    void removeNode(const int index);
    QColor getColor();
    void setColor(const QColor color);
    QString getName(const bool withParent = true) const;
    void setName(const QString &value);
    QString getFile() const;
    void setFile(const QString &value);

    Application *clone(QObject *parent);


protected:
    QColor color;
    QString name;
    QString file;

private:
    AppNode *rootNode;
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
