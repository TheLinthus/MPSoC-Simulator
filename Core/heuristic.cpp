#include "heuristic.h"

namespace Core {

Heuristic::Heuristic(QObject *parent) :
    QObject(parent),
    enabled(false),
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
        QScriptValue evaluated = engine->evaluate("selectCore()");
        if (evaluated.isObject()) {
            QScriptValue point = evaluated.toObject();
            QPoint p = QPoint();
            p.setX(point.property("xp").toInteger());
            p.setY(point.property("yp").toInteger());
            return p;
        } else {
            throw BadHeuristicImplementationException{};
        }
    } else {
        return QPoint(0, 0);
    }
}

} // namespace Core
