#ifndef HEURISTICSTAB_H
#define HEURISTICSTAB_H

#include <QWidget>

namespace Ui {
class HeuristicsTab;
}

class HeuristicsTab : public QWidget
{
    Q_OBJECT

public:
    explicit HeuristicsTab(QWidget *parent = 0);
    ~HeuristicsTab();

private:
    Ui::HeuristicsTab *ui;
};

#endif // HEURISTICSTAB_H
