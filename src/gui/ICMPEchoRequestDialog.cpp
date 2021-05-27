#include <stdlib.h>

#include "ICMPEchoRequestDialog.h"
#include "ui_ICMPEchoRequestDialog.h"

ICMPEchoRequestDialog::ICMPEchoRequestDialog(QWidget *parent)
    :QDialog(parent),
     ui{new Ui::ICMPEchoRequestDialog {}}
{
    qRegisterMetaType<IP4Address>("IP4Address");
    qRegisterMetaType<uint8_t>("uint8_t");

    ui->setupUi(this);
    ui->identInput->setValue(rand() % 65536);
}

void
ICMPEchoRequestDialog::accept()
{
    IP4Address ip;
    if (!ip.parseQString(ui->ipInput->text())) {
        return;
    }

    uint16_t ident = (uint16_t)ui->identInput->value();
    uint16_t seq = (uint16_t)ui->seqInput->value();

    int size = ui->sizeInput->value();
    QVector<uint8_t> pl (size);
    for (int i = 0; i < size; ++i) {
        pl[i] = i % 256;
    }

    emit info(ip, ident, seq, pl);

    QDialog::accept();
}
