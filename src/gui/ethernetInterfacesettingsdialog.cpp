#include "ethernetInterfacesettingsdialog.h"
#include "ui_interfacesettingsdialog.h"

interfaceSettingsDialog::interfaceSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::interfaceSettingsDialog)
{
    ui->setupUi(this);
}

interfaceSettingsDialog::~interfaceSettingsDialog()
{
    delete ui;
}
