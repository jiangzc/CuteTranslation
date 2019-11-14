#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QWidget>
#include <QPoint>

namespace Ui {
class SearchBar;
}

class SearchBar : public QWidget
{
    Q_OBJECT

public:
    explicit SearchBar(QWidget *parent = nullptr);
    ~SearchBar();

private:
    Ui::SearchBar *ui;
    QPoint mDragPosition;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // SEARCHBAR_H
