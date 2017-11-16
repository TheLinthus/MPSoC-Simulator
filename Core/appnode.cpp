#include "appnode.h"

namespace Core {

AppNode::AppNode(int n, int lifespan, QObject *parent)
    : lifespan(lifespan)
    , cycles(0)
    , n(n)
    , childNodes()
    , siblingNodes()
    , parentNode(0)
    , QObject(parent)
    , inCoreX(-1)
    , inCoreY(-1)
    , inThread(-1)
    , running(false)
    , done(false)
{
}

AppNode::~AppNode() {
    emit finished(this);
    for (AppNode *child : childNodes) {
        delete child;
    }
}

bool AppNode::isDone() {
    return done;
}

int AppNode::getLifespan() const {
    return lifespan;
}

int AppNode::getCycles() const {
    return cycles;
}

int AppNode::getN() const {
    return n;
}

void AppNode::setParentNode(AppNode *node) {
    parentNode = node;
}

void AppNode::addSiblingNode(AppNode *node) {
    if (!siblingNodes.contains(node))
        siblingNodes << node;
}

void AppNode::addChildNode(AppNode *node) {
    if (!childNodes.contains(node)) {
        for (AppNode *child : childNodes) {
            child->addSiblingNode(node);    // Register the node to his siblings
            node->addSiblingNode(child);    // Register the siblings to the node
        }
        childNodes << node;
    }
}

AppNode *AppNode::getParentNode() const{
    return parentNode;
}

AppNode *AppNode::find(const int n) {
    if (n == this->n) {
        return this;
    }
    if (childNodes.isEmpty())
        return 0;
    for (AppNode *child : childNodes) {
        AppNode *returnNode = child->find(n);
        if (returnNode != 0)
            return returnNode;
    }
    return 0;
}

QList<AppNode *> AppNode::getSiblingNodes() const {
    return siblingNodes;
}

QList<AppNode *> AppNode::getChildNodes() const {
    return childNodes;
}

int AppNode::getWidth() const {
    if (childNodes.isEmpty())
        return 1;
    int width = 0;
    for (AppNode *child : childNodes) {
        width += child->getWidth();
    }
    return width;
}

int AppNode::getHeight() const {
    if (childNodes.isEmpty())
        return 1;
    int height = 1;
    for (AppNode *child : childNodes) {
        height = qMax(child->getHeight(), height);
    }
    return height + 1;
}

int AppNode::count() const {
    if (childNodes.isEmpty())
        return 1;
    int count = 1;
    for (AppNode *child : childNodes) {
        count += child->count();
    }
    return count;
}

AppNode *AppNode::clone(QObject *parent) {
    AppNode *clone = new AppNode(n, lifespan, parent);

    for (AppNode *child : childNodes) {
        clone->addChildNode(child->clone(parent));
    }

    return clone;
}

int AppNode::getInThread() const {
    return inThread;
}

void AppNode::setInThread(int value) {
    inThread = value;
}

int AppNode::getInCoreX() const
{
    return inCoreX;
}

void AppNode::setInCoreX(int value)
{
    inCoreX = value;
}

int AppNode::getInCoreY() const
{
    return inCoreY;
}

void AppNode::setInCoreY(int value)
{
    inCoreY = value;
}

bool AppNode::isRunning() const
{
    return running;
}

void AppNode::setRunning(bool value)
{
    running = value;
}

void AppNode::setLifespan(const int lifespan) {
    this->lifespan = lifespan;
}

void AppNode::tick() {
    if (!done && cycles < lifespan) {
        cycles++;
        emit changed();
        if (cycles == lifespan) {
            done = true;
            emit finished(this);
        }
    }
}

void AppNode::kill() {
    qDebug() << this << "was killed";
    done = true;
    emit finished(this);
}

void AppNode::killAll() {
    kill();
    for (AppNode *child : childNodes) {
        child->killAll();
    }
}

} // namespace Core
