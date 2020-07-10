#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "configtool.h"
#include "picker.h"

class QStackedWidget;

enum CuteAction { PICK, OCRTranslate, Search, OCRText } ;

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
    QStackedWidget *stackWidget;
    const int TriangleHeight = configTool->TriangleHeight;
    const int TriangleWidth = configTool->TriangleWidth;
    int TriangleOffset = 0;
    int Direction;
    float zoom = 1.0;
    bool showTriangle = true;
    class PreviousAction{
        public:
            CuteAction Action;
        QPoint point1, point2;
        QString text1;
    } previousAction;
    QPushButton *refreshButton;
    virtual void showEvent(QShowEvent *e);
    void htmlParser(QString &res);
    bool screenshot = true;


  public slots:
    void onMouseButtonPressed(int x, int y);
    void onFloatButtonPressed(QPoint mousePressPosition, QPoint mouseReleasedPosition);
    void onOCRTranslateShortCutPressed();
    void onOCRTextShortCutPressed();
    void onSearchBarReturned(QPoint pos, QPoint size, QString res);
    void onRefreshButtonPressed();
    void onAdjustSize(float zoom, int width);
};

#endif // MAINWINDOW_H
