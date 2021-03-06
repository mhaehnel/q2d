#ifndef SCHEMATICSVIEW_H
#define SCHEMATICSVIEW_H

//#include "SchematicsTab.h"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QGraphicsView>
#include <QWidget>

namespace q2d {
namespace model {
class Component;
}

namespace gui {
class Schematic;

class SchematicsView :
    public QGraphicsView {
    Q_OBJECT
protected:

    // override for custom handling
    // TODO need to forward to scene?
//    virtual void dragEnterEvent(QDragEnterEvent* event);
//    virtual void dragMoveEvent(QDragMoveEvent* event);
//    virtual void dragLeaveEvent(QDragLeaveEvent* event);
//    virtual void dropEvent(QDropEvent *event);

public:
    explicit SchematicsView(QWidget* parent = nullptr);

    void setScene(q2d::gui::Schematic* scene);

signals:
    void signal_componentDetailRequested(model::Component* component);

public slots:
    /**
     * @brief updateScene is overridden as a debug hook. It calls its parents method.
     * @param rects
     */
    void updateScene(const QList<QRectF> &rects);
};


} // namespace gui
} // namespace q2d


#endif // SCHEMATICSVIEW_H
