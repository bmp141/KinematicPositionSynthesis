#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QGraphicsItemGroup>
#include <QLineSeries>
#include <QGraphicsItem>
#include <QtCharts/QLineSeries>

namespace Ui {
class MainWindowForm;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_actionExit_triggered();
    void on_timer();

protected:
    QGraphicsItem* create_limb(QGraphicsItem *parent,QColor color);

    void create_chart();

    void create_scene();

    void create_timer();

    void update_chart();

private:
    Ui::MainWindowForm *mMainWindowUI;
    QGraphicsItem *base;
    QGraphicsItem *limb;
    QGraphicsEllipseItem *endEffector;
    QLineSeries *series;
    int plottingCount{0};
    int baseAngle{0};
    int limbAngle{0};
};

#endif // MAINWINDOW_H
