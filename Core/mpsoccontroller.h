#ifndef CORE_MPSOCCONTROLLER_H
#define CORE_MPSOCCONTROLLER_H

#include <QObject>
#include <Core/mpsoc.h>
#define mpsocs Core::MPSoCController::instance()

namespace Core {

class MPSoCController : public QObject
{
    Q_OBJECT

private:
    MPSoCController(QObject* parent = 0);
    static MPSoCController* createInstance();

    QVector<Core::MPSoC *> mpsocList;

public:
    ~MPSoCController();
    static MPSoCController* instance();

    Core::MPSoC *add(int h, int w, const QPoint &master);
    Core::MPSoC *add(int h, int w, int masterX, int masterY);
    Core::MPSoC *get(int index);
    int count() const;
    void remove(MPSoC *mpsoc);

signals:
    void mpsocDestroyed(QObject*);

public slots:
};

} // namespace Core

#endif // CORE_MPSOCCONTROLLER_H
