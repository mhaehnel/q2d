#ifndef NODE_H
#define NODE_H

#include "../Enumerations.h"
#include "ModelElement.h"

#include <QList>
#include <QStringList>

namespace q2d {
namespace model {

// forward declaration
class Component;

// TODO documentation
class Node : public ModelElement {
private:
    QList<ModelElement*> m_drivenElements;
    ModelElement* m_driver;
public:
    Node(DocumentEntry* relatedEntry);

    // TODO rename to better reflect actual function
    void addDriver(ModelElement* driver);
    void addDrivenElement(ModelElement* drivenElement);

    ModelElement* driver() const {
        return m_driver;
    }

    QList<ModelElement*> drivenElements() const {
        return m_drivenElements;
    }

    virtual QString toString() const override;
};

// TODO documentation
// NOTE currently unused
class ConductorNode : public Node {
public:
    ConductorNode();
};

} // namespace model
} // namespace q2d

#endif // NODE_H
