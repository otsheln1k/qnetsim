#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <QDebug>

#include "nsgraphicsview.h"
#include "nsgraphicshubnode.h"

class Serialization
{
    NSGraphicsView* view;
    QString path;

public:
    Serialization(NSGraphicsView* view, QString path);
    void createSave();
    void loadSave();
};

#endif // SERIALIZATION_H
