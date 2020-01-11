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
    std::vector<int> SearchBarShortCut;
    std::vector<int> ScreenShotShortCut;

public slots:
    void onKeyPressed(int keyCode);
    void onKeyReleased(int keyCode);

signals:
    void OCRShortCutPressed(bool);
    void SearchBarShortCutPressed();
};

#endif // SHORTCUT_H
