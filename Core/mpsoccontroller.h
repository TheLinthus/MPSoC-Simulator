#ifndef CORE_MPSOCCONTROLLER_H
#define CORE_MPSOCCONTROLLER_H

#include <QObject>
#include "core/mpsoc.h"
#define mpsocs Core::MPSoCController::instance()

namespace Core {

class MPSoCController : public QObject
{
    Q_OBJECT

private:
    MPSoCController(QObject* parent = 0);
    static MPSoCController* createInstance();

    QList<Core::MPSoC *> mpsocList;

public:
    ~MPSoCController();
    static MPSoCController* instance();

    Core::MPSoC *add(int h, int w);
    Core::MPSoC *get(int index);
    int count() const;
    void remove(MPSoC *mpsoc);

signals:

public slots:
};

} // namespace Core

#endif // CORE_MPSOCCONTROLLER_H
