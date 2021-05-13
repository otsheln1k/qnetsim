#ifndef ETHERNETINTERFACESETTINGSDIALOG_H
#define ETHERNETINTERFACESETTINGSDIALOG_H

#include <QDialog>

#include "MACAddr.hpp"
#include "IP4Address.hpp"
#include "IP4OnEthernetDriver.hpp"

namespace Ui {
class EthernetInterfaceSettingsDialog;
}

class EthernetInterfaceSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EthernetInterfaceSettingsDialog(EthernetDriver*, IP4OnEthernetDriver*, QWidget *parent = nullptr);
    ~EthernetInterfaceSettingsDialog();

    virtual void accept() override;

private:
    Ui::EthernetInterfaceSettingsDialog *ui;
    EthernetDriver* drv;
    IP4OnEthernetDriver* ip4drv;

signals:
    void info(MACAddr hw, IP4Address ip, uint8_t cidr, bool arpEnabled, ARPForIP4OnEthernetDriver* table);

private slots:
    void on_checkBox_stateChanged(int arg1);
    void on_checkBox_2_stateChanged(int arg1);
    void on_arpSwitch_stateChanged(int arg1);
};

#endif // ETHERNETINTERFACESETTINGSDIALOG_H
