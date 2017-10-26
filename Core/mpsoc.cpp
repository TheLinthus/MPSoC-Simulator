#include "mpsoc.h"
#include <QVector>

namespace Core {

MPSoC::MPSoC(int h, int w, QPoint master, QObject *parent)
    : QObject(parent)
    , width(w)
    , height(h)
    , master(master)
    , processors(w, QVector<Processor *>(h))
{
    qDebug("Construindo nova MPSoC %dx%d", width, height);
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            if (i == master.x() && j == master.y()) {
                processors[i][j] = new Processor(i,j, Processor::Master);
            } else {
                processors[i][j] = new Processor(i,j);
            }
            connect(processors[i][j], SIGNAL(changed()), this, SLOT(update()));
            if (i > 0) { // If isn't 1st col, set west channel form west core
                processors[i][j]->setChannel(West, processors[i-1][j]->getChannel(East));
            }
            if (j > 0) { // If isn't 1st line, set noth channel form north core
                processors[i][j]->setChannel(North, processors[i][j-1]->getChannel(South));
            }
            if (i < w - 1) { // If isn't the last col, set new channel to the east
                processors[i][j]->setChannel(East, new Channel());
                connect(processors[i][j]->getChannel(East), SIGNAL(loadChanged(int)), this, SLOT(update()));
            }
            if (j < h - 1) { // If isn't the last line, set new channel to the south
                processors[i][j]->setChannel(South, new Channel());
                connect(processors[i][j]->getChannel(South), SIGNAL(loadChanged(int)), this, SLOT(update()));
            }
        }
    }
}

MPSoC::MPSoC(int w, int h, int masterX, int masterY, QObject *parent)
    : MPSoC(h,w,QPoint(masterX,masterY),parent)
{

}

MPSoC::~MPSoC()
{
    qDebug("MPSoC %dx%d sendo destruida", width, height);
}

Processor *MPSoC::getCore(const QPoint &p) const {
    return getCore(p.x(), p.y());
}

Processor* MPSoC::getCore(int x, int y) const {
    if (x > width || y > height) {
        return 0;
    }
    return processors[x][y];
}

int MPSoC::getHeight() const {
    return height;
}

int MPSoC::getWidth() const {
    return width;
}

QVector<Channel *> MPSoC::getPatch(const QPoint &a, const QPoint &b) const {
    return getPatch(a.x(), a.y(), b.x(), b.y());
}

QVector<Channel *> MPSoC::getPatch(int x1, int y1, int x2, int y2) const {
    if (x1 > width || y1 > height || x2 > width || y2 > height) {
        return QVector<Channel *>();    // If out of bound return empety list
    }
    int lenght = abs(x1 - x2) + abs(y1 - y2);
    QVector<Channel *> patch(lenght);

    int math = x1 < x2 ? +1 : -1;
    Direction dir = x1 < x2 ? East : West ;
    while (x1 != x2) {
        patch.append(processors[x1][y1]->getChannel(dir));
        x1 += math;
    }
    math = y1 < y2 ? +1 : -1;
    dir = y1 < y2 ? South : North;
    while (y1 != y2) {
        patch.append(processors[x1][y1]->getChannel(dir));
        y1 += math;
    }

    return patch;
}

QVector<Processor *> MPSoC::getFree() const {
    QVector<Processor *> list = QVector<Processor *>();

    for (int i = 0; i < width; i ++) {
        for (int j = 0; j < height; j++) {
            Processor * p = processors[i][j];
            for (int k = 0; k < p->nOfThreads(); k++) {
                if (p->isIdle(k)) {
                    list.append(p); // If at least one thread is free add it into return list
                    continue;
                }
            }
        }
    }

    return list;
}

QVector<Processor *> MPSoC::getBusy() const {
    QVector<Processor *> list = QVector<Processor *>();

    for (int i = 0; i < width; i ++) {
        for (int j = 0; j < height; j++) {
            Processor * p = processors[i][j];
            list.append(p);
            for (int k = 0; k < p->nOfThreads(); k++) {
                if (p->isIdle(k)) {
                    list.removeLast(); // If at least one thread is free doesn't put it into return list
                    continue;
                }
            }
        }
    }

    return list;
}

Core::Heuristic *MPSoC::getHeuristic() const {
    return heuristic;
}

void MPSoC::setHeuristic(Core::Heuristic *value) {
    heuristic = value;
}

// Prototype, dynamic modify mpsoc
//void MPSoC::setWidth(int w) {
//    width = w;
//    // destroy or create new processors
//    emit changed();
//}
//
//void MPSoC::destroyPorcessor(int x, int y) {
//  emit changed();
//}

void MPSoC::update() {
}

} // namespace Core
