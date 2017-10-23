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

    bool isDone();
    int getLifespan() const;
    int getCycles() const;

    void setLifespan(const int lifespan);

private:
    int cycles;
    int lifespan;

signals:
    void finished(AppNode *);
    void changed();

public slots:
    void tick();
};

} // namespace Core

#endif // CORE_APPNODE_H
