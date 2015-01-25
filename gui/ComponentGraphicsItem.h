#ifndef COMPONENTGRAPHICSITEM_H
#define COMPONENTGRAPHICSITEM_H

#include "SchematicsSceneChild.h"
#include <QGraphicsSvgItem>

namespace q2d {

// forward declaration
namespace metamodel {
class ComponentType;
}

namespace gui {

class ComponentGraphicsItem : public SchematicsSceneChild {
    Q_OBJECT
private:
    QGraphicsSvgItem* m_actuals; // the actually item that gets displayed

public:
    explicit ComponentGraphicsItem(metamodel::ComponentType* type, SchematicsScene *scene, QPoint position);

signals:

public slots:

};

} // namespace gui
} // namespce q2d

#endif // COMPONENTGRAPHICSITEM_H
