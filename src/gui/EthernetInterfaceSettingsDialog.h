#ifndef ETHERNETINTERFACESETTINGSDIALOG_H
#define ETHERNETINTERFACESETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class EthernetInterfaceSettingsDialog;
}

class EthernetInterfaceSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EthernetInterfaceSettingsDialog(QWidget *parent = nullptr);
    ~EthernetInterfaceSettingsDialog();

    virtual void accept() override;

private:
    Ui::EthernetInterfaceSettingsDialog *ui;

signals:
    void info();
};

#endif // ETHERNETINTERFACESETTINGSDIALOG_H
