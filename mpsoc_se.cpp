
#include "mpsoc_se.h"
#include "ui_mpsoc_se.h"

MPSoC_Simulator::MPSoC_Simulator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    thread(new QThread(this)),
    timer(new QTimer(this)),
    statusLabel(new QLabel(this->statusBar()))
{
    QSettings settings;

    // SetUp GUI
    ui->setupUi(this);

    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());    // Restore window size/position from last size/position
    restoreState(settings.value("mainWindowState").toByteArray());          // Restore window state from last window state

    QFile qss(":/stylesheet/mainwindow.qss");
    qss.open(QFile::ReadOnly);
    setStyleSheet(QLatin1String(qss.readAll()));
    qss.close();

    // SetUp progress bar from main window's Status Bar
    statusProgress = new QProgressBar(this->statusBar());
    statusProgress->setMaximumSize(242,16);

    statusProgress->setTextVisible(false);
    statusProgress->setValue(0);
    statusProgress->setMinimum(0);
    statusProgress->setMaximum(0);

    this->statusBar()->setContentsMargins(8,0,0,0);
    this->statusBar()->addPermanentWidget(statusLabel);
    this->statusBar()->addPermanentWidget(statusProgress);

    connect(apps, SIGNAL(progressUpdate(int)), statusProgress, SLOT(setValue(int)));
    connect(apps, SIGNAL(progressMaxUpdate(int)), statusProgress, SLOT(setMaximum(int)));
    connect(heuristics, SIGNAL(progressUpdate(int)), statusProgress, SLOT(setValue(int)));
    connect(heuristics, SIGNAL(progressMaxUpdate(int)), statusProgress, SLOT(setMaximum(int)));

    // SetUp widgets view for the main WidgetStack
    simulationTab = new View::SimulationTab(ui->widgetStack);
    applicationsTab = new View::ApplicationsTab(ui->widgetStack);
    heuristicsTab = new View::HeuristicsTab(ui->widgetStack);

    connect(apps, SIGNAL(updateDone(QStringList,QStringList)), simulationTab, SLOT(updateListViewModel(QStringList)));
    connect(apps, SIGNAL(updateDone(QStringList,QStringList)), applicationsTab, SLOT(updateListViewModel(QStringList,QStringList)));
    connect(heuristics, SIGNAL(updateDone(QStringList)), heuristicsTab, SLOT(updateListViewModel(QStringList)));

    // Connect action menus with shortcuts to right functions
    connect(ui->actionAdd_MPSoC, SIGNAL(triggered(bool)), simulationTab, SLOT(on_buttonAddMPSoC_clicked()));
    connect(ui->actionStep_Back, SIGNAL(triggered(bool)), simulationTab, SLOT(on_buttonPrevStep_clicked()));
    connect(ui->actionStep_Foward, SIGNAL(triggered(bool)), simulationTab, SLOT(on_buttonNextStep_clicked()));
    connect(ui->action_Reset_Simulation, SIGNAL(triggered(bool)), simulationTab, SLOT(on_ButtonReset_clicked()));
    connect(ui->actionAuto_Step, SIGNAL(toggled(bool)), simulationTab, SLOT(on_autoStepToggle(bool)));
    connect(ui->action_Open_Applications_Directory, SIGNAL(triggered(bool)), applicationsTab, SLOT(on_pushButtonOpenDirectory_clicked()));
    connect(ui->action_Open_Heuristics_Directory, SIGNAL(triggered(bool)), heuristicsTab, SLOT(on_pushButtonOpenHeuristicsDirectory_clicked()));
    connect(ui->actionLog, SIGNAL(triggered(bool)), this, SLOT(openLogFile()));

    // SetUp widgetStack's tabs of left panel buttons
    ui->widgetStack->addWidget(simulationTab);
    ui->widgetStack->addWidget(applicationsTab);
    ui->widgetStack->addWidget(heuristicsTab);

    ui->widgetStack->setCurrentWidget(simulationTab);

    // SetUp Thread Worker, for sideloading
    ApplicationLoader *worker = new ApplicationLoader;

    apps->moveToThread(worker);         // Controllers must go to auxiliary thread to right set children relationship
    heuristics->moveToThread(worker);

    connect(worker, SIGNAL(finished()), this, SLOT(loadingDone()));                             // Notify main window
    connect(worker, SIGNAL(finished()), simulationTab, SLOT(updateView()));                     // Notify widget to enable buttons
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));                           // Auto delete worker after job done
    connect(worker, SIGNAL(status(QString,int)), statusBar(), SLOT(showMessage(QString,int)));  // Show messages from worker to Status Bar

    worker->start();
}

MPSoC_Simulator::~MPSoC_Simulator() {
    delete ui;
}

/**
 * @brief ApplicationLoader::run worker job, load applications and heuristics files in an auxiliary thread
 */
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
    if (heuristics->count() == 0) {
        QMessageBox alert;
        alert.setText(QString(tr("The application could not find any heuristic, you should add at least one in matter to proceed with simulations.")));
        alert.setStandardButtons(QMessageBox::Ok | QMessageBox::Help);
        alert.setDefaultButton(QMessageBox::Ok);
        alert.setWindowTitle(windowTitle());
        alert.setIcon(QMessageBox::Warning);

        int res = alert.exec();
        switch (res) {
        case QMessageBox::Help:
            // TODO
            break;
        case QMessageBox::Ok:
            ui->heuristicsPushButton->animateClick();
            break;
        default:
            break;
        }
    }
}

/**
 * @brief MPSoC_Simulator::closeEvent actions before exit
 * @param event
 */
void MPSoC_Simulator::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, windowTitle(), QString(tr("Are you sure you want to close %1?")).arg(windowTitle()),
                                QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QSettings settings;
        settings.setValue("mainWindowGeometry", saveGeometry());    // Save window size/position
        settings.setValue("mainWindowState", saveState());          // Save window state

        event->accept();
    } else {
        event->ignore(); // Deny exit
    }
}

void MPSoC_Simulator::openLogFile() {
    QString logFile = QStandardPaths::standardLocations(QStandardPaths::DataLocation).first()+ "/log.txt";
    QDesktopServices::openUrl(QUrl::fromLocalFile(logFile));
}

void MPSoC_Simulator::on_applicationsPushButton_clicked() {
    ui->simulationPushButton->setChecked(false);
    ui->applicationsPushButton->setChecked(true);
    ui->heuristicsPushButton->setChecked(false);
    ui->widgetStack->setCurrentWidget(applicationsTab);
}

void MPSoC_Simulator::on_simulationPushButton_clicked() {
    ui->simulationPushButton->setChecked(true);
    ui->applicationsPushButton->setChecked(false);
    ui->heuristicsPushButton->setChecked(false);
    ui->widgetStack->setCurrentWidget(simulationTab);
}

void MPSoC_Simulator::on_heuristicsPushButton_clicked() {
    ui->simulationPushButton->setChecked(false);
    ui->applicationsPushButton->setChecked(false);
    ui->heuristicsPushButton->setChecked(true);
    ui->widgetStack->setCurrentWidget(heuristicsTab);
}
