#include "vectormaplayer.h"

VectorMapLayer::VectorMapLayer(char demo_index)
    : QgsVectorLayer() {
    const static std::vector<std::pair<QString, QString>> vec_path_name = {
        { "/home/dspon/Projects/online/LearnGIS/map_data/ne_10m_minor_islands/ne_10m_minor_islands.shp", "islands-shp" },
        { "./mapdata/s57/EA200001.000", "EA200001" },
    };
    QString path, name, provider;
    if (0 == demo_index) {
        const auto& shp = vec_path_name.at(0);
        path = shp.first;
        name = shp.second;
        provider = "ogr";
    } else if (1 == demo_index) {
        const auto& s57 = vec_path_name.at(1);
        path = s57.first;
        name = s57.second;
        provider = "ogr"; // vector layer using ogr, raster layer using gdal.
    }

    this->setDataSource(path, name, provider, QgsDataProvider::ProviderOptions{ QgsProject::instance()->transformContext() });
    std::cout << "VectorMapLayer:" << name.toStdString() << " isValid:" << this->isValid() << std::endl;
    if (1 == demo_index) {
        QgsVectorDataProvider* dp = this->dataProvider();
        dp;
        const auto sublayers_name = dp->subLayers();
        for (auto&& one : sublayers_name) {
            qDebug() << "sublayer:" << one;
            const auto names = one.split(dp->SUBLAYER_SEPARATOR);
            qDebug() << "name:" << names.at(1);
        }

        qDebug() << "s57 sublayer_separator:" << dp->SUBLAYER_SEPARATOR;

        qDebug() << "sublayer:" << this->subLayers().count();
    }

//    this->setCrs(QgsCoordinateReferenceSystem::fromEpsgId(4326));
}

VectorMapLayer::~VectorMapLayer() {
}
