#ifndef PROJECT_H
#define PROJECT_H


#include "Document.h"

#include <QList>
#include <QObject>
#include <QSettings>
#include <QString>

namespace q2d {

    // forward declaration
    class ApplicationContext;

// TODO documentation
// TODO saving
// TODO loading
class Project : public QObject {
    Q_OBJECT
private:
    ApplicationContext* applicationContext;
    QString name;
    QStandardItemModel documents;
    QSettings projectSettings;

public:
    Project(QString name, ApplicationContext *parent = 0);

    QStandardItemModel* getDocuments();

    void createDocument(QString name);
// TODO    void deleteDocument(Document &toDelete);

    void setupSignalsAndSlots();

signals:
    void signal_showDocument(Document* document);
    void signal_nameChanged(QString newName);

public slots:
    void slot_newDocument(QString name);

};

} // namespace q2d

#endif // PROJECT_H
