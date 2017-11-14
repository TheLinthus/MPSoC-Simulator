#ifndef CORE_HEURISTIC_H
#define CORE_HEURISTIC_H

#include <QObject>
#include <QDebug>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptValueIterator>
#include <QtScript/QtScriptDepends>
#include <Debug/logger.h>

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
    QScriptEngine* getEngine() const;
    void disableEngine();
    bool isEngineEnabled();

    int selectCore(QPoint &p, QScriptValueList& args);

    void setLogger(Debug::Logger *value);
    void log(const QString &msg);

private:
    QScriptEngine *engine;

    bool enabled;

    Debug::Logger *logger;

    QString fileName;
    QString name;
    QString description;
    QString author;

};

} // namespace Core

#endif // CORE_HEURISTIC_H
