#ifndef PLACEHOLDER_H
#define PLACEHOLDER_H

#include <QObject>
#include <QString>

struct Placeholder : QObject
{
    Q_OBJECT

public:
    QString filename;

public slots:
    void marshalFilename(const QString& fname);
};

#endif // PLACEHOLDER_H
