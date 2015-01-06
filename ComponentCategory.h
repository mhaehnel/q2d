#ifndef COMPONENTCATEGORY_H
#define COMPONENTCATEGORY_H

#include "ComponentDescriptor.h"

namespace q2d {

class ComponentCategory : public ComponentDescriptor {
    // TODO signals and slots
    // TODO later: mouse-over descriptions
    // TODO later: translations for names?
public:
    explicit ComponentCategory(QString name, ComponentCategory* parent = 0);

    void addSubItem(ComponentDescriptor *toAdd);
};

} // namespace q2d

#endif // COMPONENTCATEGORY_H
