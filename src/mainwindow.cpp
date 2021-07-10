#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , Git(GitInterface())
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btReload_clicked()
{
    uppdateUi();
}

void MainWindow::on_actionOpen_triggered()
{
    //open dialog for choice .git dir
    QDir GitDir(QString(QFileDialog::getExistingDirectory(0,"Open Project Dir")));
    for(auto& element : GitDir.entryList(QDir::Dirs | QDir::Hidden)){
        if(element == ".git"){
            Git.setPath(GitDir.path().toStdString());
            uppdateUi();
            return;
        }
    }

}

void MainWindow::on_actionClone_triggered()
{
    //open dialog for choise path and clone reppos, choise this path for working
    bool bOk;
    QString str = QInputDialog::getText(0,
                                        "Input remote link to git repository:",
                                        "ssh/http:",
                                        QLineEdit::Normal,
                                        "",
                                        &bOk);
    if(bOk){
        QString selectedFolder = QString(QFileDialog::getExistingDirectory(0,"Select directory for project"));
        QStringList folders;
        folders = str.split('/');
        QString folderName = folders.back();
        folderName.remove(".git");
        Git.clone(str.toStdString(),selectedFolder.toStdString(),folderName.toStdString());
        uppdateUi();
    }
}

void MainWindow::on_actionNew_triggered()
{
    //open dialog for choise path and init git repository
    QString selectedFolder = QString(QFileDialog::getExistingDirectory(0,"Select directory for project"));
    Git.init(selectedFolder.toStdString());

    uppdateUi();
}

void MainWindow::on_actionQuit_triggered()
{
   close();
}

void MainWindow::on_btAddChanges_clicked()
{
    Git.add();
    uppdateUi();
}

void MainWindow::uppdateUi()
{
    Git.UppdateInfo();
    //----------------------------------------------------------
    ui->teChanges->clear();
    ui->teChanges->append(Git.getInfo().c_str());
    //----------------------------------------------------------
    ui->cbBranch->clear();
    for(auto& branch : Git.getBranches()){
        ui->cbBranch->addItem(branch.first.c_str());
    }
    for(auto& branch : Git.getBranches()){
        if(branch.second)
            ui->cbBranch->setCurrentText(branch.first.c_str());
    }
    //----------------------------------------------------------
     ui->lwStaged->clear();
     ui->lwUnstaged->clear();
     if( !Git.getFileList().empty()){
        for (auto& file : Git.getFileList())
        {
            if(file.m_Stage == Stage::staged)
                ui->lwStaged->addItem(file.m_FileName.c_str());
            if(file.m_Stage == Stage::unstaged)
                ui->lwUnstaged->addItem(file.m_FileName.c_str());
        }
     }
}


void MainWindow::on_cbBranch_textActivated(const QString &arg1)
{
    Git.setBranch(arg1.toStdString());
    uppdateUi();
}

void MainWindow::on_btCommit_clicked()
{
    if(!ui->teCommitMsg->document()->toPlainText().isEmpty())
    {
        Git.commit(ui->teCommitMsg->document()->toPlainText().toStdString());
    }
    ui->teCommitMsg->clear();
    uppdateUi();
}

void MainWindow::on_lwUnstaged_itemClicked(QListWidgetItem *item)
{
    ui->teChanges->clear();
    ui->teChanges->append(Git.geFileInfo(item->text().toStdString()).c_str());
}

void MainWindow::on_lwStaged_itemClicked(QListWidgetItem *item)
{
    ui->teChanges->clear();
    ui->teChanges->append(Git.geFileInfo(item->text().toStdString()).c_str());
}

void MainWindow::on_btPush_clicked()
{
    Git.push();
}
