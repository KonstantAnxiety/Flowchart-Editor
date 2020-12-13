#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <fstream>
#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createMenus();

    chart = new GraphicsView(parent);
    ui->graphicsLayout->addWidget(chart);
}

MainWindow::~MainWindow()
{
    delete chart;
    delete fileMenu;
    delete exportAct;
    delete ui;
}


void MainWindow::on_addTerminalBtn_clicked()
{
    chart->addNode(1);
}

void MainWindow::on_pushButton_clicked()
{
    chart->addNode(2);
}

void MainWindow::on_addDecisionBtn_clicked()
{
    chart->addNode(3);
}

void MainWindow::openFlowchart()
{
    if (!askForSave())
        return;
    path = QFileDialog::getOpenFileName(this, "Open flowchart",
                                        QCoreApplication::applicationDirPath(),
                                        "text file (*.txt)" );
    if (!path.isEmpty())
        chart->open(path);
}

void MainWindow::exportFlowchart()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save image",
                                                    QCoreApplication::applicationDirPath(),
                                                    "BMP Files (*.bmp);;JPEG (*.JPEG);;PNG (*.png)" );
    if (!fileName.isEmpty()) {
        chart->removeGrid();
        QPixmap pixMap = chart->grab();
        pixMap.save(fileName);
        chart->addGrid();
    }
}

void MainWindow::saveFlowchart()
{
    if (!chart->getPath().isEmpty())
        chart->save();
    else
        saveFlowchartAs();
}

void MainWindow::saveFlowchartAs()
{
    path = QFileDialog::getOpenFileName(this, "Save flowchart",
                                        QCoreApplication::applicationDirPath(),
                                        "text file (*.txt)" );
    if (!path.isEmpty())
        chart->saveAs(path);
}

void MainWindow::exit()
{
    if (!askForSave())
        return;
    QApplication::exit();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!askForSave())
        event->ignore();
    else
        QMainWindow::closeEvent(event);
}


void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip(tr("Save flowchart to a file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::openFlowchart);

    exportAct = new QAction(tr("&Export..."), this);
    exportAct->setShortcut(QKeySequence("Ctrl+E"));
    exportAct->setStatusTip(tr("Export flowchart as image"));
    connect(exportAct, &QAction::triggered, this, &MainWindow::exportFlowchart);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save flowchart to a file"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFlowchart);

    saveAsAct = new QAction(tr("&Save as..."), this);
    saveAsAct->setShortcut(QKeySequence("Ctrl+Shift+S"));
    saveAsAct->setStatusTip(tr("Save flowchart to a file"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveFlowchartAs);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(exportAct);
}

bool MainWindow::askForSave()
{
    if (!chart->isSaved()) {
        QMessageBox msgBox;
        msgBox.setText("The document has unsaved changes");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                                  QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
          case QMessageBox::Save:
              saveFlowchart();
              return true;
              break;
          case QMessageBox::Cancel:
              return false;
              break;
          default:
              return true;
              break;
        }
    }
    return true;
}

void MainWindow::on_addConnectionBtn_clicked()
{
    chart->addNode(5);
}

void MainWindow::on_addLabelBtn_clicked()
{
    chart->addNode(4);
}
