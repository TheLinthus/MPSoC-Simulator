#ifndef CORE_CORE_H
#define CORE_CORE_H

#include <QObject>
#include <Core/channel.h>
#include <Core/appnode.h>

namespace Core {

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(int x, int y, int threads = 1, QObject *parent = 0);

    void setChannel(Direction direction, Channel *channel);
    Channel *getChannel(Direction direction);
    int firstIdle();
    int isIdle(int i = 0);
    bool run(AppNode *node, int thread = 0);
    AppNode *runningNode(int thread = 0);
    int nOfThreads() const;

    int getX() const;
    int getY() const;

private:
    Channel* north;
    Channel* west;
    Channel* south;
    Channel* east;
    AppNode** cores;
    int threads;

    int x;
    int y;

signals:
    void changed();

public slots:
    void kill(int thread);
};

} // namespace Core

#endif // CORE_CORE_H
