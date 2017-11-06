#include "application.h"

namespace Core {

Application::Application(QObject *parent)
    : QObject(parent),
      file("")
{
}

Application::~Application() {
    qDeleteAll(nodes);
    qDeleteAll(connections);
}

void Application::addNode(int index, int lifespan) {
    // If node already exists, doesn't create new one
    if (!exists(index)) {
        nodes.insert(index, new AppNode(lifespan, this));
    }
}

void Application::addNodeConnection(const int from, int to, const int volume, const qreal load) {
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

    Core::AppLoad *appLoad = new AppLoad(load, volume, this);
    connections.value(from)->insert(to, appLoad);
}

AppNode *Application::getNode(int index) {
    return nodes.value(index);
}

AppLoad *Application::getLoad(int from, int to) {
    if (connections.contains(from) && connections.value(from)->contains(to)) {
        return connections.value(from)->value(to);
    } else {
        return 0;
    }
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

QString Application::getName(bool withParent) const {
    if (withParent && this->parent() != 0) {
        return QString("[%1] %2").arg(((ApplicationGroup *) parent())->getName(), name);
    }
    return name;
}

void Application::setName(const QString &value)
{
    name = value;
}

Application *Application::clone(QObject *parent) {
    Application * clone = new Application(parent);

    clone->setColor(QColor(color));
    clone->setName(QString(name));
    for (int i = 0; i < nodes.size(); i++) {
        clone->addNode(i, nodes.value(i)->getLifespan());
    }
    for (int i = 0; i < connections.size(); i++) {
        for (int j = 0; j < connections.value(i)->size(); j++) {
            AppLoad *appload = connections.value(i)->value(j);
            clone->addNodeConnection(i, j, appload->getVolume(), appload->getLoad());
        }
    }

    return clone;
}

QString Application::getFile() const {
    return file;
}

void Application::setFile(const QString &value) {
    file = value;
}

MasterApplication::MasterApplication()
    : Application() {
    color = QColor(Qt::blue);
    name = (tr("Master"));
}

MasterApplication::~MasterApplication() {}

ApplicationGroup::ApplicationGroup(QObject *parent)
    : QObject(parent),
      file("")
{}

ApplicationGroup::~ApplicationGroup() { }

QString ApplicationGroup::getFile() const {
    return file;
}

void ApplicationGroup::setFile(const QString &value) {
    file = value;
}

QString ApplicationGroup::getName() const {
    return name;
}

void ApplicationGroup::setName(const QString &value) {
    name = value;
}

QString ApplicationGroup::getAuthor() const {
    return author;
}

void ApplicationGroup::setAuthor(const QString &value) {
    author = value;
}

QDate ApplicationGroup::getDate() const {
    return date;
}

void ApplicationGroup::setDate(const QDate &value) {
    date = value;
}

bool ApplicationGroup::isEnabled() const {
    return enabled;
}

void ApplicationGroup::setEnabled(bool value) {
    enabled = value;
}

QStringList ApplicationGroup::getApplicationsList() const {
    return applicationsList.keys();
}

int ApplicationGroup::getApplicationsCount() const {
    return applicationsList.count();
}

void ApplicationGroup::add(Application *app) {
    app->setParent(this);
    if (contains(app->getName())) {
        int n = 2;
        while (contains(app->getName() + " " + n)) {
            n++;
        }
        QString newName = app->getName() + " " + n;
        app->setName(newName);
        applicationsList.insert(newName, app);
    } else {
        applicationsList.insert(app->getName(), app);
    }
}

void ApplicationGroup::remove(const QString &app) {
    applicationsList.remove(app);
}

Application *ApplicationGroup::get(const QString &app) {
    return applicationsList.value(app);
}

bool ApplicationGroup::contains(const QString &app) {
    return applicationsList.contains(app);
}

} // namespace Core
