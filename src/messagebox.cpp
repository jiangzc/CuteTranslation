#include <QFont>
#include "messagebox.h"
#include "xdotool.h"



MessageBox::MessageBox(QWidget *parent) : QDialog(parent), m_label(this), m_pushButton(this)
{
    m_label.setGeometry(50,20, 400, 120);
    m_label.setWordWrap(true);
    m_label.setAlignment(Qt::AlignCenter);
    QFont font = m_label.font();
    font.setPixelSize(20);
    m_label.setFont(font);

    m_pushButton.setGeometry(200, 150, 100, 40);
    m_pushButton.setText("OK");
    m_pushButton.setFont(font);
    connect(&m_pushButton, &QPushButton::clicked, this, &QDialog::done);

    resize(500, 200);
    this->move((xdotool->screenWidth - this->width()) / 2, (xdotool->screenHeight - this->height()) / 2);


}

void MessageBox::setText(const QString &text)
{
    m_label.setText(text);
}

void MessageBox::information(const QString &title, const QString &text)
{
    static MessageBox msg;
    msg.setWindowTitle(title);
    msg.setText(text);
    msg.show();
}


