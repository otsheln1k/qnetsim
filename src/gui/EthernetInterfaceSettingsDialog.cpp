#include <QRegularExpression>

#include "EthernetInterfaceSettingsDialog.h"
#include "ui_ethernetInterfacesettingsdialog.h"

EthernetInterfaceSettingsDialog::EthernetInterfaceSettingsDialog(EthernetDriver* drv, IP4OnEthernetDriver* ip4drv, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EthernetInterfaceSettingsDialog)
{
    this->drv = drv;
    this->ip4drv = ip4drv;

    ui->setupUi(this);
    ui->macInput->setText(drv->address());
    ui->ipInput->setText(ip4drv->address());
    ui->cidrInput->setValue(ip4drv->cidr());

    ui->arpSwitch->setChecked(ip4drv->arpCacheEnabled());
    ui->arpTableWidget->setColumnWidth(0, 200);
    ui->arpTableWidget->setColumnWidth(1, 200);
    ui->arpTableWidget->setColumnWidth(2, 100);
    ui->arpTableWidget->horizontalHeader()->setStretchLastSection(true);

}

EthernetInterfaceSettingsDialog::~EthernetInterfaceSettingsDialog()
{
    delete ui;
}

void EthernetInterfaceSettingsDialog::accept()
{
    MACAddr hw;
    hw.parseQString(ui->macInput->text());

    QString ipText = ui->ipInput->text();
    QRegularExpression ipRegExp {
        "(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})"
    };
    auto match = ipRegExp.match(ipText);
    if (!match.hasMatch()) {
        return;
    }

    uint32_t val = 0;
    for (int i = 0; i < 4; ++i) {
        unsigned u = match.captured(i+1).toUInt();
        if (u > 255) {
            return;
        }
        val = (val << 8) | (uint8_t)u;
    }
    IP4Address ip {val};

    int cidr = ui->cidrInput->value();
    if (0 > cidr
        || cidr > 32) {
        return;
    }

    this->drv->setAddress(hw);
    this->ip4drv->setCidr(cidr);
    IP4Address tmp;
    tmp.parseQString(ip);
    this->ip4drv->setAddress(tmp);
    this->ip4drv->setArpCacheEnabled(ui->arpSwitch->isChecked());
    //this->ip4drv->arpDriver()->


    QDialog::accept();
}

void EthernetInterfaceSettingsDialog::on_checkBox_stateChanged(int arg1)
{
    ui->spinBox->setEnabled(arg1);
}

void EthernetInterfaceSettingsDialog::on_checkBox_2_stateChanged(int arg1)
{
    ui->spinBox_2->setEnabled(arg1);
}

void EthernetInterfaceSettingsDialog::on_arpSwitch_stateChanged(int arg1)
{
    ui->arpTableWidget->setEnabled(arg1);
    ui->checkBox->setEnabled(arg1);
    ui->checkBox_2->setEnabled(arg1);
    ui->label_4->setEnabled(arg1);
    ui->label_5->setEnabled(arg1);
    arg1 ? ui->spinBox->setEnabled(ui->checkBox->isChecked()) : ui->spinBox->setEnabled(arg1);
    arg1 ? ui->spinBox_2->setEnabled(ui->checkBox_2->isChecked()) : ui->spinBox_2->setEnabled(arg1);
    ui->toolButton->setEnabled(arg1);
    ui->lineEdit->setEnabled(arg1);
    ui->lineEdit_2->setEnabled(arg1);
}
