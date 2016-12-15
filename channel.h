#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
//#include <core.h>

namespace Core {

enum Direction {North,West,South,East};

class Channel : public QObject
{
    Q_OBJECT
public:
    explicit Channel(QObject *parent = 0);

    void add(qreal value);
    qreal val();
    void reset();

private:
    qreal load;

signals:
    void overloaded();
    void loadChanged(int value);

public slots:
};

} // namespace Core

#endif // CHANNEL_H
