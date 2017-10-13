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

    /**
     * @brief addNode Add a new AppNode to the Application
     * @return
     */
    void addNode(int index, int lifespan);
    void addNodeConnection(int from, int to, int volume, qreal load);

    AppNode *getNode(int index);
    bool exists(int index);
    void removeNode(int index);

    QColor getColor();
    void setColor(QColor color);

private:
    QColor color;

    QMap<int, AppNode *> nodes;
    QMap<int, QMap<int, AppLoad *> *> connections;

signals:

public slots:
};

} // namespace Core

#endif // CORE_APPLICATION_H
