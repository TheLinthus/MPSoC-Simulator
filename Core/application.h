#ifndef CORE_APPLICATION_H
#define CORE_APPLICATION_H

#include <QObject>
#include <QVector>
#include <QDebug>
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

    QString getName() const;
    void setName(const QString &value);

    Application *clone(QObject *parent);

protected:
    QColor color;
    QString name;

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

} // namespace Core

#endif // CORE_APPLICATION_H
