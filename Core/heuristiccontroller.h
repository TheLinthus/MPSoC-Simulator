#ifndef CORE_HEURISTICCONTROLLER_H
#define CORE_HEURISTICCONTROLLER_H

#include <QObject>
#include <QFile>
#include "Core/heuristic.h"
#define heuristics Core::HeuristicController::instance()

namespace Core {

class HeuristicController : public QObject
{
    Q_OBJECT
private:
    HeuristicController(QObject* parent = 0);
    static HeuristicController* createInstance();

    QMap<QString, Core::Heuristic *> heuristicList;

public:
    ~HeuristicController();
    static HeuristicController* instance();

    QList<QString> getHeuristicsList() const;
    Core::Heuristic *getHeuristic(const QString &name);

public slots:
    void updateAvailabilityList();
};

} // namespace Core

#endif // CORE_HEURISTICCONTROLLER_H
