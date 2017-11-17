#ifndef VIEW_MPSOCBOX_H
#define VIEW_MPSOCBOX_H

#include <QWidget>
#include <QMessageBox>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include <QtCharts>
#include <View/viewprocessor.h>
#include <View/viewchannel.h>
#include <Core/mpsoccontroller.h>
#include <Core/heuristiccontroller.h>

namespace View {

namespace Ui {
class MPSoCBox;
}

class MPSoCBox : public QWidget
{
    Q_OBJECT

public:
    explicit MPSoCBox(QWidget *parent = 0);
    ~MPSoCBox();

    int getGridHeight() const;
    int getGridWidth() const;
    Core::MPSoC *getMpsoc() const;

public slots:
    void setMPSoC(Core::MPSoC *value);
    void updateChart();
    void clearChart();

private slots:
    void on_closeButton_clicked();

    void drawnMPSoC();

    void fitInView();
protected:
    void paintEvent(QPaintEvent *);

private:
    int gridWidth;
    int gridHeight;
    QString heuristicName;
    const QString title = QString("%1x%2 MPSoC, %3");

    Core::MPSoC *mpsoc;

    int stepCount;
    QtCharts::QLineSeries *average;
    QtCharts::QLineSeries *max;
    QtCharts::QChart *chart;
    QtCharts::QValueAxis *axisY;
    QtCharts::QValueAxis *axisX;
    QGraphicsScene *mpsocScene;
    Ui::MPSoCBox *ui;
};


} // namespace View
#endif // VIEW_MPSOCBOX_H
