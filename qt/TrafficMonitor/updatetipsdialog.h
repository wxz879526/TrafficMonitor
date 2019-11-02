#ifndef UPDATETIPSDIALOG_H
#define UPDATETIPSDIALOG_H

#include "widgets/framelessdialog.h"

namespace Ui {
class UpdateTipsDialog;
}

class UpdateTipsDialog : public CFramelessDialog
{
    Q_OBJECT

public:
    explicit UpdateTipsDialog(QWidget *parent = nullptr);
    ~UpdateTipsDialog();

private:
    Ui::UpdateTipsDialog *ui;
};

#endif // UPDATETIPSDIALOG_H
