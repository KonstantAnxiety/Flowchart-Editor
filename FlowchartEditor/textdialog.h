#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include <QString>
#include <QDialog>

namespace Ui {
class TextDialog;
}

class TextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextDialog(QWidget *parent = nullptr);
    ~TextDialog();
    const QString& getText() const;

private slots:
    void on_okButton_clicked();

private:
    Ui::TextDialog *ui;
    QString text;
};

#endif // TEXTDIALOG_H
