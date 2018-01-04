#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QDebug>
#include <Core/appload.h>

namespace Core {

enum Direction {North,West,South,East};
enum DataDirection {In, Out};

class Channel : public QObject
{
    Q_OBJECT
public:
    explicit Channel(int ax, int ay, int bx, int by, QObject *parent = 0);

    /**
     * @brief add Add more payload to the channel.
     * @param load AppLoad pointer, with floating number to add load value
     */
    void add(AppLoad *load);
    /**
     * @brief val Return the current total payload on the channel
     * @return <i>qreal</i> payload on the channel
     */
    qreal val() const;
    /**
     * @brief reset Reset the payload to 0 (zero)
     */
    void reset();

    int getAx() const;
    int getAy() const;
    int getBx() const;
    int getBy() const;

private:
    QVector<AppLoad *> loads;
    int ax,ay,bx,by;

private slots:
    void removeLoad(AppLoad *load);

signals:
    void overloaded();
    void loadChanged(int value);

public slots:
};

} // namespace Core

#endif // CHANNEL_H
