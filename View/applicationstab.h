#ifndef APPLICATIONSTAB_H
#define APPLICATIONSTAB_H

#include <QWidget>

namespace Ui {
class ApplicationsTab;
}

class ApplicationsTab : public QWidget
{
    Q_OBJECT

public:
    explicit ApplicationsTab(QWidget *parent = 0);
    ~ApplicationsTab();

private:
    Ui::ApplicationsTab *ui;
};

#endif // APPLICATIONSTAB_H
