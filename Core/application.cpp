#include "application.h"

namespace Core {

Application::Application(AppNode *root, QObject *parent)
    : QObject(parent)
{
    nodes.append(root);
}

int Application::addNode(AppNode *node)
{
    int size = nodes.size();
    nodes.append(node);
    return size;
}

void Application::addNodeConnection(qreal load, int from, int to)
{
    if (from < 0) {
        qCritical() << "int from(" << from << ") out of index array (<0)";
        return;
    }
    if (to > nodes.size()) {
        qCritical() << "int to(" << to << ") out of index array (>" << nodes.size() << ")";
        return;
    }

    AppLoad *appLoad = new AppLoad();

}

} // namespace Core
