#ifndef SHORTCUT_H
#define SHORTCUT_H
#include <QObject>
#include <deque>

class ShortCut: public QObject
{
    Q_OBJECT
public:
    ShortCut();

private:
    std::deque<int> keyCodes;

public slots:
    void onKeyPressed(int keyCode);
};

#endif // SHORTCUT_H
