#include "Type.h"

#include "Category.h"
#include "ConfigurationBitDescriptor.h"
#include "PortDescriptor.h"

#include <QtDebug>
#include <QtSvg/QtSvg>

using namespace q2d::metamodel;

Type::Type(QString name,
                             Category* parent) :
    HierarchyElement(name, parent) {
    m_instanceIndex = 0;
}

/**
 * @brief ComponentType::setCircuitSymbol sets the circuit symbol from a given svg file
 * and derives an icon from it
 * @param symbolFilePath
 */
void
Type::loadCircuitSymbol(QString symbolFilePath) {

    QGraphicsSvgItem* symbol = new QGraphicsSvgItem(symbolFilePath);
    Q_CHECK_PTR(symbol);

    // Create Icon for UI from Svg
    QIcon* icon = new QIcon(symbolFilePath);
    Q_CHECK_PTR(icon);
    this->setIcon(*icon);
}

/**
 * @brief ComponentType::symbolPath is a getter for convenience.
 * @return the path to the symbol file
 */
QString
Type::symbolPath() {
    return this->data((int)ComponentDescriptorRole::CIRCUIT_SYMBOL_FILE).toString();
}

void
Type::setSymbolPath(QString symbolPath) {
    Q_ASSERT(!(symbolPath.isEmpty()));

    this->setData(QVariant::fromValue(symbolPath), (int)ComponentDescriptorRole::CIRCUIT_SYMBOL_FILE);
    this->loadCircuitSymbol(symbolPath); // update the circuit symbol
}

/**
 * @brief ComponentType::descriptorPath fetches the absolute path at which the type descriptor file resides.
 * @return
 */
QString
Type::descriptorPath() const {
    return this->data((int)ComponentDescriptorRole::DESCRIPTOR_FILE).toString();
}

void
Type::setDescriptorPath(const QString path) {
    Q_ASSERT(!path.isEmpty());
    this->setData(QVariant::fromValue(path), (int)ComponentDescriptorRole::DESCRIPTOR_FILE);
}

void
Type::addPort(QString name, QPoint relativePosition, q2d::model::enums::PortDirection direction) {

    PortDescriptor* portDescriptor = new PortDescriptor(name, direction, relativePosition, this);
    this->appendRow(portDescriptor);
}

/**
 * @brief ComponentType::addConfigBits adds a group of configuration bit descriptors.
 * @param groupName
 *      must not be empty
 * @param memberCount
 *      must be greater then 0
 */
void
Type::addConfigBitGroup(ConfigBitGroupDescriptor* configBitGroup){
    Q_CHECK_PTR(configBitGroup);

    configBitGroup->setParent(this);
    this->appendRow(configBitGroup);
}

QString
Type::generateId() {
    QString id = this->text() + " " + QString::number(m_instanceIndex);
    ++m_instanceIndex;
    return id;
}