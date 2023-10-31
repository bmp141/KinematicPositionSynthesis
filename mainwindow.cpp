#include "mainwindow.h"
#include "ui_mainwindowform.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsItemGroup>
#include <QTimer>
#include <QObject>
#include <QChartView>
#include <QLineSeries>
#include <QChart>
#include <QColor>
#include <QPen>
#include <QValueAxis>
#include <QApplication>

MainWindow::MainWindow(QWidget* parent) :
  QMainWindow{parent},
  mMainWindowUI{new Ui::MainWindowForm}
{
  mMainWindowUI->setupUi(this);

  create_scene();
  create_chart();
  create_timer();
}

MainWindow::~MainWindow()
{
  delete mMainWindowUI;
}

void MainWindow::create_chart()
{
  QChart* chart = new QChart();
  series = new QLineSeries();
  QPen pen(QColor(255, 0, 0, 255));
  series->setPen(pen);

  chart->addSeries(series);
  chart->setTitle("End Effector Position");

  QValueAxis* axisX = new QValueAxis;
  axisX->setRange(-250, 250);
  axisX->setTickCount(5);
  axisX->setLabelFormat("%.2f");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis* axisY = new QValueAxis;
  axisY->setRange(-250, 250);
  axisY->setTickCount(5);
  axisY->setLabelFormat("%.2f");
  chart->addAxis(axisY, Qt::AlignLeft);

  series->attachAxis(axisX);
  series->attachAxis(axisY);
  mMainWindowUI->chartView->setRenderHint(QPainter::Antialiasing);
  mMainWindowUI->chartView->setChart(chart);
}

void MainWindow::create_scene()
{
  QGraphicsScene* scene = new QGraphicsScene(mMainWindowUI->graphicsView);
  scene->addLine(-100, 0, 100, 0);
  scene->addLine(0, -100, 0, 100);

  base=create_limb(nullptr, QColor(255, 0, 255, 127));
  scene->addItem(base);

  limb = create_limb(base, QColor(0, 0, 255, 127));
  limb->setPos(100, 0);

  endEffector = new QGraphicsEllipseItem(0, 0, 5, 5, limb);
  endEffector->setPos(105, 0);
  QBrush brush = Qt::SolidPattern;
  brush.setColor(QColor(255, 0, 0, 255));
  endEffector->setBrush(brush);

  mMainWindowUI->graphicsView->setScene(scene);
}

void MainWindow::create_timer()
{
  QTimer* timer = new QTimer(this);
  timer->setInterval(100);
  QObject::connect(timer, SIGNAL(timeout()), this, SLOT(on_timer()));
  timer->start();
}

QGraphicsItem* MainWindow::create_limb(QGraphicsItem* parent, QColor color)
{
  QBrush brush = Qt::SolidPattern;
  brush.setColor(color);

  QGraphicsItemGroup* base = new QGraphicsItemGroup(parent);
  QGraphicsEllipseItem* joint = new QGraphicsEllipseItem(-10, -10, 20, 20, base);
  joint->setBrush(brush);

  QGraphicsRectItem* rect = new QGraphicsRectItem(0, -10, 100, 20, base);
  rect->setBrush(brush);

  base->addToGroup(joint);
  base->addToGroup(rect);
  return base;
}

void MainWindow::on_actionExit_triggered()
{
  QApplication::quit();
}

void MainWindow::update_chart()
{
  double xpos=endEffector->scenePos().x();
  double ypos=endEffector->scenePos().y();
  series->append(xpos, -ypos);

  mMainWindowUI->chartView->update();
}

void MainWindow::on_timer()
{
  plottingCount++;
  baseAngle+=12;
  limbAngle+=5;

  base->setRotation(baseAngle);
  limb->setRotation(limbAngle);

  update_chart();

  if(plottingCount>100)
  {
    series->clear();
    plottingCount=0;
  }

  if(baseAngle>=360)
  {
    baseAngle=0;
  }

  if(limbAngle>=360)
  {
    limbAngle=0;
  }

}
