#ifndef CORE_APPLICATION_H
#define CORE_APPLICATION_H

#include <QObject>
#include <QVector>
#include "appload.h"
#include "appnode.h"

namespace Core {

class Application : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Application A graph of an application to be scheduled in future emulation
     * @param root Root AppNode, to be the start point
     * @param parent
     */
    explicit Application(AppNode *root, QObject *parent = 0);

    /**
     * @brief addNode Add a new AppNode to the Application, and add a AppLoad betwen an existing AppNode
     * @param node The AppNode to be inserted in the Application
     * @param load The AppLoad betwen the added AppNode and the AppNode to append at
     * @param to Index of AppNode to append at
     * @return integer Index of inserted AppNode in the Application
     */
    int addNode(AppNode *node, AppLoad *load, int to);
    int indexOf(AppNode *node);
    QVector<AppNode *> connectedNodes();
    AppNode *getNode(int index);

    void removeNode(int index);

private:
    QVector<AppNode *> nodes;
    QVector<AppLoad *> packageTransmission;

signals:

public slots:
};

} // namespace Core

#endif // CORE_APPLICATION_H
