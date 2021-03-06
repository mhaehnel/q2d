#include "DocumentEntry.h"
#include "Document.h"
#include "Enumerations.h"

#include "factories/ToolTipFactory.h"
#include "model/ModelElement.h"

#include <QtDebug>

using namespace q2d;
using namespace q2d::core;
using namespace q2d::gui;
using namespace q2d::model;


Identifiable*
DocumentEntry::chooseParentIdentifier(Identifiable* document, Identifiable* parentEntry){
    if(parentEntry == nullptr){ return document; }
    return parentEntry;
}

DocumentEntry::DocumentEntry(QString id,
                             enums::DocumentEntryType type, Document* document,
                             DocumentEntry* parent)
    : QObject(document),
    Identifiable(id, chooseParentIdentifier(document, parent)) {

    Q_CHECK_PTR(document);

    m_type = type;
    m_modelElement = nullptr;
    m_schematicElement = nullptr;
    m_parent = parent;
    m_document = document;
}

void
DocumentEntry::setModelElement(ModelElement* modelElement) {
    Q_CHECK_PTR(modelElement);
    m_modelElement = modelElement;
    m_modelElement->setRelatedEntry(this);
    this->slot_updateToolTip();

    // connect signals and slots
    connect(m_modelElement, &ModelElement::signal_changed,
            this, &DocumentEntry::slot_updateToolTip);
}

void
DocumentEntry::setSchematicElement(SchematicElement* schematicElement) {
    Q_CHECK_PTR(schematicElement);
    m_schematicElement = schematicElement;
    this->slot_updateToolTip();
}


q2d::enums::DocumentEntryType
DocumentEntry::type() const {
    return m_type;
}

ModelElement*
DocumentEntry::modelElement() const {
    return m_modelElement;
}

SchematicElement*
DocumentEntry::schematicElement() const {
    return m_schematicElement;
}

DocumentEntry*
DocumentEntry::parent() const {
    return m_parent;
}

Document*
DocumentEntry::document() const {
    return m_document;
}

Model*
DocumentEntry::model() const {
    return m_document->model();
}

Schematic*
DocumentEntry::scene() const {
    return m_document->schematic();
}


void
DocumentEntry::slot_updateToolTip() {
    if (m_schematicElement == nullptr) {
        return;
    }

    m_schematicElement->setToolTip(factories::ToolTipFactory::generateToolTip(this));
}
