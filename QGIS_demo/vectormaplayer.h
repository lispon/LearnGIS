#ifndef VECTORMAPLAYER_H
#define VECTORMAPLAYER_H

#include <qgsproject.h>
#include <qgsvectorlayer.h>

class VectorMapLayer : public QgsVectorLayer {
    Q_OBJECT

  public:
    explicit VectorMapLayer(char demo_index);
    ~VectorMapLayer() override;
};

#endif // VECTORMAPLAYER_H
