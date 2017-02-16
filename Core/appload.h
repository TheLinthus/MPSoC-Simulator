#ifndef CORE_APPLOAD_H
#define CORE_APPLOAD_H

#include <QObject>

namespace Core {

class AppLoad : public QObject
{
    Q_OBJECT
public:
    explicit AppLoad(QObject *parent = 0);

signals:

public slots:
};

} // namespace Core

#endif // CORE_APPLOAD_H