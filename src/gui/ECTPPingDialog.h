#ifndef _NS_GUI_ECTP_PING_DIALOG_H_
#define _NS_GUI_ECTP_PING_DIALOG_H_

#include <stdint.h>

#include <QDialog>

#include "MACAddr.hpp"
#include "GenericNetworkInterface.hpp"
#include "NetworkNode.h"

namespace Ui {
class ECTPPingDialog;
}

class ECTPPingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ECTPPingDialog(QWidget *parent = nullptr, NetworkNode *node = nullptr);
    ~ECTPPingDialog();

    virtual void accept() override;

private:
    Ui::ECTPPingDialog *ui;

signals:
    void info(GenericNetworkInterface *iface, uint16_t seq, MACAddr addr);
};

#endif // _NS_GUI_ECTP_PING_DIALOG_H_
