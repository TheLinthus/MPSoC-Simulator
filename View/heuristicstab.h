#ifndef HEURISTICSTAB_H
#define HEURISTICSTAB_H

#include <QWidget>

namespace View {

namespace Ui {
class HeuristicsTab;
}

class HeuristicsTab : public QWidget {

public:
    explicit HeuristicsTab(QWidget *parent = 0);
    ~HeuristicsTab();

private:
    Ui::HeuristicsTab *ui;
};


} // namespace View
#endif // HEURISTICSTAB_H
