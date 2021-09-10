#ifndef SEARCHBAR_H
#define SEARCHBAR_H

class QLabel;
class QPushButton;

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
    void ClearLineEdit();
    ~SearchBar();
protected:
    bool eventFilter(QObject *obj, QEvent *ev);
private:
    Ui::SearchBar *ui;
    QPoint mDragPosition;
    QPushButton *hideButton;
    QLabel *searchIcon;
    QWidget *mainWindow;
    virtual void showEvent(QShowEvent *e);

signals:
    void returnPressed(QPoint pos, QPoint size, QString res);

public slots:
    void OnSearchBarShortCutPressed(QWidget *mainWindow);
};

#endif // SEARCHBAR_H
