#ifndef COMPONENTTREEVIEW_H
#define COMPONENTTREEVIEW_H

#include <QMouseEvent>
#include <QPoint>
#include <QSize>
#include <QTreeView>

namespace q2d {
namespace gui {

class ComponentTreeView : public QTreeView {
private:
    // TODO make changeable via settings
    static QSize ICON_SIZE;

    QPoint dragStartPosition;

public:
    explicit ComponentTreeView(QWidget *parent = 0);

    // ovverride for custom drag event
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:

public slots:

};

} // namespace gui
} // namespace q2d

#endif // COMPONENTTREEVIEW_H
