#include "logger.h"

namespace Debug {

Logger::Logger(QObject *parent) : QObject(parent) {}

void Logger::log(const QString &msg) {
    qDebug() << msg;
    QString fileName = QStandardPaths::standardLocations(QStandardPaths::DataLocation).first() + "/log.txt";
    QFile file;
    file.setFileName(fileName);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << QString(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ") + " " + msg);
    }
    file.close();
}

} // namespace Debug
