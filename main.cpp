#include "mpsoc_se.h"
#include <QApplication>

void logger(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    QFile file(QStandardPaths::standardLocations(QStandardPaths::DataLocation).first() + "/log.txt");
    QDir().mkpath(QFileInfo(file).absolutePath());
    QString typeStr;
    switch (type) {
        case QtDebugMsg:
                typeStr = "Debug:    ";
                fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtInfoMsg:
                typeStr = "Info:     ";
                fprintf(stdout, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtWarningMsg:
                typeStr = "Warning:  ";
                fprintf(stdout, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtCriticalMsg:
                typeStr = "Critical: ";
                fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtFatalMsg:
                typeStr = "Fatal:    ";
                fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
    }
    QString outStr(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ") + typeStr + msg);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << outStr;
        endl(out);
    } else {
        fprintf(stderr, "Could not open file (%s): %s", file.fileName(), file.errorString());
    }
    file.close();
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

#ifdef Q_OS_MAC
    a.setWindowIcon(QIcon("mpsoc.icns"));
#else
    a.setWindowIcon(QIcon("mpsoc.ico"));
#endif

    QCoreApplication::setOrganizationDomain("tecnomissoes.com.br");
    QCoreApplication::setOrganizationName("Tecno Missões");
    QCoreApplication::setApplicationName("MPSoC Simulation Environment");
    QCoreApplication::setApplicationVersion("1.0.0");

    qInstallMessageHandler(logger);

    MPSoC_Simulator w;
    w.show();

    return a.exec();
}
