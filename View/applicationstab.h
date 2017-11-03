#ifndef APPLICATIONSTAB_H
#define APPLICATIONSTAB_H

#include <QFrame>
#include <QItemSelection>
#include <QStringListModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <Core/applicationcontroller.h>

namespace View {

namespace Ui {
class ApplicationsTab;
}

class ApplicationsTab : public QFrame
{
    Q_OBJECT

public:
    explicit ApplicationsTab(QWidget *parent = 0);
    ~ApplicationsTab();

public slots:
    void updateListViewModel(const QStringList &applications, const QStringList &groups);
    void on_pushButtonCreateNewApplicationGroup_clicked();
    void on_pushButtonCreateApplicationGroupFromFile_clicked();
    void on_pushButtonSaveApplicationGroup_clicked();
    void on_pushButtonCancelApplicationGroup_clicked();
    void on_listViewApplicationsGroup_selectionModel_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void on_pushButtonOpenDirectory_clicked();

private slots:

    void clearEditor();

private:
    Ui::ApplicationsTab *ui;

    QStringListModel *listModelApplicationsGroup;
    Core::ApplicationGroup *editApplicationsGroup;
};


} // namespace View
#endif // APPLICATIONSTAB_H
