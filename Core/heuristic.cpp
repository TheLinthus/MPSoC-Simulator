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

int Heuristic::selectCore(QPoint &p, QScriptValueList &args) {
    if (enabled) {
        QScriptValue fun = engine->globalObject().property("selectCore");   // Select Core Function on Heuristic Script
        QScriptValue result = fun.call(QScriptValue(), args);               // Call the heuristic script function to selecting a Core
        if (engine->hasUncaughtException()) {
            int line = engine->uncaughtExceptionLineNumber();
            this->log(QString("uncaught exception at line %1: %2").arg(QString::number(line), result.toString()));
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
            this->log(QString("bad heuristic implementation: expecting an 'Object' with 'x' and 'y' got '%1'").arg(result.toString()));
            return 3; // Bad Heuristic Implementation
        }
    } else {
        log("script is not enabled");
        return 2; // Script not enabled
    }
}

void Heuristic::setLogger(Debug::Logger *value) {
    logger = value;
}

void Heuristic::log(const QString &msg) {
    if (logger != 0)
        logger->log(msg);
}

} // namespace Core
