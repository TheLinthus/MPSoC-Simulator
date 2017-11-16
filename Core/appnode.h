#ifndef CORE_APPNODE_H
#define CORE_APPNODE_H

#include <QObject>
#include <QColor>

#include <QDebug>

namespace Core {

class AppNode : public QObject
{
    Q_OBJECT
public:
    explicit AppNode(int n,int lifespan, QObject *parent = 0);
    ~AppNode();

    bool isDone();
    int getLifespan() const;
    int getCycles() const;
    void setLifespan(const int lifespan);
    int getN() const;

    void setParentNode(AppNode *node);
    void addSiblingNode(AppNode *node);
    void addChildNode(AppNode *node);
    AppNode *getParentNode() const;
    AppNode *find(const int n);
    QList<AppNode *> getSiblingNodes() const;
    QList<AppNode *> getChildNodes() const;

    int getWidth() const;
    int getHeight() const;
    int count() const;

    AppNode *clone(QObject *parent);

    int getInThread() const;
    void setInThread(int value);
    int getInCoreX() const;
    void setInCoreX(int value);
    int getInCoreY() const;
    void setInCoreY(int value);
    bool isRunning() const;
    void setRunning(bool value);

private:
    int cycles;
    int n;
    int lifespan;

    AppNode *parentNode;
    QList<AppNode *> siblingNodes;
    QList<AppNode *> childNodes;
    int inThread;
    int inCoreX;
    int inCoreY;
    bool running;
    bool done;

signals:
    void finished(AppNode *);
    void changed();

public slots:
    void tick();
    void kill();
    void killAll();
};

} // namespace Core

#endif // CORE_APPNODE_H
