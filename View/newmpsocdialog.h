#ifndef VIEW_NEWMPSOCDIALOG_H
#define VIEW_NEWMPSOCDIALOG_H

#include <QDialog>
#include <QRadioButton>
#include <QStringListModel>
#include <Core/heuristiccontroller.h>
#include <QDebug>

namespace View {

namespace Ui {
class NewMPSoCDialog;
}

class NewMPSoCDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewMPSoCDialog(QWidget *parent = 0);
    ~NewMPSoCDialog();

    int getW();
    int getH();
    QPoint getMaster() const;
    QString getHeuristic();

private:
    Ui::NewMPSoCDialog *ui;

    int w;
    int h;
    int masterX;
    int masterY;

private slots:
    void masterChoose();
    void on_xSpinBox_valueChanged(int x);
    void on_ySpinBox_valueChanged(int arg1);
};


} // namespace View
#endif // VIEW_NEWMPSOCDIALOG_H
