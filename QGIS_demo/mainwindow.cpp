#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  logAllProviderName();

  QGridLayout* layout = new QGridLayout(ui->centralwidget);
  ui->centralwidget->setLayout(layout);
  layout->setContentsMargins(0, 0, 0, 0);
  _map_canvas = new QgsMapCanvas(ui->centralwidget);
  layout->addWidget(_map_canvas);

  initCanvas(*_map_canvas);

  initLayerTree();

  addMemoryLayer();

  addShpfile();

  addS57();

  addOnlineWMS();

  addOfflineWMS();

  _map_canvas->zoomToFullExtent();
  _map_canvas->refresh();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::initCanvas(QgsMapCanvas& canvas) {
  canvas.setObjectName(QStringLiteral("theMapCanvas"));
  canvas.setFocus();
  canvas.enableAntiAliasing(true);
  canvas.setCanvasColor(QColor(200, 200, 200));
  canvas.setDestinationCrs(QgsCoordinateReferenceSystem::fromEpsgId(3857));
  canvas.freeze(false);

  QObject::connect(&canvas, &QgsMapCanvas::xyCoordinates,
                   [this](const QgsPointXY& xy) {
                     this->statusBar()->showMessage(
                         QString("xy:(%1,%2)").arg(xy.x()).arg(xy.y()));
                   });
}

void MainWindow::initLayerTree() {
  QgsProject* project = QgsProject::instance();

  QgsLayerTree* tree_root = project->layerTreeRoot();
  QObject::connect(
      tree_root, &QgsLayerTree::visibilityChanged, [](QgsLayerTreeNode* node) {
        qDebug() << __func__ << "signal__visibilityChanged:" << node->name();
      });

  QgsLayerTreeModel* model = new QgsLayerTreeModel(tree_root, this);
  model->setFlag(QgsLayerTreeModel::AllowNodeReorder);
  model->setFlag(QgsLayerTreeModel::AllowNodeRename);
  model->setFlag(QgsLayerTreeModel::AllowNodeChangeVisibility);
  model->setFlag(QgsLayerTreeModel::ShowLegendAsTree);
  model->setFlag(QgsLayerTreeModel::UseEmbeddedWidgets);
  model->setFlag(QgsLayerTreeModel::UseTextFormatting);
  model->setAutoCollapseLegendNodes(10);

  QDockWidget* dock_wgt = new QDockWidget;
  dock_wgt->setObjectName("layer_tree_view_dock");
  QgsLayerTreeView* layer_tree_view = new QgsLayerTreeView(dock_wgt);
  layer_tree_view->setModel(model);
  QWidget* w = new QWidget;
  QVBoxLayout* vbox_layout = new QVBoxLayout;
  w->setLayout(vbox_layout);
  vbox_layout->setContentsMargins(0, 0, 0, 0);
  QToolBar* tool_bar = new QToolBar;
  vbox_layout->addWidget(tool_bar);
  vbox_layout->addWidget(layer_tree_view);
  dock_wgt->setWidget(w);
  dock_wgt->setAllowedAreas(Qt::LeftDockWidgetArea);
  this->addDockWidget(Qt::LeftDockWidgetArea, dock_wgt);

  QgsLayerTreeMapCanvasBridge* tmc_bridge =
      new QgsLayerTreeMapCanvasBridge(tree_root, _map_canvas, this);
  tmc_bridge->setObjectName("layer_tree_mapcanvas_bridge.");
  QObject::connect(
      tmc_bridge, &QgsLayerTreeMapCanvasBridge::canvasLayersChanged,
      [](const QList<QgsMapLayer*>& layers) {
        qDebug() << __func__
                 << "signal__canvasLayersChanged:" << layers.count();
      });
}

void MainWindow::addMemoryLayer() {
  // geometry type: point, include Z dimension, include M values.
  // EPSG: 4326.
  // field lists:
  // field_text_len10: field type=text, length=10.
  // field_whole-number_len10: field type=whole number, length=10.
  // field_decimal-number_len10_precision6: field type=decimal number,
  //                                        length=10, precision=6.
  // ...
  const QString uri = QStringLiteral(
      "PointZM?crs=EPSG:4326&field=field_text_len10:string(10,0)&field=field_"
      "whole-number_len10:integer(10,0)&field=field_decimal-number_len10_"
      "precision6:double(10,6)&field=field_boolean:boolean(0,0)&field=field_"
      "date:date(0,0)&field=field_time:time(0,0)&field=field_date-and-time:"
      "datetime(0,0)&field=field_binary-BLOB:string(0,0)");

  const QString name = QStringLiteral("vector_memory");
  const QString provider = QStringLiteral("memory");
  QgsVectorLayer* layer = new QgsVectorLayer(uri, name, provider);
  layer->setCrs(QgsCoordinateReferenceSystem::fromEpsgId(4326));

  layer->startEditing();

  {
    QgsField field;
    field.setName("add_unlimited-length-text");
    field.setType(QVariant::String);
    const bool ok = layer->addAttribute(field);
    if (!ok) {
      qDebug() << "add_field_to_memory_layer:" << ok;
    }

    QgsFields fields = layer->fields();
    qDebug() << "memory_layer_fields:" << fields.names();
  }

  {
    QgsFeature feature;
    feature.initAttributes(layer->fields().count());

    // attr0, field_text_len10, length=10, but attribute.length=11.
    feature.setAttribute(0, QByteArray("01234567890"));

    const bool ok = layer->addFeature(feature);
    if (!ok) {
      qDebug() << "add_feature_to_memory_layer:" << ok;
    }

    qDebug() << "attr-field_text_len10:"
             << feature.attribute(0);  // 0123456789, length=11.

    QgsFeatureIds ids = layer->allFeatureIds();
    Q_ASSERT(1 == ids.count());
    if (1 == ids.count()) {
      QgsFeature ftr = layer->getFeature(*ids.begin());
      QVariant attr = ftr.attribute(0);
      qDebug() << "out_range_field_length:" << attr;  // 01234567890
    }

    qDebug() << "memory_add_feature:" << layer->featureCount();
  }

  QgsProject::instance()->addMapLayer(layer);
}

void MainWindow::addShpfile() {
  const QString uri = QStringLiteral("./mapdata/shp/gadm36_CHN_3.shp");
  const QString name = QStringLiteral("gadm36_CHN_3");
  const QString provider = QStringLiteral("ogr");
  QgsVectorLayer* layer = new QgsVectorLayer(uri, name, provider);

  { layer->setCrs(QgsCoordinateReferenceSystem::fromEpsgId(4326)); }

  QgsProject::instance()->addMapLayer(layer);
}

void MainWindow::addS57() {
  //
}

void MainWindow::addOnlineWMS() {
  // type=xyz&zmin=1&zmax=18&url=http://ecn.t3.tiles.virtualearth.net/tiles/a{q}.jpeg?g%3D0%26dir%3Ddir_n'
  const QString uri = QStringLiteral(
      "type=xyz&zmin=1&zmax=18&url=http://ecn.t3.tiles.virtualearth.net/tiles/"
      "a{q}.jpeg?g%3D0%26dir%3Ddir_n'");
  const QString name = QStringLiteral("bing_satellite_online");

  addWMS(uri, name);
}

void MainWindow::addOfflineWMS() {
  const QString uri = QStringLiteral(
      "type=xyz&zmin=1&zmax=18&url=file:///home/dspon/lisibh/resources/"
      "maptiles/"
      "maptiles/bing_sattelite/{z}/{x}/{y}.jpg");
  const QString name = QStringLiteral("bing_satellite_offline");

  addWMS(uri, name);
}

void MainWindow::addWMS(const QString& uri, const QString& name) {
  const QString provider = QStringLiteral("wms");
  QgsRasterLayer* layer = new QgsRasterLayer(uri, name, provider);
  layer->setCrs(QgsCoordinateReferenceSystem::fromEpsgId(3857));
  if (!layer->isValid()) {
    qDebug() << "raster lyer not valid," << name << ","
             << layer->error().message();
    for (const auto& one : layer->error().messageList()) {
      qDebug() << "error:" << one.message();
    }
  }

  QgsProject::instance()->addMapLayer(layer);
}

void MainWindow::transform_demo() {
  //
}

void MainWindow::logAllProviderName() {
  auto pr = QgsProviderRegistry::instance();
  qDebug() << "allProviderName:" << pr->providerList() << pr->pluginList()
           << pr->libraryDirectory();
}
