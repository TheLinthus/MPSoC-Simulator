#ifndef VIEW_SIMULATIONTAB_H
#define VIEW_SIMULATIONTAB_H

#include <QWidget>

namespace View {

namespace Ui {
class SimulationTab;
}

class SimulationTab : public QWidget {

public:
    explicit SimulationTab(QWidget *parent = 0);
    ~SimulationTab();

private:
    Ui::SimulationTab *ui;
};


} // namespace View
#endif // VIEW_SIMULATIONTAB_H
