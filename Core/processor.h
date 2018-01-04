#ifndef CORE_CORE_H
#define CORE_CORE_H

#include <QObject>
#include <QDebug>
#include <Core/channel.h>
#include <Core/application.h>

namespace Core {

class Processor : public QObject
{
    Q_OBJECT

public:
    enum Type {Master,Slave};
    Q_ENUM(Type)

    explicit Processor(int x, int y, Type type = Slave, int threads = 1, QObject *parent = 0);

    void setChannel(DataDirection inout, Direction direction, Channel *channel);
    Channel *getChannel(DataDirection inout, Direction direction);
    int firstIdle();
    int isIdle(int i = 0);
    bool run(AppNode *node, int thread = 0);
    AppNode *runningNode(int thread = 0);
    int nOfThreads() const;

    int getX() const;
    int getY() const;
    int getType() const;
    bool isMaster();
    int getThreadOf(const AppNode *appNode);

private:
    Channel* inNorth;
    Channel* inWest;
    Channel* inSouth;
    Channel* inEast;
    Channel* outNorth;
    Channel* outWest;
    Channel* outSouth;
    Channel* outEast;
    QVector<AppNode*> cores;
    int threads;
    Type type;

    int x;
    int y;

signals:
    void changed();

public slots:
    void kill(int thread = -1);
    void kill(AppNode * appNode);
    void kill(QObject * appNode);
};

} // namespace Core

#endif // CORE_CORE_H
