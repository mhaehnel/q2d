#ifndef PORT_H
#define PORT_H

#include "Node.h"

namespace q2d {
namespace model {

/**
 * @brief The Port class is an abstraction for data flow interfaces.
 * It thereby is a special case of nodes and inherits the sender/receiver semantics.
 * Additionally a port has a direction that specifies whether if the data flow goes into or out of the interfaced model element.
 */
class Port : public Node {
protected:
    enums::PortDirection m_direction;
public:

    // FIXME deprecated
    /**
     * @brief Port
     * @param direction
     * @param relatedEntry
     */
    Port(enums::PortDirection direction,
         DocumentEntry* relatedEntry);

    enums::PortDirection direction() const {
        return m_direction;
    }

    virtual QStringList nodeVariables() const override;
};

class ComponentPort : public Port {
private:
    Component* m_component;
public:
    ComponentPort(enums::PortDirection direction, Component* interfacedComponent, DocumentEntry* relatedEntry);

    /**
     * @brief component is a getter for the component the port provides an interface for.
     * @return the component this port is attached to.
     */
    Component* component() const;
};

// TODO class ModulePort
/**
 * @brief The ModulePort class abstracts connections of a whole module to the outside.
 * Keep in mind that ports are always viewed from the inside of a module,
 * so an input port acts like a source of data while an output port acts like a sink for the data flow.
 */
class ModulePort : public Port {

public:
    /**
     * @brief ModulePort
     * @param direction is the direction of the Port <b>as viewed from the outside of the module</b>.
     * It will be translated to the opposing direction to fit the <i>inside the module</i> point of view.
     * @param relatedEntry
     */
    ModulePort(enums::PortDirection direction, DocumentEntry* relatedEntry);

};

} // namespace model
} // namewspace q2d

#endif // PORT_H