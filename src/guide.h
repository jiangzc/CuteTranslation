#ifndef GUIDE_H
#define GUIDE_H

#include <QWidget>
#include <QStackedLayout>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QVector>
#include <QMediaPlaylist>
#include <QMediaContent>
#include <QPushButton>
#include <QMap>

class Guide : public QWidget
{
    Q_OBJECT
public:
    Guide(Guide &g) = delete;
    Guide& operator=(Guide &g) = delete;
    void initUI();
    static Guide *instance;
    static Guide *getInstance();
    QWidget* makePage1();
    QWidget* makePage2();
    QWidget* makePage3();

private:
    explicit Guide(QWidget *parent = nullptr);
    QStackedLayout *stackedLayout;
    QVector<QWidget*> pages;
    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    QMediaPlaylist videoPlaylist;
    QMap<QWidget*, QMediaContent> videoForPage;
    QPushButton *preBtn;
    QPushButton *nextBtn;
    void closeEvent(QCloseEvent *event);


private slots:
    void onPreBtnClicked();
    void onNextBtnClicked();
signals:

public slots:
};

#endif // GUIDE_H
