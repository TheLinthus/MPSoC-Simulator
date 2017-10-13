#include "application.h"

namespace Core {

Application::Application(QObject *parent)
    : QObject(parent)
{
}

void Application::addNode(int index, int lifespan) {
    // If application already exists, doesn't create new one
    if (!exists(index)) {
        nodes.insert(index, new AppNode(lifespan, this));
    }
}

void Application::addNodeConnection(int from, int to, int volume, qreal load) {
    // Set Node's lifespan to max Load Data Volume linked or let it be if greater than
    // Lifespan can't be lesser than data volume
    if (volume > getNode(from)->getLifespan())
        getNode(from)->setLifespan(volume);
    if (volume > getNode(to)->getLifespan())
        getNode(to)->setLifespan(volume);

    // Create connection
    if (connections.find(from) == connections.end()) {
        connections.insert(from, new QMap<int, AppLoad *>());
    }

    Core::AppLoad * appLoad = new AppLoad(load, volume, this);
    connections.value(from)->insert(to, appLoad);
}

AppNode *Application::getNode(int index) {
    return nodes.value(index);
}

bool Application::exists(int index) {
    return nodes.find(index) != nodes.end();
}

void Application::removeNode(int index) {
    nodes.remove(index);
}

QColor Application::getColor() {
    return color;
}

void Application::setColor(QColor color) {
    this->color = color;
}

} // namespace Core
