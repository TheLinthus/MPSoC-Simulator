#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <QStandardPaths>
#include <QScriptValue>

namespace Debug {

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = 0);
    explicit Logger(const QString &fileName, QObject *parent = 0);

private:
    QFile file;

signals:

public slots:
    void log(const QScriptValue &value);
};

} // namespace Debug

#endif // DEBUG_LOGGER_H
