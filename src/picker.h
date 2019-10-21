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
    QString CurrentWindowsPath;
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
extern Picker *picker;
#endif // PICKER_H
