#ifndef CORE_MPSOC_H
#define CORE_MPSOC_H

#include <QObject>
#include <Core/processor.h>
#include <Core/channel.h>
#include <Core/heuristic.h>

namespace Core {

class MPSoC : public QObject
{
    Q_OBJECT
public:
    explicit MPSoC(int h, int w, QPoint master, QObject *parent = 0);
    explicit MPSoC(int h, int w, int masterX = 0, int masterY = 0, QObject *parent = 0);
    ~MPSoC();

    Processor *getCore(const QPoint &p) const;
    Processor *getCore(int x, int y) const;

    int getHeight() const;
    int getWidth() const;
    QVector<Channel *> getPatch(const QPoint &a, const QPoint &b) const;
    QVector<Channel *> getPatch(int x1, int y1, int x2, int y2) const;
    QVector<Processor *> getFree() const;
    QVector<Processor *> getBusy() const;

    Core::Heuristic *getHeuristic() const;
    void setHeuristic(Core::Heuristic *value);

protected:
    int height;
    int width;

private:
    QVector<QVector <Processor *>> processors;
    QPoint master;
    Core::Heuristic *heuristic;

signals:
    void changed();

public slots:
    void update();
};

} // namespace Core

#endif // CORE_MPSOC_H
