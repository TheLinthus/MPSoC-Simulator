#ifndef CORE_APPNODE_H
#define CORE_APPNODE_H

#include <QObject>
#include <QColor>

namespace Core {

class AppNode : public QObject
{
    Q_OBJECT
public:
    explicit AppNode(qreal lifespan = 200, QColor* color = new QColor(Qt::red), QObject *parent = 0);

    QColor *getColor();

    int getThread() const;
    void setThread(int value);

    qreal getLifespan() const;

private:
    QColor* color;
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
