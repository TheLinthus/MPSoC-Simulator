#include "applicationstab.h"
#include "ui_applicationstab.h"

#ifdef Q_OS_MAC
#define APPLICATIONSPATH "../Resources/Applications"
#else
#define APPLICATIONSPATH "Resources/Applications"
#endif

namespace View {

ApplicationsTab::ApplicationsTab(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ApplicationsTab),
    scene(new QGraphicsScene),
    listModelApplicationsGroup(new QStringListModel())
{
    ui->setupUi(this);

    ui->graphicsViewApplication->setScene(scene);
    ui->graphicsViewApplication->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);

    ui->listViewApplicationsGroup->setModel(listModelApplicationsGroup);

    connect(
                ui->listViewApplicationsGroup->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this, SLOT(on_listViewApplicationsGroup_selectionModel_selectionChanged(QItemSelection,QItemSelection)));
}

ApplicationsTab::~ApplicationsTab() {
    delete ui;
}

void ApplicationsTab::updateListViewModel(const QStringList &, const QStringList &groups) {
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
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Applications Group (*.json)"));
    if (dialog.exec()) {
        // TODO
    }
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

        foreach (Core::Application *app, removeList) {
            editApplicationsGroup->remove(app->getName());
            app->deleteLater();
        }
        foreach (Core::Application *app, addList) {
            editApplicationsGroup->add(app);
        }

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

        foreach (Core::Application *app, addList) {
            editApplicationsGroup->add(app);
        }

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

void ApplicationsTab::on_listViewApplicationsGroup_selectionModel_selectionChanged(const QItemSelection &, const QItemSelection &) {
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

        ui->comboBoxApplications->clear();
        ui->comboBoxApplications->addItem(QString(tr("(Select an Application here)")));
        qobject_cast<QStandardItemModel *>(ui->comboBoxApplications->model())->item(0)->setEnabled(false);
        ui->comboBoxApplications->addItems(editApplicationsGroup->getApplicationsList());
        ui->comboBoxApplications->setEnabled(true);
        ui->pushButtonAddApplication->setEnabled(true);
        ui->pushButtonRemoveApplication->setEnabled(false);
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

    ui->comboBoxApplications->clear();
    ui->comboBoxApplications->addItem(QString(tr("(Select an Application Grup first)")));
    qobject_cast<QStandardItemModel *>(ui->comboBoxApplications->model())->item(0)->setEnabled(false);

    ui->comboBoxApplications->setEnabled(false);
    ui->pushButtonAddApplication->setEnabled(false);
    ui->pushButtonRemoveApplication->setEnabled(false);

    removeList.clear();
    addList.clear();
}

void ApplicationsTab::on_pushButtonOpenDirectory_clicked() {
    QDir dir(QApplication::applicationDirPath());
    dir.cd(APPLICATIONSPATH);
    QDesktopServices::openUrl(QUrl::fromLocalFile(dir.path()));
}

void ApplicationsTab::on_comboBoxApplications_currentIndexChanged(const QString &value) {
    scene->clear();
    if (!value.isEmpty() && ui->comboBoxApplications->currentIndex() != 0) {
        Core::Application *app = apps->getApplication(value);
        if (app != 0) {
            ui->pushButtonRemoveApplication->setEnabled(true);

            View::AppNode *appnode = new View::AppNode(0,0,0,0);
            scene->addItem(appnode);
            appnode = new View::AppNode(0,1,2,1);
            scene->addItem(appnode);
            appnode = new View::AppNode(1,1,2,2);
            scene->addItem(appnode);
            appnode = new View::AppNode(0,2,5,3);
            scene->addItem(appnode);
            appnode = new View::AppNode(1,2,5,4);
            scene->addItem(appnode);
            appnode = new View::AppNode(2,2,5,5);
            scene->addItem(appnode);
            appnode = new View::AppNode(3,2,5,6);
            scene->addItem(appnode);
            appnode = new View::AppNode(4,2,5,7);
            scene->addItem(appnode);

            ui->graphicsViewApplication->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
        } else {
            QMessageBox::critical(parentWidget()->parentWidget(), parentWidget()->windowTitle(), QString(tr("Error getting application, selected Index not found")));
        }
    } else {
        ui->pushButtonRemoveApplication->setEnabled(false);
    }
}

void ApplicationsTab::on_pushButtonAddApplication_clicked() {
    bool confirm;
    QString name = QInputDialog::getText(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                          tr("New Applications name:"), QLineEdit::Normal,
                          tr("Application"), &confirm);
    if (confirm) {
        QFileDialog dialog(parentWidget()->parentWidget());
        dialog.setFileMode(QFileDialog::ExistingFiles);
        dialog.setNameFilter(tr("Applications (*.txt *.xml)"));
        if (dialog.exec()) {
            Core::Application *app = apps->readAppFromFile(dialog.selectedFiles().first(), name);
            if (app) {
                addList.append(app);
            } else {
                QMessageBox::critical(parentWidget()->parentWidget(), parentWidget()->windowTitle(), QString(tr("Error loading Application file!")));
            }
        }
    }
}

void ApplicationsTab::on_pushButtonRemoveApplication_clicked() {
    if (QMessageBox::question(
                parentWidget()->parentWidget(),
                parentWidget()->windowTitle(),
                QString(tr("Are you sure you want to remove %1").arg(ui->comboBoxApplications->currentText())),
                QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
        if (removeList.isEmpty()) {
            QMessageBox::information(
                        parentWidget()->parentWidget(),
                        parentWidget()->windowTitle(),
                        QString(tr("You need to \"Save\" to apply changes")),
                        QMessageBox::Ok);
        }
        removeList << apps->getApplication(ui->comboBoxApplications->currentText());
        ui->comboBoxApplications->removeItem(ui->comboBoxApplications->currentIndex());
        ui->lineEditApplicationGroupApplicationsCount->setText(QString::number(editApplicationsGroup->getApplicationsCount() - removeList.count() + addList.count()));
        ui->comboBoxApplications->setCurrentIndex(0);
    }
}

} // namespace View
