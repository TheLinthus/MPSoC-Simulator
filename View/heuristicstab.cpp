#include "heuristicstab.h"
#include "ui_heuristicstab.h"

namespace View {

HeuristicsTab::HeuristicsTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeuristicsTab)
{
    ui->setupUi(this);
}

HeuristicsTab::~HeuristicsTab()
{
    delete ui;
}

} // namespace View
