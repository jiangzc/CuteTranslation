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
    std::vector<int> OCRTranslateShortCut;
    std::vector<int> OCRTextShortCut;

public slots:
    void onKeyPressed(int keyCode);
    void onKeyReleased(int keyCode);

signals:
    void OCRTextShortCutPressed();
    void OCRTranslateShortCutPressed();
    void SearchBarShortCutPressed();
};

#endif // SHORTCUT_H
