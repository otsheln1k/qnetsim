#include <QDebug>
#include <QRegularExpression>

#include "EthernetInterfaceSettingsDialog.h"
#include "ui_ethernetInterfacesettingsdialog.h"

EthernetInterfaceSettingsDialog::EthernetInterfaceSettingsDialog(
    EthernetDriver* drv, IP4OnEthernetDriver* ip4drv, QWidget *parent)
    :QDialog(parent),
     ui(new Ui::EthernetInterfaceSettingsDialog),
     drv{drv},
     ip4drv{ip4drv}
{

    ui->setupUi(this);
    ui->macInput->setText(drv->address());
    ui->ipInput->setText(ip4drv->address());
    ui->cidrInput->setValue(ip4drv->cidr());

    ui->arpSwitch->setChecked(ip4drv->arpCacheEnabled());
    ui->arpTableWidget->setColumnWidth(0, 200);
    ui->arpTableWidget->setColumnWidth(1, 200);
    ui->arpTableWidget->setColumnWidth(2, 100);
    ui->arpTableWidget->horizontalHeader()->setStretchLastSection(true);

    auto *arptab = ip4drv->arpTable();

    int arp_maxn = arptab->maxSize();
    ui->sizeLimitCheckbox->setChecked(arp_maxn >= 0);
    ui->sizeLimitSpinbox->setValue((arp_maxn >= 0) ? arp_maxn : 0);

    ui->arpTableWidget->setRowCount(arptab->currentSize());
    int i = 0;
    for (auto [ip, mac, lt] : *arptab) {
        setArpTableRow(i++, ip, mac, lt);
    }
}

void
EthernetInterfaceSettingsDialog::setArpTableRow(
    int i, IP4Address ip, MACAddr mac, int lt)
{
    auto *c1 = new QTableWidgetItem {ip};
    c1->setData(Qt::UserRole, QVariant::fromValue(ip));
    auto *c2 = new QTableWidgetItem {mac};
    c2->setData(Qt::UserRole, QVariant::fromValue(mac));
    auto *c3 = new QTableWidgetItem {lt < 0 ? "-" : QString::number(lt)};
    c3->setData(Qt::UserRole, QVariant {lt});

    ui->arpTableWidget->setItem(i, 0, c1);
    ui->arpTableWidget->setItem(i, 1, c2);
    ui->arpTableWidget->setItem(i, 2, c3);
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

    auto *arptab = this->ip4drv->arpTable();
    arptab->clearTable();

    int n = ui->arpTableWidget->rowCount();
    for (int i = 0; i < n; ++i) {
        auto ip = ui->arpTableWidget->item(i, 0)
            ->data(Qt::UserRole).value<IP4Address>();
        auto mac = ui->arpTableWidget->item(i, 1)
            ->data(Qt::UserRole).value<MACAddr>();
        auto lt = ui->arpTableWidget->item(i, 2)
            ->data(Qt::UserRole).toInt();
        arptab->addEntry(ip, mac, lt);
    }

    QDialog::accept();
}

void EthernetInterfaceSettingsDialog::on_lifetimeCheckbox_stateChanged(int arg1)
{
    ui->lifetimeSpinbox->setEnabled(arg1);
}

void EthernetInterfaceSettingsDialog::on_sizeLimitCheckbox_stateChanged(int arg1)
{
    ui->sizeLimitSpinbox->setEnabled(arg1);
}

void EthernetInterfaceSettingsDialog::on_arpSwitch_stateChanged(int arg1)
{
    ui->arpTableWidget->setEnabled(arg1);
    ui->lifetimeCheckbox->setEnabled(arg1);
    ui->sizeLimitCheckbox->setEnabled(arg1);
    ui->label_4->setEnabled(arg1);
    ui->label_5->setEnabled(arg1);
    ui->lifetimeSpinbox->setEnabled(arg1 && ui->lifetimeCheckbox->isChecked());
    ui->sizeLimitSpinbox->setEnabled(arg1 && ui->sizeLimitCheckbox->isChecked());
    ui->arpAddButton->setEnabled(arg1);
    ui->arpIpInput->setEnabled(arg1);
    ui->arpMacInput->setEnabled(arg1);
}

void EthernetInterfaceSettingsDialog::on_arpAddButton_clicked()
{
    IP4Address ip;
    if (!ip.parseQString(ui->arpIpInput->text())) {
        return;
    }

    MACAddr mac;
    if (!mac.parseQString(ui->arpMacInput->text())) {
        return;
    }

    bool ok = true;
    int lt = ui->lifetimeCheckbox->isChecked()
        ? (int)ui->lifetimeSpinbox->text().toUInt(&ok)
        : -1;
    if (!ok) {
        return;
    }

    int row = ui->arpTableWidget->rowCount();
    ui->arpTableWidget->setRowCount(ui->arpTableWidget->rowCount() + 1);
    setArpTableRow(row, ip, mac, lt);
}
