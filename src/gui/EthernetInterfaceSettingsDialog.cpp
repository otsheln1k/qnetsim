#include "EthernetInterfaceSettingsDialog.h"
#include "ui_ethernetInterfacesettingsdialog.h"
#include <QDebug>

#include <QRegularExpression>

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
    QString ip = ui->lineEdit_2->text();
    qDebug() << ip;
    QRegularExpression ipRegExp("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}");
    if (ipRegExp.match(ip).hasMatch()){
        qDebug() << "Accepted";
        QDialog::accept();
    }
}


