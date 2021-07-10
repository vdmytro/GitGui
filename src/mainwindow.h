#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GitInterface/gitinterface.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>
#include <QListWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btReload_clicked();

    void on_actionOpen_triggered();

    void on_actionClone_triggered();

    void on_actionNew_triggered();

    void on_actionQuit_triggered();

    void on_btAddChanges_clicked();

    void on_cbBranch_textActivated(const QString &arg1);

    void on_btCommit_clicked();

    void on_lwUnstaged_itemClicked(QListWidgetItem *item);

    void on_lwStaged_itemClicked(QListWidgetItem *item);

    void on_btPush_clicked();

private:
    Ui::MainWindow *ui;

    GitInterface Git;

    void uppdateUi();
};
#endif // MAINWINDOW_H
