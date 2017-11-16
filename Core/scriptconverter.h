#ifndef CORE_SCRIPTCONVERTER_H
#define CORE_SCRIPTCONVERTER_H

#include <QObject>
#include <QScriptEngine>
#include <Core/mpsoc.h>

namespace Core {

class ScriptConverter : public QObject
{
    Q_OBJECT
public:
    explicit ScriptConverter(QObject *parent = 0);

    static QScriptValue toScriptValue(QScriptEngine *engine, Processor *processor);
    static QScriptValue toScriptValue(QScriptEngine *engine, AppNode *node);
    static QScriptValue toScriptValue(QScriptEngine *engine, Channel *channel);
    static QScriptValue toScriptValue(QScriptEngine *engine, Application *app);
    static QScriptValue toScriptValue(QScriptEngine *engine, AppLoad *load);
    static QScriptValue toScriptValue(QScriptEngine *engine, MPSoC *mpsoc);

signals:

public slots:
};

} // namespace Core

#endif // CORE_SCRIPTCONVERTER_H
