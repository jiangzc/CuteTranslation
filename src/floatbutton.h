#ifndef FLOATBUTTON_H
#define FLOATBUTTON_H

#include <QWidget>
#include <QMenu>
#include <QString>

class QLabel;

namespace Ui
{
class FloatButton;
}

enum PICKTYPE : int;
class FloatButton : public QWidget
{
    Q_OBJECT

  public:
    explicit FloatButton(QWidget *parent = nullptr);

    ~FloatButton();

  protected:
    void mousePressEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

  signals:
    void floatButtonPressed(QPoint mousePressPosition, QPoint mouseReleasedPosition, PICKTYPE type);

  private:
    QMenu floatButtonMenu;
    QAction notShow;
    QPoint mousePressPosition;
    QPoint mouseReleasedPosition;
    QLabel *hanDictLabel;

public slots:
    void onWordPicked(QString text);
    void onMouseButtonPressed(int x, int y);  // global listen
    void onMouseButtonReleased(int x, int y); // global listen
    void onKeyPressed(int keyCode);  // global listen
};

#endif // FLOATBUTTON_H
