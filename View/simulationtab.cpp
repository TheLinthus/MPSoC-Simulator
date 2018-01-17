#include "simulationtab.h"
#include "ui_simulationtab.h"

namespace View {

SimulationTab::SimulationTab(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SimulationTab),
    listModelApplications(new QStringListModel()),
    listModelRunning(new QStandardItemModel())
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
    connect(
                simulator, SIGNAL(notify()),
                this, SLOT(updateView()));
    connect(
                simulator, SIGNAL(failed(int)),
                this, SLOT(fail(int)));
}

SimulationTab::~SimulationTab() {
    delete ui;
}

void SimulationTab::updateListViewModel(const QStringList &list) {
    listModelApplications->setStringList(list);
}

void SimulationTab::updateView() {
    listModelRunning->clear();
    QStringList strlist = apps->getRunningApplicationsList();
    QList<QStandardItem *> list;
    for (int i = 0; i < strlist.count(); i++) {
        QPixmap icon(24,24);
        icon.fill(apps->getRunning(i)->getColor());

        QStandardItem *item = new QStandardItem(QIcon(icon), strlist[i]);
        list.append(item);
    }
    listModelRunning->appendColumn(list);
    ui->listViewRunning->update();
    ui->pushButtonRunApplication->setEnabled(ui->listViewApplications->selectionModel()->selectedIndexes().count() > 0 && mpsocs->count() > 0 && simulator->isRunEnabled());
    ui->pushButtonKillApplication->setEnabled(ui->listViewRunning->selectionModel()->selectedIndexes().count() > 0  && simulator->isKillEnabled());
    ui->buttonAddMPSoC->setEnabled(heuristics->count() > 0 && !simulator->isStarted()); // Must have at least 1 heuristic and can't do with running simulation;
    ui->buttonPlayTimer->setEnabled(simulator->isStepEnable() && !simulator->isRunning());// ^^ and isn't running
    ui->buttonPauseTimer->setEnabled(simulator->isRunning());                       // If running
    ui->buttonPrevStep->setEnabled(false);//simulator->currentStep() > 0);          // TODO - Not implemented in this version
    ui->buttonNextStep->setEnabled(simulator->isStepEnable());
    ui->ButtonReset->setEnabled(simulator->isStarted());
    ui->sliderStep->setEnabled(false);//simulator->stepsCount() > 0);               // TODO - Not implemented in this version
    if (simulator->isStarted()) {
        int val = (simulator->currentStep() + 1);
        int max = simulator->stepsCount();
        ui->labelStepCounter->setText(QString("%1/%2").arg(val).arg(max));
        ui->sliderStep->setMaximum(max);
        ui->sliderStep->setValue(val);
    } else {
        ui->labelStepCounter->setText(QString("-/-"));
        ui->sliderStep->setValue(0);
        ui->sliderStep->setMaximum(0);
    }

    emit viewUpdated();
}

void SimulationTab::on_autoStepToggle(bool) {
    // TODO
}

void SimulationTab::on_pushButtonRunApplication_clicked() {
    QString appName = ui->listViewApplications->selectionModel()->selectedIndexes().first().data().toString();
    Core::Application *app = apps->run(appName);
    if (app != 0) {
        simulator->runAction(app);
    } else {
        QMessageBox::warning(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                             QString(tr("Undexpected error. Application %1 didn't run").arg(appName)));
    }
}

void SimulationTab::on_pushButtonKillApplication_clicked() {
    int selected = ui->listViewRunning->selectionModel()->selectedIndexes().first().row();
    apps->kill(selected);
    updateView();
    // TODO - register kill to undo/redo
}

void SimulationTab::on_buttonAddMPSoC_clicked() {
    View::NewMPSoCDialog *dialog = new View::NewMPSoCDialog(parentWidget()->parentWidget());

    if (dialog->exec() == QDialog::Rejected)
        return;

    QHBoxLayout* layout =  ui->mpsocsLayout;

    View::MPSoCBox *mpsocbox = new View::MPSoCBox();
    Core::MPSoC *mpsoc = mpsocs->add(dialog->getH(),dialog->getW(),dialog->getMaster());

    mpsoc->setHeuristic(heuristics->getHeuristic(dialog->getHeuristic()));

    mpsocbox->setMPSoC(mpsoc);
    connect(simulator, SIGNAL(step()), mpsocbox, SLOT(updateChart()));
    connect(simulator, SIGNAL(reseted()), mpsocbox, SLOT(clearChart()));

    layout->insertWidget(layout->count() - 1, mpsocbox); // Insert before spacer

    updateView();
}

void SimulationTab::on_ButtonReset_clicked() {
    if (QMessageBox::question(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                          QString(tr("Are you sure you want to reset simulation?")), QMessageBox::No, QMessageBox::Yes)
            == QMessageBox::Yes)
        simulator->reset();
}

void SimulationTab::on_buttonPrevStep_clicked() {
    simulator->stepBackward();
}

void SimulationTab::on_buttonNextStep_clicked() {
    simulator->stepFoward();
}

void SimulationTab::on_buttonPauseTimer_clicked() {
    simulator->setAutoStep(false);
}

void SimulationTab::on_buttonPlayTimer_clicked() {
    simulator->setAutoStep(true);
}

void SimulationTab::on_spinBoxTimer_valueChanged(int value) {
    simulator->setInterval(value);
}

void SimulationTab::on_listViewApplications_selectionModel_selectionChanged(const QItemSelection &, const QItemSelection &) {
    updateView();
}

void SimulationTab::on_listViewRunning_selectionModel_selectionChanged(const QItemSelection &, const QItemSelection &) {
    ui->pushButtonKillApplication->setEnabled(
                ui->listViewRunning->selectionModel()->selectedIndexes().count() > 0  &&
                simulator->isKillEnabled() &&
                apps->getRunning(ui->listViewRunning->selectionModel()->selectedIndexes().first().row())->isAlive());
}

void SimulationTab::fail(int e){ // change from int to Error object carrying more info
    QMessageBox msgBox;
    msgBox.setWindowTitle(parentWidget()->windowTitle());
    switch (e) { // change for Enum in Error
        case 0 :
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText(tr("Error selecting core!"));
            msgBox.setInformativeText(tr("Heuristic selected a core without free slot, can't procced with simulation.\nCheckout if Heuristic have a valid definition."));
            msgBox.exec();
            break;
    }
}

void SimulationTab::on_sliderStep_valueChanged(int value) {
    // TODO - Not implemented in this version
}

} // namespace View

void View::SimulationTab::on_listViewApplications_activated(const QModelIndex &index)
{
    if (  ui->pushButtonRunApplication->isEnabled()) {
        QString appName = index.data().toString();
        Core::Application *app = apps->run(appName);
        if (app != 0) {
            simulator->runAction(app);
        } else {
            QMessageBox::warning(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                                 QString(tr("Undexpected error. Application %1 didn't run").arg(appName)));
        }
    }
}
