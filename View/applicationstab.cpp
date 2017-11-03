#include "applicationstab.h"
#include "ui_applicationstab.h"

namespace View {

ApplicationsTab::ApplicationsTab(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ApplicationsTab),
    listModelApplicationsGroup(new QStringListModel())
{
    ui->setupUi(this);

    ui->listViewApplicationsGroup->setModel(listModelApplicationsGroup);

    connect(
                ui->listViewApplicationsGroup->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this, SLOT(on_listViewApplicationsGroup_selectionModel_selectionChanged(QItemSelection,QItemSelection)));
}

ApplicationsTab::~ApplicationsTab() {
    delete ui;
}

void ApplicationsTab::updateListViewModel(const QStringList &applications, const QStringList &groups) {
    listModelApplicationsGroup->setStringList(groups);
}

void ApplicationsTab::on_pushButtonCreateNewApplicationGroup_clicked() {
    ui->listViewApplicationsGroup->selectionModel()->clearSelection();
    ui->groupBoxApplicationGroup->setEnabled(true);
    ui->groupBoxApplicationGroup->setTitle(tr("New Application Group"));
    ui->pushButtonSaveApplicationGroup->setEnabled(true);
    ui->pushButtonCancelApplicationGroup->setEnabled(true);
    ui->lineEditApplicationGroupName->setFocus();
    ui->lineEditApplicationGroupName->setReadOnly(false);
    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");
    ui->lineEditApplicationGroupAuthor->setText(name);
    ui->lineEditApplicationGroupAuthor->setReadOnly(false);
    ui->lineEditApplicationGroupApplicationsCount->setText("0");
    ui->dateEditApplicationGroupDate->setDate(QDate::currentDate());
    ui->checkBoxApplicationGroupEnabled->setChecked(true);
}

void ApplicationsTab::on_pushButtonCreateApplicationGroupFromFile_clicked() {
    QFileDialog dialog(parentWidget()->parentWidget());

    QMessageBox::information(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                         tr("Select between a group of Applications (*.txt) or an Application Group (*.json)"), QMessageBox::Ok);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    QStringList filters;
    filters << tr("Applications (*.txt)")
            << tr("Applications Group (*.json)");
    dialog.setNameFilters(filters);

    dialog.exec();
}

void ApplicationsTab::on_pushButtonSaveApplicationGroup_clicked() {
    if (ui->lineEditApplicationGroupName->text().trimmed().isEmpty()) {
        QMessageBox::warning(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                             tr("Application group's Name can not be empty"), QMessageBox::Ok);
        ui->lineEditApplicationGroupName->setFocus();
        return;
    }
    if (ui->listViewApplicationsGroup->selectionModel()->selectedIndexes().count() > 0) { // Edit
        QString applicationsGroupName = ui->listViewApplicationsGroup->selectionModel()->selectedIndexes().first().data().toString();
        editApplicationsGroup = apps->getApplicationGroup(applicationsGroupName);
        editApplicationsGroup->setEnabled(ui->checkBoxApplicationGroupEnabled->isChecked());

        if (apps->saveToFile(editApplicationsGroup)) {
            QMessageBox::information(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                                     tr("Application Group Saved"), QMessageBox::Ok);
        } else {
            QMessageBox::critical(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                                  tr("Error saving Application Group File, please review System permitions"), QMessageBox::Ok);
        }
    } else {    // New
        editApplicationsGroup = new Core::ApplicationGroup(apps);
        editApplicationsGroup->setAuthor(ui->lineEditApplicationGroupAuthor->text());
        editApplicationsGroup->setDate(ui->dateEditApplicationGroupDate->date());
        editApplicationsGroup->setEnabled(ui->checkBoxApplicationGroupEnabled->isChecked());
        editApplicationsGroup->setName(ui->lineEditApplicationGroupName->text());

        if (apps->saveToFile(editApplicationsGroup)) {
            apps->add(editApplicationsGroup);
            QMessageBox::information(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                                     tr("New Application Group Saved"), QMessageBox::Ok);
        } else {
            QMessageBox::critical(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                                  tr("Error creating new Application Group File, please review Application Group informations and System permitions"), QMessageBox::Ok);
        }
    }
}

void ApplicationsTab::on_pushButtonCancelApplicationGroup_clicked() {
    ui->listViewApplicationsGroup->selectionModel()->clearSelection();
    clearEditor();
}

void ApplicationsTab::on_listViewApplicationsGroup_selectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    if (ui->listViewApplicationsGroup->selectionModel()->selectedIndexes().count() > 0) {
        ui->groupBoxApplicationGroup->setEnabled(true);
        ui->groupBoxApplicationGroup->setTitle(tr("Edit Application Group"));
        ui->pushButtonSaveApplicationGroup->setEnabled(true);
        ui->pushButtonCancelApplicationGroup->setEnabled(true);

        editApplicationsGroup = apps->getApplicationGroup(ui->listViewApplicationsGroup->selectionModel()->selectedIndexes().first().data().toString());

        ui->lineEditApplicationGroupName->setReadOnly(true);
        ui->lineEditApplicationGroupName->setText(editApplicationsGroup->getName());
        ui->lineEditApplicationGroupAuthor->setReadOnly(true);
        ui->lineEditApplicationGroupAuthor->setText(editApplicationsGroup->getAuthor());
        ui->lineEditApplicationGroupApplicationsCount->setText(QString::number(editApplicationsGroup->getApplicationsCount()));
        ui->dateEditApplicationGroupDate->setDate(editApplicationsGroup->getDate());
        ui->checkBoxApplicationGroupEnabled->setChecked(editApplicationsGroup->isEnabled());
    } else {
        clearEditor();
    }
}

void ApplicationsTab::clearEditor() {
    editApplicationsGroup = 0;

    ui->groupBoxApplicationGroup->setEnabled(false);
    ui->groupBoxApplicationGroup->setTitle(tr("Select an Application Group"));
    ui->pushButtonSaveApplicationGroup->setEnabled(false);
    ui->pushButtonCancelApplicationGroup->setEnabled(false);
    ui->lineEditApplicationGroupName->clear();
    ui->lineEditApplicationGroupAuthor->clear();
    ui->lineEditApplicationGroupApplicationsCount->clear();
    ui->checkBoxApplicationGroupEnabled->setChecked(false);
}

void ApplicationsTab::on_pushButtonOpenDirectory_clicked() {
    QDesktopServices::openUrl(QUrl("Applications"));
}

} // namespace View
