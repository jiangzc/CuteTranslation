#ifndef WORDPAGE_H
#define WORDPAGE_H

#include <QWidget>
#include <QVector>

class FlowLayout;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;

class WordPage : public QWidget
{
    Q_OBJECT
public:
    WordPage();
    void initUI();
    QLabel *titleLabel;
    QLabel *leftAudioLabel;
    QLabel *rightAudioLabel;
    QVBoxLayout *mainlayout;
    void updateDescription(const QJsonObject &obj);
private:
    QVector<QHBoxLayout*> descriptions;
    QVector<QLabel *> tags;
    FlowLayout *tagslayout;

};

#endif // WORDPAGE_H
