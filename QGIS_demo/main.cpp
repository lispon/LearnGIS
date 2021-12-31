#include <qgsapplication.h>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
  QgsApplication a(argc, argv, true);

  a.init(QString());
  a.initQgis();

  MainWindow w;
  w.show();

  return a.exec();
}
