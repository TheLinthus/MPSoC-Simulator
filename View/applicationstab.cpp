#include "applicationstab.h"
#include "ui_applicationstab.h"

ApplicationsTab::ApplicationsTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplicationsTab)
{
    ui->setupUi(this);
}

ApplicationsTab::~ApplicationsTab()
{
    delete ui;
}
