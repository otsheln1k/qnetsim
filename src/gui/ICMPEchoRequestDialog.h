#ifndef ICMPECHOREQUESTDIALOG_H
#define ICMPECHOREQUESTDIALOG_H

#include <QDialog>

#include "IP4Address.hpp"

namespace Ui {
class ICMPEchoRequestDialog;
}

class ICMPEchoRequestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ICMPEchoRequestDialog(QWidget *parent = nullptr);
    ~ICMPEchoRequestDialog() {};

    virtual void accept() override;

signals:
    void info(IP4Address dest, uint16_t ident, uint16_t seq,
              const QVector<uint8_t> &payload);

private:
    Ui::ICMPEchoRequestDialog *ui;

private slots:
};

#endif // ICMPECHOREQUESTDIALOG_H
