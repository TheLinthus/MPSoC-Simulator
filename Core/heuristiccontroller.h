#ifndef CORE_HEURISTICCONTROLLER_H
#define CORE_HEURISTICCONTROLLER_H

#include <QObject>
#include <QApplication>
#include <QFile>
#include "Core/heuristic.h"
#include <Debug/logger.h>
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

    QStringList getHeuristicsList() const;
    Core::Heuristic *getHeuristic(const QString &name);
    int count() const;

public slots:
    void updateAvailabilityList();

signals:
    void progressUpdate(int);
    void progressMaxUpdate(int);
    void updateDone(QStringList);
};

} // namespace Core

#endif // CORE_HEURISTICCONTROLLER_H
