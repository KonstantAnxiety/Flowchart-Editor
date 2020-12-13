#include "textdialog.h"
#include "ui_textdialog.h"

TextDialog::TextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();
}

TextDialog::~TextDialog()
{
    delete ui;
}

void TextDialog::on_okButton_clicked()
{
    text = ui->lineEdit->text();
    this->accept();
}

const QString &TextDialog::getText() const
{
    return text;
}
