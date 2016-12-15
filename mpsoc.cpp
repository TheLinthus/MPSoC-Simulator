#include "mpsoc.h"
#include <QVector>

namespace Core {

MPSoC::MPSoC(int w, int h, QObject *parent)
    : QObject(parent)
    , width(w)
    , height(h) {
    qDebug("Construindo nova MPSoC %dx%d", width, height);
    cores = new Core**[w];
    for (int i = 0; i < w; i++) {
        cores[i] = new Core*[h];
        for (int j = 0; j < h; j++) {
            cores[i][j] = new Core(i,j);
            connect(cores[i][j], SIGNAL(changed()), this, SLOT(update()));
            if (i > 0) { // If isn't 1st col, set west channel form west core
                cores[i][j]->setChannel(West, cores[i-1][j]->getChannel(East));
            }
            if (j > 0) { // If isn't 1st line, set noth channel form north core
                cores[i][j]->setChannel(North, cores[i][j-1]->getChannel(South));
            }
            if (i < w - 1) { // If isn't the last col, set new channel to the east
                cores[i][j]->setChannel(East, new Channel());
                connect(cores[i][j]->getChannel(East), SIGNAL(loadChanged(int)), this, SLOT(update()));
            }
            if (j < h - 1) { // If isn't the last line, set new channel to the south
                cores[i][j]->setChannel(South, new Channel());
                connect(cores[i][j]->getChannel(South), SIGNAL(loadChanged(int)), this, SLOT(update()));
            }
        }
    }
}

MPSoC::~MPSoC()
{
    qDebug("MPSoC %dx%d sendo destruida", width, height);
}

Core* MPSoC::getCore(int x, int y)
{
    if (x > width || y > height) {
        return nullptr;
    }
    return cores[x][y];
}

int MPSoC::getHeight() const
{
    return height;
}

int MPSoC::getWidth() const
{
    return width;
}

QVector<Channel *> MPSoC::getPatch(Core a, Core b) {
    return getPatch(a.getX(), a.getY(), b.getX(), b.getY());
}

QVector<Channel *> MPSoC::getPatch(int x1, int y1, int x2, int y2) {
    if (x1 > width || y1 > height || x2 > width || y2 > height) {
        return QVector<Channel *>();
    }
    int lenght = std::abs(x1 - x2) + std::abs(y1 - y2);
    QVector<Channel *> patch(lenght);

    int count = 0;
    int math = x1 < x2 ? +1 : -1;
    Direction dir = x1 < x2 ? East : West ;
    while (x1 != x2) {
        patch[count] = cores[x1][y1]->getChannel(dir);
        x1 += math;
        count++;
    }
    math = y1 < y2 ? +1 : -1;
    dir = y1 < y2 ? South : North;
    while (y1 != y2) {
        patch[count] = cores[x1][y1]->getChannel(dir);
        y1 += math;
        count++;
    }

    return patch;
}

void MPSoC::update()
{
    emit changed();
}

} // namespace Core
