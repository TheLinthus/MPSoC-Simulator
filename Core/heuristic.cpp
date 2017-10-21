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

void Heuristic::disableEngine() {
    delete engine;
    enabled = false;
}

bool Heuristic::isEngineEnabled() {
    return enabled;
}

QPoint Heuristic::selectCore() const {
    if (enabled) {
        QScriptValue point = engine->evaluate("selectCore()");      // Call the heuristic script function to selecting a Core
        if (point.isObject() && !point.isError()
                && point.property("x").isValid()
                && point.property("y").isValid()) {                 // If no error was found and the return is an object with X and Y, nothing is wrong with the script
            QPoint p = QPoint();
            p.setX(point.property("x").toInteger());                // Extract the points X and Y from the return
            p.setY(point.property("y").toInteger());
            return p;
        } else {
            throw BadHeuristicImplementationException{};
        }
    } else {
        return QPoint(0, 0);
    }
}

} // namespace Core
