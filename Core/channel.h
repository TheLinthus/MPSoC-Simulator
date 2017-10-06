#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>

namespace Core {

enum Direction {North,West,South,East};

class Channel : public QObject
{
    Q_OBJECT
public:
    explicit Channel(QObject *parent = 0);

    /**
     * @brief add Change the payload of the channel, can have negative and positive values, to increase and decrease.
     * @param value qreal floating number to add or remove payload
     */
    void add(qreal value);
    /**
     * @brief val Return the current payload on the channel
     * @return <i>qreal</i> payload on the channel
     */
    qreal val();
    /**
     * @brief reset Reset the payload to 0 (zero)
     */
    void reset();

private:
    qreal load; // A floating value that may go from 0 to 100

signals:
    void overloaded();
    void loadChanged(int value);

public slots:
};

} // namespace Core

#endif // CHANNEL_H
