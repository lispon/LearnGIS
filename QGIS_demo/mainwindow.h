#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qgslayertree.h>
#include <qgslayertreemapcanvasbridge.h>
#include <qgslayertreemodel.h>
#include <qgslayertreeview.h>
#include <qgsmapcanvas.h>
#include <qgsproject.h>
#include <qgsproviderregistry.h>
#include <qgsrasterlayer.h>

#include <QDockWidget>
#include <QGridLayout>
#include <QMainWindow>
#include <QToolBar>

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
  void addOnlineWMS();
  void addOfflineWMS();

 private:
  Ui::MainWindow* ui;
  QgsMapCanvas* _map_canvas;
  void initCanvas(QgsMapCanvas& canvas);
  void initLayerTree();

  void addWMS(const QString& uri, const QString& name);

 private:
  void transform_demo();
  void logAllProviderName();
};
#endif  // MAINWINDOW_H
