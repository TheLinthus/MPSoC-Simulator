#ifndef CORE_HEURISTIC_H
#define CORE_HEURISTIC_H

#include <QObject>
#include <QDebug>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptValueIterator>
#include <QtScript/QtScriptDepends>
#include <Exception/badheuristicimplementationexception.h>

namespace Core {

class Heuristic : public QObject
{
    Q_OBJECT
public:
    explicit Heuristic(QObject *parent = 0);

    QString getFileName() const;
    void setFileName(const QString &value);
    QString getName() const;
    void setName(const QString &value);
    QString getDescription() const;
    void setDescription(const QString &value);
    QString getAuthor() const;
    void setAuthor(const QString &value);

    void setEngine(QScriptEngine *value);
    void disableEngine();
    bool isEngineEnabled();

    QPoint selectCore() const;

private:
    QScriptEngine *engine;

    bool enabled;

    QString fileName;
    QString name;
    QString description;
    QString author;

};

} // namespace Core

#endif // CORE_HEURISTIC_H
