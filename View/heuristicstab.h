#ifndef HEURISTICSTAB_H
#define HEURISTICSTAB_H

#include <QFrame>
#include <QDir>
#include <QStandardPaths>
#include <QUrl>
#include <QDesktopServices>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QFileDialog>
#include <Core/heuristiccontroller.h>

namespace View {

namespace Ui {
class HeuristicsTab;
}

class HeuristicsTab : public QFrame
{
    Q_OBJECT

public:
    explicit HeuristicsTab(QWidget *parent = 0);
    ~HeuristicsTab();

public slots:
    void on_pushButtonOpenHeuristicsDirectory_clicked();

private slots:
    void on_pushButtonNew_clicked();
    void on_pushButtonCloneHeuristic_clicked();
    void on_pushButtonDeleteHeuristic_clicked();
    void on_pushButtonExternaEditorHeuristic_clicked();
    void on_pushButtonHeuristicLog_clicked();
    void on_pushButtonSaveHeuristic_clicked();
    void on_pushButtonCancelHeuristic_clicked();

    void on_textEditScript_textChanged();
    void on_textEditDescription_textChanged();

    void on_comboBoxHeuristics_currentIndexChanged(const QString &selection);
    void updateListViewModel(const QStringList &list);
    void externalEditorClosed();

    void clearEditor();

    void on_pushButtonOpenHeuristicsDirectory_2_clicked();

private:
    Ui::HeuristicsTab *ui;

    Core::Heuristic *heuristicEdit;
};


} // namespace View
#endif // HEURISTICSTAB_H
