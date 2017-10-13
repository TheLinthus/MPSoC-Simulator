#include "newmpsocdialog.h"
#include "ui_newmpsocdialog.h"

namespace View {

NewMPSoCDialog::NewMPSoCDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewMPSoCDialog)
{
    ui->setupUi(this);

    ui->comboBox->setModel(new QStringListModel(heuristics->getHeuristicsList()));
}

NewMPSoCDialog::~NewMPSoCDialog() {
    delete ui;
}

int NewMPSoCDialog::getX() {
    return ui->xSpinBox->value();
}

int NewMPSoCDialog::getY() {
    return ui->ySpinBox->value();
}

QString NewMPSoCDialog::getHeuristic() {
    return ui->comboBox->currentText();
}

} // namespace View
