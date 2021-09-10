#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>

class MessageBox : public QDialog
{
    Q_OBJECT
public:
    explicit MessageBox(QWidget *parent = nullptr);
    void setText(const QString &text);
    static void information(const QString &title, const QString &text);

private:
    QLabel m_label;
    QPushButton m_pushButton;

signals:

public slots:
};

#endif // MESSAGEBOX_H
