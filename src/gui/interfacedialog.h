#ifndef INTERFACEDIALOG_H
#define INTERFACEDIALOG_H

#include <QDialog>
#include "GenericNetworkInterface.hpp"
#include "../NetworkNode.h"

namespace Ui {
class InterfaceDialog;
}

struct InterfaceDialogAnswer{
    GenericNetworkInterface *interface;
    unsigned short res;
};

class InterfaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InterfaceDialog(QWidget *parent = nullptr, NetworkNode *node = nullptr);
    ~InterfaceDialog();

    InterfaceDialogAnswer getResult();


private:
    Ui::InterfaceDialog *ui;

signals:
    void accepted();
};

#endif // INTERFACEDIALOG_H
