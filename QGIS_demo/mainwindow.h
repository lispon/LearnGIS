#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qgslayertree.h>
#include <qgslayertreemapcanvasbridge.h>
#include <qgslayertreemodel.h>
#include <qgslayertreeview.h>
#include <qgsmapcanvas.h>
#include <qgsproject.h>

#include <QDockWidget>
#include <QGridLayout>
#include <QMainWindow>
#include <QToolBar>

#include "vectormaplayer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  void addShpfile();
  void addS57();
  void addLocalTMS();
  void addWMS();

 private:
  Ui::MainWindow* ui;
  QgsMapCanvas* _map_canvas;
  void initCanvas(QgsMapCanvas& canvas);
  void initLayerTree();

 private:
  void transform_demo();
};
#endif  // MAINWINDOW_H
