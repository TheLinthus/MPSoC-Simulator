#include "simulationtab.h"
#include "ui_simulationtab.h"

namespace View {

SimulationTab::SimulationTab(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SimulationTab),
    listModelApplications(new QStringListModel()),
    listModelRunning(new QStringListModel())
{
    ui->setupUi(this);

    ui->listViewApplications->setModel(listModelApplications);
    ui->listViewRunning->setModel(listModelRunning);

    connect(
                ui->listViewApplications->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this, SLOT(on_listViewApplications_selectionModel_selectionChanged(QItemSelection,QItemSelection)));
    connect(
                ui->listViewRunning->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this, SLOT(on_listViewRunning_selectionModel_selectionChanged(QItemSelection,QItemSelection)));
    //connect(
    //            simulator, SIGNAL(notify()),
    //            this, SLOT(updateView()));
}

SimulationTab::~SimulationTab() {
    delete ui;
}

void SimulationTab::updateListViewModel(const QStringList &list) {
    listModelApplications->setStringList(list);
}

void SimulationTab::updateView() {
    ui->pushButtonRunApplication->setEnabled(ui->listViewApplications->selectionModel()->selectedIndexes().count() > 0 && mpsocs->count() > 0 /* && Can Add an Application now */);
    ui->pushButtonKillApplication->setEnabled(ui->listViewRunning->selectionModel()->selectedIndexes().count() > 0 /*&& Can Kill an Application now */);
    ui->buttonAddMPSoC->setEnabled(heuristics->count() > 0);                        // Must have at least 1 heuristic
    ui->buttonNextStep->setEnabled(mpsocs->count() > 0);                            // Must have mpsocs to start simulation
    //ui->buttonPlayTimer->setEnabled(mpsocs->count() > 0 && !simulator->isRunning());// ^^ and isn't running
    //ui->buttonPauseTimer->setEnabled(simulator->isRunning());                       //If running
    //ui->buttonPrevStep->setEnabled(simulator->currentStep() > 0);
    //ui->buttonNextStep->setEnabled(simulator->currentStep() < simulator->stepsCount() - 1);
    //ui->ButtonReset->setEnabled(simulator->stepsCount() > 0);
    //ui->sliderStep->setEnabled(simulator->stepsCount() > 0);
    //ui->labelStepCounter->setText(QString("%1/%2").arg(simulator->currentStep() + 1, simulator->stepCount()));
}

void SimulationTab::on_autoStepToggle(bool) {
    // TODO
}

void SimulationTab::on_pushButtonRunApplication_clicked() {
    // TODO
}

void SimulationTab::on_pushButtonKillApplication_clicked() {
    // TODO
}

void SimulationTab::on_buttonAddMPSoC_clicked() {
    View::NewMPSoCDialog *dialog = new View::NewMPSoCDialog(parentWidget()->parentWidget());

    if (dialog->exec() == QDialog::Rejected)
        return;

    QHBoxLayout* layout =  ui->mpsocsLayout;

    View::MPSoCBox *mpsocbox = new View::MPSoCBox();
    Core::MPSoC *mpsoc = mpsocs->add(dialog->getW(),dialog->getH(),dialog->getMaster());

    mpsoc->setHeuristic(heuristics->getHeuristic(dialog->getHeuristic()));

    mpsocbox->setMPSoC(mpsoc);

    layout->insertWidget(layout->count() - 1, mpsocbox); // Insert before spacer
}

void SimulationTab::on_ButtonReset_clicked() {
    // TODO
}

void SimulationTab::on_buttonPrevStep_clicked() {
    // TODO
}

void SimulationTab::on_buttonNextStep_clicked() {
    // TODO
}

void SimulationTab::on_buttonPauseTimer_clicked() {
    // TODO
}

void SimulationTab::on_buttonPlayTimer_clicked() {
    // TODO
}

void SimulationTab::on_spinBoxTimer_valueChanged(int value) {
    // TODO
}

void SimulationTab::on_listViewApplications_selectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    updateView();
}

void SimulationTab::on_listViewRunning_selectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    updateView();
}

void SimulationTab::on_sliderStep_valueChanged(int value) {
    // TODO
}

} // namespace View
