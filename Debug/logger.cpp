#include "logger.h"

namespace Debug {

Logger::Logger(QObject *parent) : Logger(QStandardPaths::standardLocations(QStandardPaths::DataLocation).first() + "/log.txt", parent) {}

Logger::Logger(const QString &fileName, QObject *parent) :
    QObject(parent),
    file(QStandardPaths::standardLocations(QStandardPaths::DataLocation).first() + "/" + fileName)
{
    QDir().mkpath(QFileInfo(file).absolutePath());
}

void Logger::log(const QScriptValue &value) {
    qDebug() << QFileInfo(file).baseName() << value.toString();
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << QString(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ") + value.toString());
        endl(out);
    } else {
        qWarning() << "Could not open" << file.fileName() << ":" << file.errorString();
    }
    file.close();
}

} // namespace Debug
