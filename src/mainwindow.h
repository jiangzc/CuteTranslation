#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>


class QStackedWidget;
class QLabel;
class WordPage;

enum CuteAction { PICK, PICK_HANDICT, OCRTranslate, Search, OCRText } ;
enum PICKTYPE : int;

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
    QLabel *textLabel;
    WordPage *wordPage;
    const int TriangleHeight;
    const int TriangleWidth;
    int TriangleOffset = 0;
    int Direction;
    bool showTriangle = true;
    class PreviousAction{
        public:
            CuteAction Action;
        QPoint point1, point2;
        QString text1;
        PICKTYPE type;
    } previousAction;
    QPushButton *refreshButton;
    virtual void showEvent(QShowEvent *e);
    void resultParser(CuteAction action, QString &res);
    QPoint getAdjustedPosition(QPoint mousePressPosition, QPoint mouseReleasedPosition);
    bool screenshot = true;


  public slots:
    void onMouseButtonPressed(int x, int y);
    void onFloatButtonPressed(QPoint mousePressPosition, QPoint mouseReleasedPosition, PICKTYPE type);
    void onOCRTranslateShortCutPressed();
    void onOCRTextShortCutPressed();
    void onSearchBarReturned(QPoint pos, QPoint size, QString res);
    void onRefreshButtonPressed();
    void onAdjustSize(float zoom, int width); // this function is obsolete
    void onCopyButtonPressed();
};

#endif // MAINWINDOW_H
