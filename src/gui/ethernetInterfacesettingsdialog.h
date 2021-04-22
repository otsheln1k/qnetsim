#ifndef ETHERNETINTERFACESETTINGSDIALOG_H
#define ETHERNETINTERFACESETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class interfaceSettingsDialog;
}

class interfaceSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit interfaceSettingsDialog(QWidget *parent = nullptr);
    ~interfaceSettingsDialog();

    virtual void accept() override;

private:
    Ui::interfaceSettingsDialog *ui;

signals:
    void info();
};

#endif // ETHERNETINTERFACESETTINGSDIALOG_H
