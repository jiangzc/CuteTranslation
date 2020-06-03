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
        enum { PICK, OCRTranslate, Search, OCRText } Action;
        QPoint point1, point2;
        QString text1;
    } previousAction;
    QPushButton *refreshButton;
    virtual void showEvent(QShowEvent *e);
    void htmlParser(QString &res);
    bool screenshot = true;

  signals:
    void gotHeight();


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
