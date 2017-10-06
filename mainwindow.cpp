
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer(this))
{
    QSettings settings;
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());

    QFile qss(":/stylesheet/mainwindow.qss");
    qss.open(QFile::ReadOnly);
    setStyleSheet(QLatin1String(qss.readAll()));

    ui->setupUi(this);

    applicationsList = new QStringListModel();
    qDebug() << "Reading applications files...";
    QDir applications("Applications");
    applications.setNameFilters(QStringList()<<"*.json");
    foreach (QString s, applications.entryList()) {
        QString text;
        QFile file("Applications\\"+s);
        qDebug() << s << "...";
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        text = file.readAll();
        file.close();
        QJsonDocument json = QJsonDocument::fromJson(text.toUtf8());
        if (json.isNull()) {
            qWarning() << "Error File";
        }
        QJsonObject obj = json.object();
        QString appsName = obj.value("name").toString();
        qDebug() << "Application set name: " << appsName;
        qDebug() << "Application set date: " << obj.value("date").toString();
        qDebug() << "Application set author: " << obj.value("author").toString();
        QJsonArray array = obj.value("applications").toArray();
        qDebug() << "Found" << array.size() << "applications";
        for (int i = 0; i < array.size(); i++) {
            qDebug() << array.at(i).toObject().value("name").toString();
            int index = applicationsList->rowCount();
            applicationsList->insertRow(index);
            applicationsList->setData(applicationsList->index(index), "[" + appsName + "]" + array.at(i).toObject().value("name").toString());
        }
    }
    ui->applicationsList->setModel(applicationsList);
    connect(ui->applicationsList->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_applicationsList_changed(QItemSelection)));
    connect(ui->runningList->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_runningList_changed(QItemSelection)));

    statusProgress = new QProgressBar(this);
    statusProgress->setMaximumSize(242,16);
    statusProgress->setTextVisible(false);

    statusProgress->setValue(0);
    statusProgress->setMinimum(0);
    statusProgress->setMaximum(0);
    statusProgress->setVisible(false);

    this->statusBar()->setContentsMargins(8,0,8,0);
    this->statusBar()->addPermanentWidget(statusProgress);
    this->statusBar()->showMessage("Inicializando",2000);

    restoreState(settings.value("mainWindowState").toByteArray());
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *) {
    QSettings settings;
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());
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
    timer->setInterval(val);
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
    //TODO!
}
