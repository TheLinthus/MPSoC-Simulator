#ifndef APPLICATIONSTAB_H
#define APPLICATIONSTAB_H

#include <QFrame>
#include <QItemSelection>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QInputDialog>
#include <QDesktopServices>
#include <QGraphicsScene>
#include <Core/applicationcontroller.h>
#include <View/viewappnode.h>

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
    void updateListViewModel(const QStringList &, const QStringList &groups);
    void on_pushButtonCreateNewApplicationGroup_clicked();
    void on_pushButtonCreateApplicationGroupFromFile_clicked();
    void on_pushButtonSaveApplicationGroup_clicked();
    void on_pushButtonCancelApplicationGroup_clicked();
    void on_listViewApplicationsGroup_selectionModel_selectionChanged(const QItemSelection &, const QItemSelection &unselected);
    void on_pushButtonOpenDirectory_clicked();
    void on_comboBoxApplications_currentIndexChanged(const QString &value);
    void on_pushButtonAddApplication_clicked();
    void on_pushButtonRemoveApplication_clicked();

private slots:

    void clearEditor();
    void on_lineEditApplicationGroupName_textEdited(const QString &arg1);
    void on_checkBoxApplicationGroupEnabled_toggled(bool checked);
    void on_lineEditApplicationGroupAuthor_textEdited(const QString &arg1);

private:
    Ui::ApplicationsTab *ui;

    QGraphicsScene *scene;
    QMap<int, View::AppNode *> graphicalNodes;

    QStringListModel *listModelApplicationsGroup;
    Core::ApplicationGroup *editApplicationsGroup;
    Core::Application *editApplication;
    QVector<Core::Application*> removeList;
    QVector<Core::Application*> addList;
    void addNodeToList(AppNode *node, const qreal width);
};


} // namespace View
#endif // APPLICATIONSTAB_H
