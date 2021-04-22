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
    QDialog::accept();
}


