#include <QRegularExpression>

#include "EthernetInterfaceSettingsDialog.h"
#include "ui_ethernetInterfacesettingsdialog.h"

EthernetInterfaceSettingsDialog::EthernetInterfaceSettingsDialog(MACAddr mac, IP4Address ip4, uint8_t mask, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EthernetInterfaceSettingsDialog)
{
    ui->setupUi(this);
    ui->macInput->setText(mac);
    ui->ipInput->setText(QString{"%1.%2.%3.%4"}.arg(ip4[0]).arg(ip4[1]).arg(ip4[2]).arg(ip4[3]));
    ui->cidrInput->setValue(mask);

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

    emit info(hw, ip, (uint8_t)cidr);

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
    arg1 ? ui->spinBox->setEnabled(ui->checkBox_2->isChecked()) : ui->spinBox->setEnabled(arg1);
    arg1 ? ui->spinBox_2->setEnabled(ui->checkBox_2->isChecked()) : ui->spinBox_2->setEnabled(arg1);
    ui->toolButton->setEnabled(arg1);
    ui->lineEdit->setEnabled(arg1);
    ui->lineEdit_2->setEnabled(arg1);
}
