#include <QRegularExpression>

#include "EthernetInterfaceSettingsDialog.h"
#include "ui_ethernetInterfacesettingsdialog.h"

EthernetInterfaceSettingsDialog::EthernetInterfaceSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EthernetInterfaceSettingsDialog)
{
    ui->setupUi(this);
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
