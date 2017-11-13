#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QStandardPaths>

namespace Debug {

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = 0);

signals:

public slots:
    void log(const QString &msg);
};

} // namespace Debug

#endif // DEBUG_LOGGER_H
