#ifndef PICKER_H
#define PICKER_H

#include <QClipboard>
#include <QObject>
#include <QString>

class Picker : public QObject
{
    Q_OBJECT
  public:
    explicit Picker(QObject *parent = nullptr);
    void buttonPressed();
    void buttonReleased();
    QString getSelectedText();
    QString text;

  private:
    QClipboard *clipboard;
    bool isPressed;

  signals:
    void wordsPicked(QString);

  public slots:
};

#endif // PICKER_H
