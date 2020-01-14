#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QPushButton>

#include "configtool.h"
#include "picker.h"


namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    Ui::MainWindow *ui;
    virtual void paintEvent(QPaintEvent *event);
    const int TriangleHeight = configTool->TriangleHeight;
    const int TriangleWidth = configTool->TriangleWidth;
    int TriangleOffset = 0;
    int Direction;
    QWebEngineView *view;
    QString html1;
    QString html2;
    bool showTriangle = true;
    class PreviousAction{
        public:
        enum { PICK, OCR, Search } Action;
        QPoint point1, point2;
        QString text1;
    } previousAction;
    QPushButton *refreshButton;
    virtual void showEvent(QShowEvent *e);
    void htmlParser(QString &res);

  signals:
    void gotHeight();


  public slots:
    void onMouseButtonPressed(int x, int y);
    void onFloatButtonPressed(QPoint mousePressPosition, QPoint mouseReleasedPosition);
    void onOCRShortCutPressed(bool screenshot=true);
    void onSearchBarReturned(QPoint pos, QPoint size, QString res);
    void onRefreshButtonPressed();
};

#endif // MAINWINDOW_H
