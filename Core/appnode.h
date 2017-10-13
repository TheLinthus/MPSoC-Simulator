#ifndef CORE_APPNODE_H
#define CORE_APPNODE_H

#include <QObject>
#include <QColor>

namespace Core {

class AppNode : public QObject
{
    Q_OBJECT
public:
    explicit AppNode(int lifespan, QObject *parent = 0);
    ~AppNode();

    int getThread() const;
    void setThread(int value);

    bool isDone();
    int getLifespan() const;
    int getCycles() const;

    void setLifespan(const int lifespan);

private:
    int cycles;
    int lifespan;
    int thread;

signals:
    void finished(int thread);
    void changed();

public slots:
    void tick();
};

} // namespace Core

#endif // CORE_APPNODE_H
