
#include "mpsoc_se.h"
#include "ui_mpsoc_se.h"

MPSoC_Simulator::MPSoC_Simulator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    thread(new QThread(this)),
    timer(new QTimer(this)),
    applicationsListModel(new QStringListModel()),
    runningListModel(new QStringListModel()),
    statusLabel(new QLabel(this->statusBar()))
{
    QSettings settings;
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());

    QFile qss(":/stylesheet/mainwindow.qss");
    qss.open(QFile::ReadOnly);
    setStyleSheet(QLatin1String(qss.readAll()));

    ui->setupUi(this);

    statusProgress = new QProgressBar(this->statusBar());
    statusProgress->setMaximumSize(242,16);

    statusProgress->setTextVisible(false);
    statusProgress->setValue(0);
    statusProgress->setMinimum(0);
    statusProgress->setMaximum(0);

    this->statusBar()->setContentsMargins(8,0,0,0);
    this->statusBar()->addPermanentWidget(statusLabel);
    this->statusBar()->addPermanentWidget(statusProgress);

    ui->applicationsList->setModel(applicationsListModel);
    ui->runningList->setModel(runningListModel);

    connect(ui->applicationsList->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this, SLOT(applicationsListModel_selectionChanged(QItemSelection,QItemSelection)));
    connect(ui->runningList->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                this, SLOT(runningListModel_selectionChanged(QItemSelection,QItemSelection)));

    connect(apps, SIGNAL(progressUpdate(int)), statusProgress, SLOT(setValue(int)));
    connect(apps, SIGNAL(progressMaxUpdate(int)), statusProgress, SLOT(setMaximum(int)));
    connect(heuristics, SIGNAL(progressUpdate(int)), statusProgress, SLOT(setValue(int)));
    connect(heuristics, SIGNAL(progressMaxUpdate(int)), statusProgress, SLOT(setMaximum(int)));

    ApplicationLoader *worker = new ApplicationLoader;

    apps->moveToThread(worker);
    heuristics->moveToThread(worker);

    connect(worker, SIGNAL(finished()), this, SLOT(loadingDone()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(worker, SIGNAL(status(QString,int)), statusBar(), SLOT(showMessage(QString,int)));

    worker->start();

    restoreState(settings.value("mainWindowState").toByteArray());
}

MPSoC_Simulator::~MPSoC_Simulator() {
    delete ui;
}

void ApplicationLoader::run() {
    emit status("Loading applications files...", 0);
    apps->updateAvailabilityList();
    emit status("Read applications done.", 2000);

    emit status("Looking for avaliable heuristics...", 0);
    heuristics->updateAvailabilityList();
    emit status("Heuristics lookup done.", 2000);

    apps->moveToThread(QApplication::instance()->thread());
    heuristics->moveToThread(QApplication::instance()->thread());

    emit status("Load done", 2000);
}

void MPSoC_Simulator::loadingDone() {
    statusProgress->setVisible(false);
    applicationsListModel->setStringList(apps->getApplicationsList());
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

    View::MPSoCBox *mpsocbox = new View::MPSoCBox();
    Core::MPSoC *mpsoc = mpsocs->add(dialog->getX(),dialog->getY());

    mpsoc->setHeuristic(heuristics->getHeuristic(dialog->getHeuristic()));

    mpsocbox->setMPSoC(mpsoc);

    layout->insertWidget(layout->count() - 1, mpsocbox);

    connect(mpsocbox, SIGNAL(destroyed(QObject*)), this, SLOT(applicationButtonsCheckEnable()));
    applicationButtonsCheckEnable();
}

void MPSoC_Simulator::applicationsListModel_selectionChanged(const QItemSelection, const QItemSelection) {
    applicationButtonsCheckEnable();
}

void MPSoC_Simulator::runningListModel_selectionChanged(const QItemSelection, const QItemSelection) {
    applicationButtonsCheckEnable();
}

void MPSoC_Simulator::on_timerSpinBox_valueChanged(int val) {
    timer->setInterval(val);
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

void MPSoC_Simulator::on_nextStepButton_clicked() {
    try {
        qDebug() << "First Free: " << heuristics->getHeuristic("First Free")->selectCore();
        qDebug() << "Nearest Neighbor: " << heuristics->getHeuristic("Nearest Neighbor")->selectCore();
    } catch (int e) {
        qDebug() << "Exception " << e;
    }
}
