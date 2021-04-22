#ifndef ETHERNETINTERFACESETTINGSDIALOG_H
#define ETHERNETINTERFACESETTINGSDIALOG_H

#include <QDialog>

#include "MACAddr.hpp"
#include "IP4Address.hpp"

namespace Ui {
class EthernetInterfaceSettingsDialog;
}

class EthernetInterfaceSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EthernetInterfaceSettingsDialog(QWidget *parent = nullptr);
    ~EthernetInterfaceSettingsDialog();

    virtual void accept() override;

private:
    Ui::EthernetInterfaceSettingsDialog *ui;

signals:
    void info(MACAddr hw, IP4Address ip, uint8_t cidr);
};

#endif // ETHERNETINTERFACESETTINGSDIALOG_H
