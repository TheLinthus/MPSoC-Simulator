#include "newmpsocdialog.h"
#include "ui_newmpsocdialog.h"

namespace View {

NewMPSoCDialog::NewMPSoCDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewMPSoCDialog),
    masterX(0),
    masterY(0),
    w(2),
    h(2)
{
    ui->setupUi(this);

    ui->comboBox->setModel(new QStringListModel(heuristics->getHeuristicsList()));

    for (int i = 0; i < 2; ++ i)
        for (int j = 0; j < 2; ++ j) {
            QRadioButton *b = new QRadioButton("", this);
            if (i == 0 && j == 0) {
                b->setChecked(true);
            }
            ui->gridLayout->addWidget(b, i, j);
            ui->gridLayout->setAlignment(b, Qt::AlignCenter);
            connect(b, SIGNAL(clicked(bool)), this, SLOT(masterChoose()));
        }
}

NewMPSoCDialog::~NewMPSoCDialog() {
    delete ui;
}

int NewMPSoCDialog::getW() {
    return ui->xSpinBox->value();
}

int NewMPSoCDialog::getH() {
    return ui->ySpinBox->value();
}

QPoint NewMPSoCDialog::getMaster() const {
    return QPoint(masterX, masterY);
}

QString NewMPSoCDialog::getHeuristic() {
    return ui->comboBox->currentText();
}

void NewMPSoCDialog::masterChoose() {
    QWidget *sender = (QWidget *) QObject::sender();

    int index = ui->gridLayout->indexOf(sender);
    int rs, cs;
    ui->gridLayout->getItemPosition(index, &masterY, &masterX, &rs, &cs);
    ui->masterAddr->setText(QString("%1, %2").arg(masterX).arg(masterY));
}

} // namespace View

void View::NewMPSoCDialog::on_xSpinBox_valueChanged(int x) {
    if (x == w) return;
    int min = qMin(x, w);
    int max = qMax(x, w);
    for (int i = 0; i < h; i++)
        for (int j = min; j < max; j++) {
            if (x < w) {
                ui->gridLayout->itemAtPosition(i,j)->widget()->deleteLater();
                ui->gridLayout->removeItem(ui->gridLayout->itemAtPosition(i,j));
            } else {
                QRadioButton *b = new QRadioButton("", this);
                ui->gridLayout->addWidget(b, i, j);
                ui->gridLayout->setAlignment(b, Qt::AlignCenter);
                connect(b, SIGNAL(clicked(bool)), this, SLOT(masterChoose()));
            }
        }
    if (masterX >= x) {
        masterX = x - 1;
        ui->masterAddr->setText(QString("%1, %2").arg(masterX).arg(masterY));
        ((QRadioButton *) ui->gridLayout->itemAtPosition(masterY,masterX)->widget())->setChecked(true);
    }
    w = x;
}

void View::NewMPSoCDialog::on_ySpinBox_valueChanged(int y) {
    if (y == h) return;
    int min = qMin(y, h);
    int max = qMax(y, h);
    for (int j = 0; j < w; j++)
        for (int i = min; i < max; i++) {
            if (y < h) {
                ui->gridLayout->itemAtPosition(i,j)->widget()->deleteLater();
                ui->gridLayout->removeItem(ui->gridLayout->itemAtPosition(i,j));
            } else {
                QRadioButton *b = new QRadioButton("", this);
                ui->gridLayout->addWidget(b, i, j);
                ui->gridLayout->setAlignment(b, Qt::AlignCenter);
                connect(b, SIGNAL(clicked(bool)), this, SLOT(masterChoose()));
            }
        }
    if (masterY >= y) {
        masterY = y - 1;
        ui->masterAddr->setText(QString("%1, %2").arg(masterX).arg(masterY));
        ((QRadioButton *) ui->gridLayout->itemAtPosition(masterY,masterX)->widget())->setChecked(true);
    }
    h = y;
}
