#ifndef GRAPHICALMPSOC_H
#define GRAPHICALMPSOC_H

#include <QWidget>
#include <QPainter>
#include <mpsoc.h>

namespace Ui {
class GraphicalMPSoC;
}

class GraphicalMPSoC : public QWidget
{
    Q_OBJECT

signals:

public slots:
    void setMPSoC(Core::MPSoC *value);

public:
    explicit GraphicalMPSoC(QWidget *parent = 0);
    ~GraphicalMPSoC();

    int getGridHeight() const;
    int getGridWidth() const;
    Core::MPSoC *getMpsoc() const;

protected:
    void paintEvent(QPaintEvent *);

private:
    int gridWidth;
    int gridHeight;
    Core::MPSoC* mpsoc;
};

#endif // GRAPHICALMPSOC_H
