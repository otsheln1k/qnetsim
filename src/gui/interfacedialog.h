#ifndef INTERFACEDIALOG_H
#define INTERFACEDIALOG_H

#include <stdint.h>

#include <QDialog>

#include "MACAddr.hpp"
#include "GenericNetworkInterface.hpp"
#include "NetworkNode.h"

namespace Ui {
class InterfaceDialog;
}

class InterfaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InterfaceDialog(QWidget *parent = nullptr, NetworkNode *node = nullptr);
    ~InterfaceDialog();

    virtual void accept() override;

private:
    Ui::InterfaceDialog *ui;

signals:
    void info(GenericNetworkInterface *iface, uint16_t seq, MACAddr addr);
};

#endif // INTERFACEDIALOG_H
