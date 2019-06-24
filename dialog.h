#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "ui_dialog.h"
#include "simitem.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog, private Ui::Dialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);

private slots:
    void on_pushButtonBrowse_clicked();
    void on_pushButtonAdd_clicked();
    void on_pushButtonStart_clicked();

private:
    void processItem(SimItem& simItem);

    QString _filename;
    QList<SimItem> _simItems;
};

#endif // DIALOG_H
