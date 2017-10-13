#ifndef VIEW_NEWMPSOCDIALOG_H
#define VIEW_NEWMPSOCDIALOG_H

#include <QDialog>
#include <QStringListModel>
#include <Core/heuristiccontroller.h>

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

    int getX();
    int getY();
    QString getHeuristic();

private:
    Ui::NewMPSoCDialog *ui;
};


} // namespace View
#endif // VIEW_NEWMPSOCDIALOG_H
