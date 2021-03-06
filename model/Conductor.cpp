#include "../DocumentEntry.h"
#include "Conductor.h"

using namespace q2d::model;

/**
 * @brief Conductor::Conductor
 * Conductors are assigned a default name for convenience. It can be changed later.
 *
 * @param start
 * @param end
 */
Conductor::Conductor(Node* start, Node* end, DocumentEntry* relatedEntry)
    : ModelElement(relatedEntry) {
    Q_CHECK_PTR(start);
    Q_CHECK_PTR(end);

    this->m_start = start;
    this->m_end = end;

    start->addDrivenElement(this);
    end->addDriver(this);
}

QStringList
Conductor::nodeVariables() const {
    Q_CHECK_PTR(m_start->relatedEntry());
    Q_CHECK_PTR(m_end->relatedEntry());

    QStringList result;
    result.append(m_start->relatedEntry()->fullId());
    result.append(m_end->relatedEntry()->fullId());

    return result;
}

QStringList
Conductor::functions() const {

    Q_CHECK_PTR(m_start->relatedEntry());
    Q_CHECK_PTR(m_end->relatedEntry());

    QString function(m_start->relatedEntry()->fullId() + " = " + m_end->relatedEntry()->fullId());

    return QStringList(function);
}

