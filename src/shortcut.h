#ifndef SHORTCUT_H
#define SHORTCUT_H
#include <QObject>
#include <deque>
#include <vector>

class ShortCut: public QObject
{
    Q_OBJECT
public:
    ShortCut();

private:
    std::deque<int> keyCodes;
    std::vector<int> FloatBarShortCut;
    std::vector<int> ScreenShotShortCut;

public slots:
    void onKeyPressed(int keyCode);

signals:
    void OCRCompleted(QString res);
};

#endif // SHORTCUT_H
