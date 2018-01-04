#include "scriptconverter.h"

namespace Core {

ScriptConverter::ScriptConverter(QObject *parent) : QObject(parent) { }

QScriptValue ScriptConverter::toScriptValue(QScriptEngine *engine, AppNode *node) {
    QScriptValue obj = engine->newObject();

    if (node == 0) {
        obj.setProperty("free", true);
        return obj;
    }

    obj.setProperty("n", node->getN());
    obj.setProperty("cycles", node->getCycles());
    obj.setProperty("lifespan", node->getLifespan());
    obj.setProperty("appUID", qobject_cast<Core::Application *>(node->parent())->getUid());

    QScriptValue children = engine->newArray();
    for (int i = 0; i < node->getChildNodes().count(); i++) {
        children.setProperty(i, node->getChildNodes().value(i)->getN());
    }
    obj.setProperty("children", children);

    return obj;
}

QScriptValue ScriptConverter::toScriptValue(QScriptEngine *engine, Channel *channel) {
    if (channel == 0)
        return QScriptValue(QScriptValue::UndefinedValue);
    QScriptValue obj  = engine->newObject();

    obj.setProperty("ax", channel->getAx());
    obj.setProperty("ay", channel->getAy());
    obj.setProperty("bx", channel->getBx());
    obj.setProperty("by", channel->getBy());
    obj.setProperty("load", channel->val());

    return obj;
}

QScriptValue ScriptConverter::toScriptValue(QScriptEngine *engine, Core::Processor *processor) {
    if (processor == 0)
        return QScriptValue(QScriptValue::UndefinedValue);
    QScriptValue obj = engine->newObject();

    obj.setProperty("threadsCount", processor->nOfThreads());
    obj.setProperty("x", processor->getX());
    obj.setProperty("y", processor->getY());
    QScriptValue threads = engine->newArray();
    for (int i = 0; i < processor->nOfThreads(); i++) {
        threads.setProperty(i, toScriptValue(engine, processor->runningNode()));
    }
    obj.setProperty("threads", threads);

    QScriptValue north = engine->newObject();
    north.setProperty("in", toScriptValue(engine, processor->getChannel(In, North)));
    north.setProperty("out", toScriptValue(engine, processor->getChannel(Out, North)));

    QScriptValue south = engine->newObject();
    south.setProperty("in", toScriptValue(engine, processor->getChannel(In, South)));
    south.setProperty("out", toScriptValue(engine, processor->getChannel(Out, South)));

    QScriptValue west = engine->newObject();
    west.setProperty("in", toScriptValue(engine, processor->getChannel(In, West)));
    west.setProperty("out", toScriptValue(engine, processor->getChannel(Out, West)));

    QScriptValue east = engine->newObject();
    east.setProperty("in", toScriptValue(engine, processor->getChannel(In, East)));
    east.setProperty("out", toScriptValue(engine, processor->getChannel(Out, East)));

    obj.setProperty("north", north);
    obj.setProperty("south", south);
    obj.setProperty("west", west);
    obj.setProperty("east", east);

    return obj;
}

QScriptValue ScriptConverter::toScriptValue(QScriptEngine *engine, Application *app) {
    if (app == 0)
        return QScriptValue(QScriptValue::UndefinedValue);
    QScriptValue obj = engine->newObject();

    obj.setProperty("uid", app->getUid());
    obj.setProperty("name", app->getName());
    obj.setProperty("root", toScriptValue(engine, app->getRootNode()));

    QScriptValue allNodes = engine->newArray();
    QVector<AppNode *> nodes = app->getAllNodes();
    for (AppNode *node : nodes) {
        allNodes.setProperty(node->getN(), toScriptValue(engine, node));
    }
    obj.setProperty("nodes", allNodes);

    QScriptValue fromConnections = engine->newArray();
    QMap<int, QMap<int, AppLoad *> *> connections = app->getConnections();
    for (int i :  connections.keys()) {
        QScriptValue toConnections = engine->newArray();
        for (int j : connections.value(i)->keys()) {
            AppLoad *appLoad = connections.value(i)->value(j);
            toConnections.setProperty(j, toScriptValue(engine, appLoad));
        }
        fromConnections.setProperty(i, toConnections);
    }
    obj.setProperty("connections", fromConnections);

    return obj;
}

QScriptValue ScriptConverter::toScriptValue(QScriptEngine *engine, AppLoad *load) {
    if (load == 0)
        return QScriptValue(QScriptValue::UndefinedValue);
    QScriptValue obj = engine->newObject();

    obj.setProperty("load", load->getLoad());
    obj.setProperty("volume", load->getVolume());

    return obj;
}

QScriptValue ScriptConverter::toScriptValue(QScriptEngine *engine, MPSoC *mpsoc) {
    if (mpsoc == 0)
        return QScriptValue(QScriptValue::UndefinedValue);
    QScriptValue obj = engine->newObject();

    obj.setProperty("width", mpsoc->getWidth());
    obj.setProperty("height", mpsoc->getHeight());

    QScriptValue cores = engine->newArray(mpsoc->getHeight());
    for (int i = 0; i < mpsoc->getWidth(); i++) {
        QScriptValue column = engine->newArray(mpsoc->getWidth());
        for (int j = 0; j < mpsoc->getHeight(); j++) {
            column.setProperty(j, toScriptValue(engine, mpsoc->getCore(i,j)));
        }
        cores.setProperty(i, column);
    }
    obj.setProperty("cores", cores);

    return obj;
}

} // namespace Core
