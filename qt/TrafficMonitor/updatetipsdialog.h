#ifndef UPDATETIPSDIALOG_H
#define UPDATETIPSDIALOG_H

#include <QDialog>

namespace Ui {
class UpdateTipsDialog;
}

class UpdateTipsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateTipsDialog(QWidget *parent = nullptr);
    ~UpdateTipsDialog();

private:
    Ui::UpdateTipsDialog *ui;
};

#endif // UPDATETIPSDIALOG_H
