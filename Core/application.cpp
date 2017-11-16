#include "application.h"

namespace Core {

Application::Application(QObject *parent)
    : QObject(parent),
      file(""),
      rootNode(0),
      alive(true)
{
}

Application::~Application() {
    delete rootNode;
    qDeleteAll(connections);
}

void Application::setRootNode(AppNode *node) {
    if (rootNode != 0)
        rootNode->deleteLater();
    node->setParent(this);
    rootNode = node;
}

void Application::setRootNode(const int index, const int lifespan) {
    if (rootNode != 0) {
        rootNode->deleteLater();
    }
    rootNode = new AppNode(index, lifespan, this);
}

bool Application::addNode(const int index, const int lifespan, const int to) {
    if (to == -1) {
        setRootNode(index, lifespan);
        return true;
    }
    if (rootNode == 0)
        return false;         // Can't add to without initilized root
    if (rootNode->find(index)) {   // It is already added
        return true;
    } else {
        AppNode *parentNode = rootNode->find(to);
        if (parentNode) { // Can't add if parent node not found in
            parentNode->addChildNode(new AppNode(index, lifespan, this));
            return true;
        }
    }
    return false;
}

void Application::addNodeConnection(const int from, int to, const int volume, const qreal load) {
    // Set Node's lifespan to max Load Data Volume linked or let it be if greater than
    // Lifespan can't be lesser than data volume
    if (volume > getNode(from)->getLifespan())
        getNode(from)->setLifespan(volume);
    if (volume > getNode(to)->getLifespan())
        getNode(to)->setLifespan(volume);

    // Create connection
    if (connections.value(from) == 0) {
        connections.insert(from, new QMap<int, AppLoad *>());
    }

    Core::AppLoad *appLoad = new AppLoad(load, volume, this);
    connections.value(from)->insert(to, appLoad);
}

AppNode *Application::getNode(const int index) {
    return rootNode->find(index);
}

AppNode *Application::getRootNode() {
    return rootNode;
}

AppLoad *Application::getLoad(const int from, const int to) {
    if (connections.contains(from) && connections.value(from)->contains(to)) {
        return connections.value(from)->value(to);
    } else {
        return 0;
    }
}

int Application::graphWidth() const {
    return rootNode->getWidth();
}

int Application::graphHeigh() const {
    return rootNode->getHeight();
}

int Application::childCount(const int index) const {
    return rootNode->find(index)->count() - 1;
}

int Application::connectionCount(int index) const {
    return connections.values(index).count();
}

QMap<int, AppLoad *> *Application::getConnectionsFrom(const int from) const {
    return connections.value(from);
}

bool Application::exists(const int index) {
    return rootNode->find(index) != 0;
}

int Application::nodeCount() const {
    return rootNode->count();
}

void Application::removeNode(const int index) {
    rootNode->find(index)->deleteLater();
}

QColor Application::getColor() {
    return color;
}

void Application::setColor(const QColor color) {
    this->color = color;
}

QString Application::getName(const bool withParent) const {
    if (withParent && this->parent() != 0) {
        return QString("[%1] %2").arg(((ApplicationGroup *) parent())->getName(), name);
    }
    return name;
}

void Application::setName(const QString &value)
{
    name = value;
}

Application *Application::clone() {
    Application * clone = new Application();

    connect(this, SIGNAL(destroyed(QObject*)), clone, SLOT(deleteLater()));
    connect(this, SIGNAL(wasKilled()), clone, SLOT(kill()));

    clone->setColor(QColor(color));
    clone->setName(QString(getName()));
    clone->setRootNode(rootNode->clone(clone));
    clone->setUid(uid);

    for (int i :  connections.keys()) {
        for (int j : connections.value(i)->keys()) {
            AppLoad *appload = connections.value(i)->value(j);
            clone->addNodeConnection(i, j, appload->getVolume(), appload->getLoad());
        }
    }

    return clone;
}

int Application::getUid() const {
    return uid;
}

void Application::setUid(int value) {
    uid = value;
}

bool Application::isAlive() {
    return alive;
}

void Application::kill() {
    alive = false;
    rootNode->killAll();
    emit wasKilled();
}

QMap<int, QMap<int, AppLoad *> *> Application::getConnections() const {
    return connections;
}

QVector<AppNode *> Application::getAllNodes(AppNode *from) const {
    QVector<AppNode *> nodes;
    if (from == 0) {
        from = rootNode;
    }
    nodes << from;
    for (AppNode *child : from->getChildNodes()) {
        nodes += getAllNodes(child);
    }
    return nodes;
}

QString Application::getFile() const {
    return file;
}

void Application::setFile(const QString &value) {
    file = value;
}

MasterApplication::MasterApplication()
    : Application() {
    color = QColor::fromRgb(0xff,0xae,0x2d);
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
