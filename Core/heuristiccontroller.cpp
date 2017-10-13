#include "heuristiccontroller.h"
#include "singleton.h"

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

void HeuristicController::updateAvailabilityList() {
    qDeleteAll(heuristicList);
    heuristicList.clear();

    QDir heuristicsDir("Heuristics");
    heuristicsDir.setNameFilters(QStringList()<<"*.js");    // Filter for '.js' script files

    foreach (QString s, heuristicsDir.entryList()) {
        QString text;

        QFile file(QString("Heuristics\\%1").arg(s));
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        text = file.readAll();
        file.close();

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
        if (engine->evaluate("name == undefined").toBool()) {
            qWarning() << s << " doesn't have defined name";
            heuristic->setName(s.left(s.size() - 3));    // If it doesn't have a name, cut down '.js' form file name to use as name
        } else {
            heuristic->setName(engine->evaluate("name").toString());        // Read the stored heuristic name to save
        }
        if (engine->evaluate("description == undefined").toBool()) {
            qWarning() << s << " doesn't have defined description";
        } else {
            heuristic->setDescription(engine->evaluate("description").toString());
        }
        if (engine->evaluate("author == undefined").toBool()) {
            qWarning() << s << " doesn't have defined author";
        } else {
            heuristic->setAuthor(engine->evaluate("author").toString());
        }

        QString name = heuristic->getName();
        if (heuristicList.contains(heuristic->getName())) {
            int n = 2;
            do {
                name = heuristic->getName().append(QString(" (%1)").arg(n));
                n++;
            } while (heuristicList.contains(name));

        }
        heuristicList.insert(name, heuristic);
    }
}

} // namespace Core
