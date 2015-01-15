#ifndef MODELELEMENT_H
#define MODELELEMENT_H

#include <QObject>
#include <QString>

namespace q2d {
namespace model {

    // forward declaration
    class Model;

// TODO documentation
// TODO visitor-Pattern
class ModelElement : public QObject {
    Q_OBJECT
private:
    QString name;
public:
    ModelElement(QString name, Model* parent = nullptr);
};

} // namespace model
} // namespace q2d

#endif // MODELELEMENT_H
