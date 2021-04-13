#include "interfacedialog.h"
#include "ui_interfacedialog.h"

InterfaceDialog::InterfaceDialog(QWidget *parent, NetworkNode *node) :
    QDialog(parent),
    ui(new Ui::InterfaceDialog)
{
    ui->setupUi(this);

    for (GenericNetworkInterface *iface : *node){
        ui->ifaceCombo->addItem(
            QString{"Интерфейс @ 0x%1"}
            .arg((size_t)iface, sizeof(size_t)*2, 16, QChar{'0'}),
            QVariant::fromValue(iface));
    }
}

InterfaceDialog::~InterfaceDialog()
{
    delete ui;
}

InterfaceDialogAnswer InterfaceDialog::getResult()
{
    MACAddr addr;
    addr.parseQString(ui->addrInput->text());

    return {
        ui->ifaceCombo->currentData().value<GenericNetworkInterface *>(),
        static_cast<unsigned short>(ui->sequenceInput->value()),
        addr,
    };
}
