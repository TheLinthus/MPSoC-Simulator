#ifndef VIEW_MPSOCBOX_H
#define VIEW_MPSOCBOX_H

#include <QWidget>
#include <QMessageBox>
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include <View/viewprocessor.h>
#include <View/viewchannel.h>
#include <Core/mpsoccontroller.h>
#include <Core/heuristiccontroller.h>
#include <QDebug>

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

    QGraphicsScene *mpsocScene;
    Ui::MPSoCBox *ui;
};


} // namespace View
#endif // VIEW_MPSOCBOX_H
