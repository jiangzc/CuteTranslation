#include <QFont>
#include "messagebox.h"

MessageBox::MessageBox(QWidget *parent) : QDialog(parent), m_label(this), m_pushButton(this)
{
    m_label.setGeometry(100,30, 300, 100);
    m_label.setWordWrap(true);
    m_label.setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    QFont font = m_label.font();
    font.setPixelSize(20);
    m_label.setFont(font);

    m_pushButton.setGeometry(200, 150, 100, 40);
    m_pushButton.setText("OK");
    m_pushButton.setFont(font);
    connect(&m_pushButton, &QPushButton::clicked, this, &QDialog::done);

    resize(500, 200);

}

void MessageBox::setText(QString text)
{
    m_label.setText(text);
}
