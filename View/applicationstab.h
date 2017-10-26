#ifndef APPLICATIONSTAB_H
#define APPLICATIONSTAB_H

#include <QWidget>

namespace View {

namespace Ui {
class ApplicationsTab;
}

class ApplicationsTab : public QWidget {

public:
    explicit ApplicationsTab(QWidget *parent = 0);
    ~ApplicationsTab();

private:
    Ui::ApplicationsTab *ui;
};


} // namespace View
#endif // APPLICATIONSTAB_H
