#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QGridLayout* layout = new QGridLayout(ui->centralwidget);
    ui->centralwidget->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    _map_canvas = new QgsMapCanvas(ui->centralwidget);
    layout->addWidget(_map_canvas);


    initCanvas(*_map_canvas);

    initLayerTree();

    addShpfile();

    addS57();

    addLocalTMS();

    addWMS();


    _map_canvas->zoomToFullExtent();
    _map_canvas->refresh();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initCanvas(QgsMapCanvas& canvas) {
    canvas.setObjectName(QStringLiteral("theMapCanvas"));
    canvas.setFocus();
    canvas.enableAntiAliasing(true);
    canvas.setCanvasColor(QColor(200, 200, 200));
    canvas.setDestinationCrs(QgsCoordinateReferenceSystem::fromEpsgId(3857));
    canvas.freeze(false);

    QObject::connect(&canvas, &QgsMapCanvas::xyCoordinates, [this](const QgsPointXY & xy) {
        this->statusBar()->showMessage(QString("xy:(%1,%2)").arg(xy.x()).arg(xy.y()));
    });
}

void MainWindow::initLayerTree() {
    QgsProject* project = QgsProject::instance();

    QgsLayerTree* tree_root = project->layerTreeRoot();
    QObject::connect(tree_root, &QgsLayerTree::visibilityChanged, [](QgsLayerTreeNode * node) {
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

    QgsLayerTreeMapCanvasBridge* tmc_bridge = new QgsLayerTreeMapCanvasBridge(tree_root, _map_canvas, this);
    tmc_bridge->setObjectName("layer_tree_mapcanvas_bridge.");
    QObject::connect(tmc_bridge, &QgsLayerTreeMapCanvasBridge::canvasLayersChanged, [](const QList<QgsMapLayer*>& layers) {
        qDebug() << __func__ << "signal__canvasLayersChanged:" << layers.count();
    });
}


void MainWindow::addShpfile() {
    const QString uri = QStringLiteral("./mapdata/shp/gadm36_CHN_3.shp");
    const QString name = QStringLiteral("gadm36_CHN_3");
    QgsVectorLayer* layer = new QgsVectorLayer(uri, name, "ogr");

    {
        layer->setCrs(QgsCoordinateReferenceSystem::fromEpsgId(4326));
    }

    QgsProject::instance()->addMapLayer(layer);
}

void MainWindow::addS57() {
    //
}

void MainWindow::addLocalTMS() {
    //
}

void MainWindow::addWMS() {
    //
}

void MainWindow::transform_demo() {
    //
}
