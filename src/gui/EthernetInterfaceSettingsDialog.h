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

    void setArpTableRow(int i, IP4Address ip, MACAddr mac, int lt);

private slots:
    void on_lifetimeCheckbox_stateChanged(int arg1);
    void on_sizeLimitCheckbox_stateChanged(int arg1);
    void on_arpSwitch_stateChanged(int arg1);
    void on_arpAddButton_clicked();
};

#endif // ETHERNETINTERFACESETTINGSDIALOG_H
