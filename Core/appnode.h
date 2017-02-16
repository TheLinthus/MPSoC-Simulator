#ifndef CORE_APPNODE_H
#define CORE_APPNODE_H

#include <QObject>
#include <QColor>

namespace Core {

class AppNode : public QObject
{
    Q_OBJECT
public:
    explicit AppNode(qreal lifespan, QObject *parent = 0);
    ~AppNode();

    QColor getColor();
    void setColor(QColor color);

    int getThread() const;
    void setThread(int value);

    bool isDone();
    qreal getLifespan() const;
    qreal getCycles() const;

private:
    QColor color;
    qreal cycles;
    qreal lifespan;
    int thread;

signals:
    void finished(int thread);
    void changed();

public slots:
    void tick();
};

} // namespace Core

#endif // CORE_APPNODE_H
