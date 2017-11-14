#include "heuristicstab.h"
#include "ui_heuristicstab.h"

#ifdef Q_OS_MAC
#define APPLICATIONSFILES "*.app *.APPL"
#define HEURISTICSPATH "../Resources/Heuristics"
#else
#ifdef Q_OS_WIN
#define APPLICATIONSFILES "*.exe"
#else
#define APPLICATIONSFILES "*"
#endif
#define HEURISTICSPATH "../Heuristics"
#endif

namespace View {

HeuristicsTab::HeuristicsTab(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::HeuristicsTab)
{
    ui->setupUi(this);
}

HeuristicsTab::~HeuristicsTab()
{
    delete ui;
}

} // namespace View

void View::HeuristicsTab::on_pushButtonNew_clicked() {
    QMessageBox::information(0,"TODO","not implemented yet",QMessageBox::Accepted);
    QDir dir(QApplication::applicationDirPath());
    dir.cd(HEURISTICSPATH);
    QString newFile = QFileDialog::getSaveFileName(parentWidget()->parentWidget(), tr("Save new Heuristic"),
                                 dir.path(),tr("Heuristic Script (*.js)"));
    if (newFile.isEmpty()) {
        return; // User canceled
    }

    ui->comboBoxHeuristics->setCurrentIndex(-1);

    heuristicEdit = new Core::Heuristic();
    heuristicEdit->setFileName(newFile);

    ui->groupBoxHeuristic->setEnabled(true);
    ui->lineEditHeuristicpAuthor->setEnabled(true);
    ui->lineEditHeuristicName->setEnabled(true);
    ui->textEditDescription->setEnabled(true);
    ui->textEditScript->setEnabled(true);
    ui->pushButtonCancelHeuristic->setEnabled(true);
    ui->pushButtonExternaEditorHeuristic->setEnabled(true);

    QFile file(newFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QMessageBox::critical(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                              tr("Could not create file\n").append(file.errorString()));
        return;
    }

    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");
    ui->lineEditHeuristicpAuthor->setText(name);
    name = QFileInfo(file).fileName();
    name.chop(3);
    ui->lineEditHeuristicName->setText(name);
    ui->dateEditHeuristicDate->setDate(QDate::currentDate());
    ui->textEditScript->setPlainText("function selectCore(mpsoc,node,application) {\n\t//" + tr("Your heuristic code here") + "\n}");

    QTextStream out(&file);
    out << "var name = \"\";" << endl;
    out << "var author = \"\";" << endl;
    out << "var data = \"" << QDate::currentDate().toString(Qt::ISODate) << "\";" << endl;
    out << "var description = \"\";\n" << endl;
    out << "function selectCore(mpsoc,node,application) {\n\t//" << tr("Your heuristic code here") << "\n}" << endl;
    file.close();

    ui->pushButtonSaveHeuristic->setEnabled(false);
}

void View::HeuristicsTab::on_pushButtonCloneHeuristic_clicked() {
    QString description = ui->textEditDescription->toPlainText();
    QString script = ui->textEditScript->toPlainText();

    on_pushButtonNew_clicked();

    ui->textEditDescription->setPlainText(description);
    ui->textEditScript->setPlainText(script);

    on_pushButtonSaveHeuristic_clicked();
}

void View::HeuristicsTab::on_pushButtonDeleteHeuristic_clicked() {
    QFile file(heuristicEdit->getFileName());
    if (QMessageBox::question(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                              tr("Are you sure you want to delete %1?").arg(QFileInfo(file).fileName()),
                              QMessageBox::No, QMessageBox::Yes)
            == QMessageBox::Yes) {
        if (!file.remove()) {
            QMessageBox::critical(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                                  tr("Error trying to delete Heuristic file\n").append(file.errorString()));
        }

        heuristics->updateAvailabilityList();
    }
}

void View::HeuristicsTab::on_pushButtonExternaEditorHeuristic_clicked() {
    QSettings settings;
    QString defaultEditor = settings.value("defaultEditor").toString();
    if (defaultEditor.isEmpty()) {
        QMessageBox::warning(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                             tr("Default Script Editor not definied."), QMessageBox::Ok);
        QFileDialog openEditor;
        openEditor.setParent(parentWidget()->parentWidget());
        openEditor.setLabelText(QFileDialog::DialogLabel::FileName,tr("Default Scirpt Editor"));
        openEditor.setDirectory(QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation).first());
        openEditor.setFileMode(QFileDialog::ExistingFile);
        openEditor.setNameFilter(tr("Application (%1)").arg(APPLICATIONSFILES));
        openEditor.setFilter(QDir::Files | QDir::Executable);
        if (openEditor.exec()) {
            defaultEditor = openEditor.selectedFiles().first();
            settings.setValue("defaultEditor", defaultEditor);
        } else {
            return;
        }
    }
    QProcess *process = new QProcess();
    connect (process, SIGNAL(finished(int)), process, SLOT(deleteLater()));
    process->start(defaultEditor,QStringList() << heuristicEdit->getFileName());
}

void View::HeuristicsTab::on_pushButtonHeuristicLog_clicked() {
    QString file(QStandardPaths::standardLocations(QStandardPaths::DataLocation).first() + "/Heuristics/Logs/"+QFileInfo(heuristicEdit->getFileName()).fileName()+"_log.txt");
    if (QFile(file).exists()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(file));
    } else {
        QMessageBox::information(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                                 tr("Current Heuristic doesn't have a log file yet"));
    }
}

void View::HeuristicsTab::on_pushButtonOpenHeuristicsDirectory_clicked() {
    QDir dir(QApplication::applicationDirPath());
    dir.cd(HEURISTICSPATH);
    QDesktopServices::openUrl(QUrl::fromLocalFile(dir.path()));
}

void View::HeuristicsTab::on_pushButtonSaveHeuristic_clicked() {
    // Validate Script
    QScriptEngine temp;
    temp.evaluate(ui->textEditScript->toPlainText());
    if (temp.hasUncaughtException()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(parentWidget()->windowTitle());
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(tr("Script Error!"));
        msgBox.setInformativeText(tr("Script file contains uncaught exception."));
        QString details = tr("Line %1: %2").arg(temp.uncaughtExceptionLineNumber()).arg(temp.uncaughtException().toString());
        msgBox.setDetailedText(details);

        QSpacerItem* horizontalSpacer = new QSpacerItem(350, 0, QSizePolicy::Minimum, QSizePolicy::Expanding); // Hack to fix min width
        QGridLayout* layout = (QGridLayout*)msgBox.layout();
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());

        msgBox.exec();
        return;
    }

    heuristicEdit->setAuthor(ui->lineEditHeuristicpAuthor->text().toHtmlEscaped());
    heuristicEdit->setName(ui->lineEditHeuristicName->text().toHtmlEscaped());
    heuristicEdit->setDate(QDate::currentDate());
    heuristicEdit->setDescription(ui->textEditDescription->toPlainText());

    if (heuristicEdit->getFileName().isEmpty()) {
        QMessageBox::warning(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                             tr("File not specified"));
        return;
    }

    QFile file(heuristicEdit->getFileName());

    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
        QTextStream out(&file);
        out << "var name = \"" << heuristicEdit->getName() << "\";" << endl;
        out << "var author = \"" << heuristicEdit->getAuthor() << "\";" << endl;
        out << "var data = \"" << heuristicEdit->getDate().toString(Qt::ISODate) << "\";" << endl;
        out << "var description = \"" << heuristicEdit->getDescription() << "\";\n" << endl;
        out << ui->textEditScript->toPlainText() << endl;

        file.close();
        QMessageBox::information(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                                 tr("Heuristic file saved. The Application need to reload heuristics list now."));
        heuristics->updateAvailabilityList();
    } else {
        QMessageBox::critical(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                              tr("Could not write to file %1").arg(QFileInfo(file).fileName()), QMessageBox::Ok);
    }
}

void View::HeuristicsTab::on_pushButtonCancelHeuristic_clicked() {
    if (ui->comboBoxHeuristics->currentIndex() == -1) { // is hew heuristic
        heuristicEdit->deleteLater();
    }
    clearEditor();
    ui->comboBoxHeuristics->setCurrentIndex(0);
}

void View::HeuristicsTab::on_textEditScript_textChanged() {
    ui->pushButtonSaveHeuristic->setEnabled(true);
}

void View::HeuristicsTab::on_textEditDescription_textChanged() {
    ui->pushButtonSaveHeuristic->setEnabled(true);
}

void View::HeuristicsTab::on_comboBoxHeuristics_currentIndexChanged(const QString &selection) {
    if (ui->pushButtonSaveHeuristic->isEnabled())
        if (QMessageBox::question(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                                  tr("Are you sure? Current changes on Heuristic will be lost"),
                                  QMessageBox::No, QMessageBox::Yes)
                == QMessageBox::No)
            ui->comboBoxHeuristics->setCurrentText(heuristicEdit->getName());
            return;

    clearEditor();
    if (ui->comboBoxHeuristics->currentIndex() <= 0) {
        ui->pushButtonCloneHeuristic->setEnabled(false);
        ui->pushButtonDeleteHeuristic->setEnabled(false);
        ui->pushButtonExternaEditorHeuristic->setEnabled(false);
        ui->pushButtonHeuristicLog->setEnabled(false);
        ui->pushButtonSaveHeuristic->setEnabled(false);
        ui->pushButtonCancelHeuristic->setEnabled(false);
        return;
    }

    heuristicEdit = heuristics->getHeuristic(selection);

    if (heuristicEdit) {
        ui->groupBoxHeuristic->setEnabled(true);
        ui->pushButtonCloneHeuristic->setEnabled(true);
        ui->pushButtonDeleteHeuristic->setEnabled(true);
        ui->pushButtonExternaEditorHeuristic->setEnabled(true);
        ui->pushButtonHeuristicLog->setEnabled(true);
        ui->pushButtonCancelHeuristic->setEnabled(true);

        ui->lineEditHeuristicName->setText(heuristicEdit->getName());
        ui->lineEditHeuristicName->setReadOnly(true);
        ui->lineEditHeuristicpAuthor->setText(heuristicEdit->getAuthor());
        ui->lineEditHeuristicpAuthor->setReadOnly(true);
        ui->textEditDescription->setPlainText(heuristicEdit->getDescription());
        ui->dateEditHeuristicDate->setDate(heuristicEdit->getDate());

        QFile file(heuristicEdit->getFileName());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                                  tr("Error opening Heuristic's script file.\n").append(file.errorString()), QMessageBox::Ok);
            return;
        }
        QString script = file.readAll();
        script.remove(0, script.indexOf("function"));
        ui->textEditScript->setPlainText(script);
        file.close();
    } else {
        QMessageBox::critical(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                              tr("Error Heuristic not loaded on application environment."), QMessageBox::Ok);
    }
    ui->pushButtonSaveHeuristic->setEnabled(false);
}

void View::HeuristicsTab::updateListViewModel(const QStringList &list) {
    ui->comboBoxHeuristics->clear();
    ui->comboBoxHeuristics->addItem(QString(tr("(Select an Heuristic here)")));
    qobject_cast<QStandardItemModel *>(ui->comboBoxHeuristics->model())->item(0)->setEnabled(false);
    ui->comboBoxHeuristics->addItems(list);
}

void View::HeuristicsTab::externalEditorClosed() {
    if (QMessageBox::question(parentWidget()->parentWidget(), parentWidget()->windowTitle(),
                          tr("External Editor was closed, do you want to reload heuristics?"),
                          QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
        heuristics->updateAvailabilityList();
    }
}

void View::HeuristicsTab::clearEditor() {
    heuristicEdit = 0;
    ui->groupBoxHeuristic->setEnabled(false);
    ui->lineEditHeuristicName->clear();
    ui->lineEditHeuristicpAuthor->clear();
    ui->textEditDescription->clear();
    ui->textEditScript->clear();
    ui->dateEditHeuristicDate->clear();
}

void View::HeuristicsTab::on_pushButtonOpenHeuristicsDirectory_2_clicked() {
    heuristics->updateAvailabilityList();
}
