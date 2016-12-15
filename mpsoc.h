#ifndef CORE_MPSOC_H
#define CORE_MPSOC_H

#include <QObject>
#include <core.h>
#include <channel.h>

namespace Core {

class MPSoC : public QObject
{
    Q_OBJECT
public:
    explicit MPSoC(int h, int w, QObject *parent = 0);
    ~MPSoC();

    Core *getCore(int x, int y);

    int getHeight() const;
    int getWidth() const;
    QVector<Channel *> getPatch(Core a, Core b);
    QVector<Channel *> getPatch(int x1, int y1, int x2, int y2);

protected:
    int height;
    int width;

private:
    Core*** cores;

signals:
    void changed();

public slots:
    void update();
};

} // namespace Core

#endif // CORE_MPSOC_H
