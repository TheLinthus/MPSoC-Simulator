#include "heuristic.h"

namespace Core {

Heuristic::Heuristic(QObject *parent) :
    QObject(parent),
    enabled(true),
    name("No name"),
    description(""),
    author("Unknow")
{

}

QString Heuristic::getFileName() const {
    return fileName;
}

void Heuristic::setFileName(const QString &value) {
    fileName = value;
}

QString Heuristic::getName() const {
    return name;
}

void Heuristic::setName(const QString &value) {
    name = value;
}

QString Heuristic::getDescription() const {
    return description;
}

void Heuristic::setDescription(const QString &value) {
    description = value;
}

QString Heuristic::getAuthor() const {
    return author;
}

void Heuristic::setAuthor(const QString &value) {
    author = value;
}

void Heuristic::setEngine(QScriptEngine *value) {
    engine = value;
    enabled = true;
}

QScriptEngine *Heuristic::getEngine() const {
    return engine;
}

void Heuristic::disableEngine() {
    delete engine;
    enabled = false;
}

bool Heuristic::isEngineEnabled() {
    return enabled;
}

int Heuristic::selectCore(QPoint &p, QScriptValueList &args) const {
    if (enabled) {
        QScriptValue fun = engine->globalObject().property("selectCore");   // Select Core Function on Heuristic Script
        QScriptValue result = fun.call(QScriptValue(), args);               // Call the heuristic script function to selecting a Core
        if (engine->hasUncaughtException()) {
            int line = engine->uncaughtExceptionLineNumber();
            qDebug() << "uncaught exception at line" << line << ":" << result.toString();
            engine->clearExceptions();
            return 4; // Script error
        }
        if (result.isObject() && !result.isError()
                && result.property("x").isValid()
                && result.property("y").isValid()) {                        // If no error was found and the return is an object with X and Y, nothing is wrong with the script
            p.setX(result.property("x").toInteger());                       // Extract the points X and Y from the return
            p.setY(result.property("y").toInteger());
            return 1; // Ok
        } else {
            qDebug() << result.toString();
            return 3; // Bad Heuristic Implementation
        }
    } else {
        return 2; // Script not enabled
    }
}

} // namespace Core
