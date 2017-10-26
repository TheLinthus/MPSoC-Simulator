#include "simulationtab.h"
#include "ui_simulationtab.h"

namespace View {

SimulationTab::SimulationTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimulationTab)
{
    ui->setupUi(this);
}

SimulationTab::~SimulationTab()
{
    delete ui;
}

} // namespace View
