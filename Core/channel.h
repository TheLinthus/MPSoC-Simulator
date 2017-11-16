#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QDebug>

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
     * @param AtB boolean direction of load indicator, if from point A to B
     */
    void add(qreal value, bool AtoB);
    /**
     * @brief val Return the current total payload on the channel
     * @return <i>qreal</i> payload on the channel
     */
    qreal val() const;
    /**
      * @brief valAtB Return the current payload from point A to B in the channel
      * @return <i>qreal</i> payload from A to B
      */
    qreal valAtB() const;
    /**
      * @brief valAtB Return the current payload from point A to B in the channel
      * @return <i>qreal</i> payload from A to B
      */
    qreal valBtA() const;
    /**
     * @brief reset Reset the payload to 0 (zero)
     */
    void reset();

private:
    qreal loadAtB; // A floating value that may go from 0 to 100
    qreal loadBtA;

signals:
    void overloaded();
    void loadChanged(int value);

public slots:
};

} // namespace Core

#endif // CHANNEL_H
