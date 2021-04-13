#include "ECTPPingDialog.h"
#include "ui_ECTPPingDialog.h"

ECTPPingDialog::ECTPPingDialog(QWidget *parent, NetworkNode *node) :
    QDialog(parent),
    ui(new Ui::ECTPPingDialog)
{
    ui->setupUi(this);

    for (GenericNetworkInterface *iface : *node){
        ui->ifaceCombo->addItem(
            QString{"Интерфейс @ 0x%1"}
            .arg((size_t)iface, sizeof(size_t)*2, 16, QChar{'0'}),
            QVariant::fromValue(iface));
    }
}

ECTPPingDialog::~ECTPPingDialog()
{
    delete ui;
}

void ECTPPingDialog::accept()
{
    auto *iface = ui->ifaceCombo->currentData()
        .value<GenericNetworkInterface *>();

    uint16_t seq = ui->sequenceInput->value();

    MACAddr addr;
    addr.parseQString(ui->addrInput->text());

    emit info(iface, seq, addr);

    QDialog::accept();
}
