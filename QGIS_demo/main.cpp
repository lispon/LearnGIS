#include <qgsapplication.h>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
  QgsApplication a(argc, argv, true);
#if 1
  a.setPrefixPath("/usr", true);
#else
  a.setPrefixPath("usr", false);
  a.setPluginPath("/usr/lib/qgis/plugins");
#endif

  a.init(QString());
  a.initQgis();

  MainWindow w;
  w.show();

  return a.exec();
}
