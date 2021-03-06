#ifndef PORTGRAPHICSITEM_H
#define PORTGRAPHICSITEM_H

#include "Enumerations.h"
#include "SchematicElement.h"

#include <QBrush>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QPen>
#include <QPointF>

namespace q2d {
namespace gui {

// forward declaration
class ComponentGraphicsItem;

/**
 * @brief The PortGraphicsItem class
 * It has a QGraphicsEllipseItem as child to avoid inheritance conflicts
 * between QGraphicsItem and QObject and keep the code flexible, should one
 * decide, that an ellipse is not enough.
 */
class PortGraphicsItem
        : public SchematicElement {
    Q_OBJECT
private:

    static QPen PEN_INPUT_PORT;
    static QPen PEN_IN_OUT_PORT;
    static QPen PEN_OUTPUT_PORT;
    static QPen PEN_UNDEFINED_PORT;
    static QPen PEN_HOVER_PORT;

    static QBrush BRUSH_INPUT_PORT;
    static QBrush BRUSH_IN_OUT_PORT;
    static QBrush BRUSH_OUTPUT_PORT;
    static QBrush BRUSH_UNDEFINED_PORT;
    static QBrush BRUSH_HOVER_PORT;

    QPen    m_defaultPen;
    QBrush  m_defaultBrush;
    QPointF m_dragStartPosition;

    // for drag and drop
    bool    m_dragOver;
    static bool    m_wireDrawingMode;
    static QGraphicsLineItem*  m_wireDrawingLineItem;
    static QPointF m_wireDrawingStart;
    static QPointF m_wireDrawingEnd;
    static QLineF* m_wireDrawingLine;

    void performDrag();

protected:
    model::enums::PortDirection m_direction;
    virtual QString specificType() override;

public:

    /**
     * @brief PortGraphicsItem
     * This is the more general constructor if one needs more special things
     * like input/output ports for the schematic.
     * @param relativeCenterPosition
     * @param direction
     * @param scene
     * @param parent
     */
    explicit PortGraphicsItem(QPointF position, DocumentEntry* relatedEntry,
                              model::enums::PortDirection direction);

    model::enums::PortDirection direction() const {
        return m_direction;
    }

    // ovverride for custom dragging
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent* event);
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent* event);
    virtual void dropEvent(QGraphicsSceneDragDropEvent* event);

    // cosmetic overrides
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

    static bool wireDrawingMode();
    static void setWireDrawingMode(bool mode, QPointF* origin = nullptr);

signals:
    void signal_posChanged();

public slots:
    void slot_drawConnected();

};

} // namespace gui
} // namespace q2d

#endif // PORTGRAPHICSITEM_H
