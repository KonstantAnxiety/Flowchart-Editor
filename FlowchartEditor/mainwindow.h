#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphicsview.h"

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
    void on_addTerminalBtn_clicked();
    void on_pushButton_clicked();
    void on_addDecisionBtn_clicked();
    void openFlowchart();
    void exportFlowchart();
    void saveFlowchart();
    void saveFlowchartAs();
    void exit();

    void on_addConnectionBtn_clicked();

    void on_addLabelBtn_clicked();

private:
    void closeEvent(QCloseEvent *event);
    void createActions();
    void createMenus();
    bool askForSave();
    Ui::MainWindow *ui;
    GraphicsView *chart;
    QString path;
    QMenu *fileMenu;
    QAction *newAct;
    QAction *exportAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
};
#endif // MAINWINDOW_H
