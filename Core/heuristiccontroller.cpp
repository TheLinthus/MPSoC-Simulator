#include "heuristiccontroller.h"
#include "singleton.h"

#ifdef Q_OS_MAC
#define HEURISTICSPATH "../Resources/Heuristics"
#else
#define HEURISTICSPATH "../Heuristics"
#endif

namespace Core {

Core::HeuristicController::HeuristicController(QObject *parent) :
    QObject(parent)
{

}

Core::HeuristicController *HeuristicController::createInstance() {
    return new HeuristicController();
}

Core::HeuristicController::~HeuristicController() {

}

Core::HeuristicController* HeuristicController::instance() {
    return Singleton<Core::HeuristicController>::instance(Core::HeuristicController::createInstance);
}

QList<QString> HeuristicController::getHeuristicsList() const {
    return heuristicList.keys();
}

Heuristic *HeuristicController::getHeuristic(const QString &name) {
    return heuristicList.value(name);
}

int HeuristicController::count() const{
    return heuristicList.count();
}

void HeuristicController::updateAvailabilityList() {
    qDeleteAll(heuristicList);
    heuristicList.clear();

    QDir heuristicsDir(QDir::toNativeSeparators(QApplication::applicationDirPath()));
    heuristicsDir.cd(HEURISTICSPATH);
    heuristicsDir.setNameFilters(QStringList()<<"*.js");    // Filter for '.js' script files

    int count = heuristicsDir.count();
    int progress = 0;

    emit progressMaxUpdate(count);

    foreach (QString s, heuristicsDir.entryList()) {
        emit progressUpdate(progress);
        QString text;

        QFile file(heuristicsDir.absoluteFilePath(s));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Could not open file" << file.fileName();
            progress++;
            continue;
        }

        text = file.readAll();
        file.close();

        progress++;

        QScriptEngine * engine = new QScriptEngine();

        engine->evaluate(text);

        if (engine->hasUncaughtException()) {           // If the file have a syntax error or something like
            qCritical() << file.fileName() << " encountered an exception and could not be loaded.";
            foreach(QString err, engine->uncaughtExceptionBacktrace()) {
                qCritical() << err;
            }
            continue;
        }
        if (engine->evaluate("selectCore == undefined").toBool()) { // Check if it have the required function selectCore
            qCritical() << file.fileName() << " doesn't have selectCore function.";
            continue;
        }

        Core::Heuristic *heuristic = new Core::Heuristic(this);

        heuristic->setFileName(file.fileName());
        if (engine->evaluate("(!name || name.lenght == 0)").toBool()) {
            qWarning() << s << " doesn't have defined name";
            heuristic->setName(s.left(s.size() - 3));    // If it doesn't have a name, cut down '.js' form file name to use as name
        } else {
            heuristic->setName(engine->evaluate("name").toString());        // Read the stored heuristic name to save
        }
        if (engine->evaluate("(!description || description.lenght == 0)").toBool()) {
            qWarning() << s << " doesn't have defined description";
        } else {
            heuristic->setDescription(engine->evaluate("description").toString());
        }
        if (engine->evaluate("(!author || author.lenght == 0)").toBool()) {
            qWarning() << s << " doesn't have defined author";
        } else {
            heuristic->setAuthor(engine->evaluate("author").toString());
        }

        Debug::Logger *logger = new Debug::Logger("Heuristics/Logs/"+s+"_log.txt");
        QScriptValue scriptConsole = engine->newQObject(logger);
        engine->globalObject().setProperty("console", scriptConsole); // Register console object for heuristic debug with 'console.log(msg)'

        QString name = heuristic->getName();
        if (heuristicList.contains(heuristic->getName())) {
            int n = 2;
            do {
                name = heuristic->getName().append(QString(" (%1)").arg(n));
                n++;
            } while (heuristicList.contains(name));

        }
        heuristic->setLogger(logger);
        heuristic->setEngine(engine);
        heuristicList.insert(name, heuristic);

        heuristic->log("Initialized");
    }

    emit progressUpdate(count);
    emit updateDone();
}

} // namespace Core
