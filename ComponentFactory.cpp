#include "ComponentFactory.h"

#include "Application.h"
#include "ApplicationContext.h"
#include "Constants.h"
#include "Document.h"
#include "Enumerations.h"
#include "JsonHelpers.h"
#include "Util.h"

#include "gui/ComponentGraphicsItem.h"
#include "gui/Schematic.h"
#include "gui/PortGraphicsItem.h"
#include "gui/WireGraphicsItem.h"

#include "metamodel/Category.h"
#include "metamodel/ConfigurationBitDescriptor.h"
#include "metamodel/ComponentDescriptor.h"
#include "metamodel/PortDescriptor.h"

#include "model/Component.h"
#include "model/Conductor.h"
#include "model/Model.h"
#include "model/Port.h"

#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QtDebug>

using namespace q2d;
using namespace q2d::constants;
using namespace q2d::metamodel;

ComponentFactory::ComponentFactory(ApplicationContext* parent)
    : QObject(parent) {}

/**
 * @brief ComponentFactory::addCategory creates a new category and adds it to
 * the component hierarchy.
 * If the parent category is null, the item will be sorted directly below the
 * root.
 *
 * @param name
 * @param parent the parent category; May be null
 */
Category*
ComponentFactory::slot_addCategory(QString name, Category* parent) {
    // TODO ensure update signals are fired upon adding a category
    Q_ASSERT(!name.isEmpty ());
    qDebug() << "Adding category" << name << "with parent" << parent;


    Category* newCategory = new Category(name, parent);

    if (parent == nullptr) {
        this->componentHierarchy.invisibleRootItem()->appendRow(newCategory);
    } else {
        parent->addSubItem(newCategory);
    }
    return newCategory;
}

/**
 * @brief ComponentFactory::slot_loadType creates a new component type
 * and adds it to the component hierarchy.
 * If the parent category is null, the item will be sorted directly below the
 * root.
 *
 * @param filePath is the absolute path of the file that contains the component description as JSON
 * @param parent the parent category; May be null
 */
void
ComponentFactory::slot_loadType(QString filePath, Category* parent) {

    // logging prefix
    const QString logPrefix = this->objectName() + "::slot_loadType(" + filePath + ", "
                              + util::ptrToString(parent) + ")";

    QJsonDocument jsonDocument = json::readJsonFile(filePath);

    // validity check
    if (jsonDocument.isNull()) {
        qWarning() << logPrefix << "json document seems to be invalid";
        return;
    }

    ComponentDescriptor* newType = json::toComponentDescriptor(jsonDocument, filePath, parent);
    Q_CHECK_PTR(newType);

    // TODO later disallow items w/o category
    if (parent != nullptr) {
        parent->addSubItem(newType);
    } else {
        this->componentHierarchy.invisibleRootItem()->appendRow(newType);
    }
}

/**
 * @brief ComponentFactory::getCategoryForIndex attempts to get the ComponentCategory
 * for the given index.
 *
 * If the index is invalid or the indexed element is not a ComponentCategory, a nullptr will be returned.
 *
 * @param index
 * @return ; May return null
 */
Category*
ComponentFactory::getCategoryForIndex(const QModelIndex &index) {
    if (!index.isValid()) {
        return nullptr;
    }

    QStandardItem* item = this->componentHierarchy.itemFromIndex(index);
    return dynamic_cast<Category*>(item);
}

/**
 * @brief ComponentFactory::getTypeForIndex attempts to get the ComponentType
 * for the given index.
 *
 * If the index is invalid or the indexed element is not a ComponentType, a nullptr will be returned.
 *
 * @param index
 * @return ; May return null
 */
ComponentDescriptor*
ComponentFactory::getTypeForIndex(const QModelIndex &index) {

    if (!index.isValid()) {
        return nullptr;
    }

    QStandardItem* item = this->componentHierarchy.itemFromIndex(index);
    return static_cast<ComponentDescriptor*>(item);
}

ComponentDescriptor*
ComponentFactory::getTypeForHierarchyName(QString hierarchyName) {
    QStringList hierarchy = hierarchyName.split(HIERARCHY_SEPERATOR,
                            QString::SkipEmptyParts);
    // traverse the hierarchy and find the component
    // TODO use a more effective approach here, this is really a hack

    QStandardItem* currentItem = this->componentHierarchy.invisibleRootItem();

    while (!hierarchy.isEmpty()) {
        // iterate over the current hierarcy level
        for (int index = 0; index < currentItem->rowCount(); ++index) {
            QStandardItem* child = currentItem->child(index);
            if (child->text() == hierarchy.first()) {
                currentItem = child;
                hierarchy.removeFirst(); // down to the next level
                break;
            }
            if (index == currentItem->rowCount() - 1) {
                // found nothing in the last element searched
                qDebug() << hierarchyName << "not found in component hierarchy!";
                return nullptr;
            }
        }
    }

    return dynamic_cast<ComponentDescriptor*>(currentItem);
}

QStandardItemModel*
ComponentFactory::getComponentHierarchy() {
    return &(this->componentHierarchy);
}

// FIXME move this and all helpers to JsonHelpers
QJsonDocument
ComponentFactory::exportHierarchy() {
    QJsonDocument result = QJsonDocument();
    QJsonArray hierarchy = QJsonArray();

    QStandardItem* rootItem = componentHierarchy.invisibleRootItem();

    for (int rIndex = 0; rIndex < rootItem->rowCount(); ++rIndex) {
        for (int cIndex = 0; cIndex < rootItem->columnCount(); ++cIndex) {
            hierarchy.append(QJsonValue(
                                 this->entryToJson(rootItem->child(rIndex, cIndex))
                             ));
        }
    }

    result.setArray(hierarchy);
    return result;
}


void
ComponentFactory::importHierarchy(QJsonDocument source) {
    Q_ASSERT(!source.isNull());
    Q_ASSERT(source.isArray());

    QJsonArray hierarchy = source.array();

    for (int index = 0; index < hierarchy.count(); ++index) {
        jsonToEntry(hierarchy[index].toObject(), nullptr);
    }
}

void
ComponentFactory::jsonToCategoryEntry(QJsonObject json, Category* parent) {

    QString name = json.value(JSON_HIERARCHY_CATEGORY_NAME).toString();
    Category* result = this->slot_addCategory(name, parent);

    QJsonArray children = json.value(JSON_HIERARCHY_CHILD).toArray();

    for (int index = 0; index < children.count(); ++index) {
        jsonToEntry(children[index].toObject(), result);
    }
}

void
ComponentFactory::jsonToTypeEntry(QJsonObject json, Category* parent) {
    QString sourcePath = json.value(JSON_HIERARCHY_SOURCE).toString();
    Q_ASSERT(!sourcePath.isEmpty());

    this->slot_loadType(sourcePath, parent);
}

void
ComponentFactory::jsonToEntry(QJsonObject json, Category* parent) {
    QString type = json.value(JSON_HIERARCHY_TYPE).toString();
    if (type == JSON_HIERARCHY_TYPE_COMPONENT) {
        jsonToTypeEntry(json, parent);
    } else if (type == JSON_HIERARCHY_TYPE_CATEGORY) {
        jsonToCategoryEntry(json, parent);
    } else {
        Q_ASSERT(false);
    }
}


QJsonObject
ComponentFactory::categoryEntryToJson(QStandardItem* item) {
    Category* category = dynamic_cast<Category*>(item);
    Q_CHECK_PTR(category);

    QJsonObject result = QJsonObject();
    QJsonArray children = QJsonArray();
    result.insert(JSON_HIERARCHY_CATEGORY_NAME, QJsonValue(category->text()));
    // child recursion
    for (int rIndex = 0; rIndex < item->rowCount(); ++rIndex) {
        for (int cIndex = 0; cIndex < item->columnCount(); ++cIndex) {
            children.append(QJsonValue(
                                this->entryToJson(item->child(rIndex, cIndex))
                            ));
        }
    }
    result.insert(JSON_HIERARCHY_CHILD, children);
    return result;
}

/**
 * @brief ComponentFactory::typeEntryToJson converts a QStandardItem describing a ComponentType
 * into a QJsonObject
 * @param item
 * @return
 */
QJsonObject
ComponentFactory::typeEntryToJson(QStandardItem* item) {
    QJsonObject result = QJsonObject();
    ComponentDescriptor* componentType = dynamic_cast<ComponentDescriptor*>(item);
    Q_CHECK_PTR(componentType);

    result.insert(JSON_HIERARCHY_SOURCE,
                  QJsonValue(componentType->descriptorPath()));
    return result;
}

QJsonObject
ComponentFactory::entryToJson(QStandardItem* item) {

    QJsonObject result;

    switch (item->type()) {
    case (int)metamodel::enums::ElementType::COMPONENT:
        result = typeEntryToJson(item);
        result.insert(JSON_HIERARCHY_TYPE, QJsonValue(JSON_HIERARCHY_TYPE_COMPONENT));
        break;
    case (int)metamodel::enums::ElementType::CATEGORY:
        result = categoryEntryToJson(item);
        result.insert(JSON_HIERARCHY_TYPE, QJsonValue(JSON_HIERARCHY_TYPE_CATEGORY));
        break;
    case (int)metamodel::enums::ElementType::PORT:
        // nothing to save, since they are bound to the component
        // this should not happen anyways
        Q_ASSERT(false);
    default:; // nothing useful to do, we should not end up here
        Q_ASSERT(false);
        result = QJsonObject();
    }

    return result;
}

void
ComponentFactory::slot_clearHierarchy() {
    componentHierarchy.clear();
}
