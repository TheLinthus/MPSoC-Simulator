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

private:
    QColor* color;
    qreal lifespan;


signals:

public slots:
};

} // namespace Core

#endif // CORE_APPNODE_H
