
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
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

    QTimer::singleShot(0, this,  SLOT(loadApplications()));

    restoreState(settings.value("mainWindowState").toByteArray());
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::loadApplications() {
    QStringListModel *applicationsListModel = new QStringListModel();
    QStringListModel *runningListModel = new QStringListModel();

    this->statusBar()->showMessage("Loading applications files...");

    QDir applications("Applications");
    applications.setNameFilters(QStringList()<<"*.json");

    int loadMax = applications.count();
    int loadValue = 0;

    statusProgress->setMaximum(loadMax);
    statusProgress->setValue(loadValue);

    foreach (QString s, applications.entryList()) {
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
            // TODO Add to real applications Object List
            loadValue++;
            statusProgress->setValue(loadValue);
            applicationsListModel->setData(applicationsListModel->index(index + i), "[" + appsName + "] " + array.at(i).toObject().value("name").toString());
        }
    }
    ui->applicationsList->setModel(applicationsListModel);
    ui->runningList->setModel(runningListModel);
    connect(ui->applicationsList->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_applicationsList_changed(QItemSelection)));
    connect(ui->runningList->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_runningList_changed(QItemSelection)));

    statusProgress->setVisible(false);
    this->statusBar()->showMessage("Read applications done...", 2000);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, windowTitle(), "Are you sure you want to close MPSoC Emulator?",
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

void MainWindow::on_addMPSoCButton_clicked() {
    bool ok = false;
    int x = QInputDialog::getInt(this,windowTitle(),"Entre o tamanho X:",2,2,16,1,&ok);
    if (!ok) return;
    int y = QInputDialog::getInt(this,windowTitle(),"Entre o tamanho Y:",2,2,16,1,&ok);
    if (!ok) return;
    QString heuristic = QInputDialog::getText(this,windowTitle(),"Heuristica:",QLineEdit::Normal,"FirstFree", &ok);
    if (!ok) return;

    Core::MPSoC* m = new Core::MPSoC(x,y);

    QHBoxLayout* layout =  ui->mpsocsLayout;

    View::MPSoCBox* newmpsoc = new View::MPSoCBox();
    newmpsoc->setMPSoC(m);
    newmpsoc->setHeuristic(heuristic);

    layout->insertWidget(layout->count() - 1, newmpsoc);

    //timer->start(350);
}

void MainWindow::on_applicationsList_changed(const QItemSelection& selection) {
    ui->runApplicationPushButton->setEnabled(selection.count() > 0);
}

void MainWindow::on_runningList_changed(const QItemSelection& selection) {
    ui->killApplicationPushButton->setEnabled(selection.count() > 0);
}

void MainWindow::on_timerSpinBox_valueChanged(int val) {
    thread->setInterval(val);
}

void MainWindow::on_stepSlider_valueChanged(int val) {
    ui->stepCounter->setText(QString("%1/%2").arg(val).arg(ui->stepSlider->maximum()));
}

void MainWindow::on_applicationsPushButton_clicked() {
    ui->emulationPushButton->setChecked(false);
    ui->applicationsPushButton->setChecked(true);
    ui->heuristicsPushButton->setChecked(false);
    ui->widgetStack->setCurrentWidget(ui->pageApplications);
}

void MainWindow::on_emulationPushButton_clicked() {
    ui->emulationPushButton->setChecked(true);
    ui->applicationsPushButton->setChecked(false);
    ui->heuristicsPushButton->setChecked(false);
    ui->widgetStack->setCurrentWidget(ui->pageEmulator);
}

void MainWindow::on_heuristicsPushButton_clicked() {
    ui->emulationPushButton->setChecked(false);
    ui->applicationsPushButton->setChecked(false);
    ui->heuristicsPushButton->setChecked(true);

    //TODO
}
