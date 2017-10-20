
#include "mpsoc_se.h"
#include "ui_mpsoc_se.h"

MPSoC_Simulator::MPSoC_Simulator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    thread(new QTimer(this))
{
    QSettings settings;
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());

    QFile qss(":/stylesheet/mainwindow.qss");
    qss.open(QFile::ReadOnly);
    setStyleSheet(QLatin1String(qss.readAll()));

    ui->setupUi(this);

    statusProgress = new QProgressBar(this->statusBar());
    statusProgress->setMaximumSize(242,16);

    statusLabel = new QLabel(this->statusBar());

    statusProgress->setTextVisible(false);
    statusProgress->setValue(0);
    statusProgress->setMinimum(0);
    statusProgress->setMaximum(0);

    this->statusBar()->setContentsMargins(8,0,0,0);
    this->statusBar()->addPermanentWidget(statusLabel);
    this->statusBar()->addPermanentWidget(statusProgress);

    applicationsListModel = new QStringListModel();
    runningListModel = new QStringListModel();
    ui->applicationsList->setModel(applicationsListModel);
    ui->runningList->setModel(runningListModel);

    connect(ui->applicationsList->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(applicationsListModel_selectionChanged(QItemSelection,QItemSelection)));
    connect(ui->runningList->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(runningListModel_selectionChanged(QItemSelection,QItemSelection)));

    QTimer::singleShot(0, this,  SLOT(parallelLoad()));

    restoreState(settings.value("mainWindowState").toByteArray());
}

MPSoC_Simulator::~MPSoC_Simulator() {
    delete ui;
}

void MPSoC_Simulator::parallelLoad() {
    loadApplications();
    loadHeuristics();
    this->statusBar()->showMessage("Load done",2000);
}

void MPSoC_Simulator::loadApplications() {

    this->statusBar()->showMessage("Loading applications files...");

    QDir applicationsDir("Applications");
    applicationsDir.setNameFilters(QStringList()<<"*.json");

    int loadMax = applicationsDir.count();
    int loadValue = 0;

    statusProgress->setMaximum(loadMax);
    statusProgress->setValue(loadValue);

    foreach (QString s, applicationsDir.entryList()) {
        loadValue++;
        statusProgress->setValue(loadValue);

        QString text;
        QFile file("Applications\\"+s);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        text = file.readAll();
        file.close();

        QJsonDocument json = QJsonDocument::fromJson(text.toUtf8());
        if (json.isNull()) {
            qWarning() << "Error File";
            continue;
        }

        QJsonObject obj = json.object();
        QString appsName = obj.value("name").toString();
        QJsonArray array = obj.value("applications").toArray();

        loadMax += array.size();
        statusProgress->setMaximum(loadMax);

        int index = applicationsListModel->rowCount();
        applicationsListModel->insertRows(index, array.size());
        for (int i = 0; i < array.size(); i++) {
            loadValue++;
            if (apps->addFromFile(array.at(i).toObject().value("file").toString())) {
                statusProgress->setValue(loadValue);
                applicationsListModel->setData(applicationsListModel->index(index + i), "[" + appsName + "] " + array.at(i).toObject().value("name").toString());
            }
        }
    }

    statusProgress->setVisible(false);
    this->statusBar()->showMessage("Read applications done.");
}

void MPSoC_Simulator::loadHeuristics() {
    this->statusBar()->showMessage("Reading heuristics availability...");
    statusProgress->setVisible(true);
    statusProgress->setRange(0,0);
    heuristics->updateAvailabilityList();
    statusProgress->setVisible(false);
    this->statusBar()->showMessage("Heuristics lookup done.");
}

void MPSoC_Simulator::applicationButtonsCheckEnable() {
    ui->runApplicationPushButton->setEnabled(ui->applicationsList->selectionModel()->selectedIndexes().count() > 0 && mpsocs->count() > 0 /* && Can Add an Application now */);
    ui->killApplicationPushButton->setEnabled(ui->runningList->selectionModel()->selectedIndexes().count() > 0 /*&& Can Kill an Application now */);
}

void MPSoC_Simulator::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, windowTitle(), QString("Are you sure you want to close %1?").arg(windowTitle()),
                                QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QSettings settings;
        settings.setValue("mainWindowGeometry", saveGeometry());
        settings.setValue("mainWindowState", saveState());

        event->accept();
    } else {
        event->ignore();
    }
}

void MPSoC_Simulator::on_addMPSoCButton_clicked() {
    View::NewMPSoCDialog *dialog = new View::NewMPSoCDialog(this);

    if (dialog->exec() == QDialog::Rejected)
        return;

    QHBoxLayout* layout =  ui->mpsocsLayout;

    View::MPSoCBox* newmpsoc = new View::MPSoCBox();
    newmpsoc->setMPSoC(mpsocs->add(dialog->getX(),dialog->getY()));
    newmpsoc->setHeuristic(heuristics->getHeuristic(dialog->getHeuristic()));

    layout->insertWidget(layout->count() - 1, newmpsoc);

    connect(newmpsoc, SIGNAL(destroyed(QObject*)), this, SLOT(applicationButtonsCheckEnable()));
    applicationButtonsCheckEnable();
}

void MPSoC_Simulator::applicationsListModel_selectionChanged(const QItemSelection, const QItemSelection) {
    applicationButtonsCheckEnable();
}

void MPSoC_Simulator::runningListModel_selectionChanged(const QItemSelection, const QItemSelection) {
    applicationButtonsCheckEnable();
}

void MPSoC_Simulator::on_timerSpinBox_valueChanged(int val) {
    thread->setInterval(val);
}

void MPSoC_Simulator::on_stepSlider_valueChanged(int val) {
    ui->stepCounter->setText(QString("%1/%2").arg(val).arg(ui->stepSlider->maximum()));
}

void MPSoC_Simulator::on_applicationsPushButton_clicked() {
    ui->simulationPushButton->setChecked(false);
    ui->applicationsPushButton->setChecked(true);
    ui->heuristicsPushButton->setChecked(false);
    ui->widgetStack->setCurrentWidget(ui->pageApplications);
}

void MPSoC_Simulator::on_simulationPushButton_clicked() {
    ui->simulationPushButton->setChecked(true);
    ui->applicationsPushButton->setChecked(false);
    ui->heuristicsPushButton->setChecked(false);
    ui->widgetStack->setCurrentWidget(ui->pageSimulator);
}

void MPSoC_Simulator::on_heuristicsPushButton_clicked() {
    ui->simulationPushButton->setChecked(false);
    ui->applicationsPushButton->setChecked(false);
    ui->heuristicsPushButton->setChecked(true);

    //TODO
}
