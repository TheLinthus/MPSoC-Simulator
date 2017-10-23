#ifndef CORE_APPLOAD_H
#define CORE_APPLOAD_H

#include <QObject>

namespace Core {

class AppLoad : public QObject
{
    Q_OBJECT
public:
    explicit AppLoad(int volume, qreal load, QObject *parent = 0);

    int getVolume() const;

    qreal getLoad() const;

signals:

public slots:

private:
    int volume;
    qreal load;
};

} // namespace Core

#endif // CORE_APPLOAD_H
