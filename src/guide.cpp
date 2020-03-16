#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QTextEdit>
#include "guide.h"
#include "configtool.h"

Guide* Guide::instance = nullptr;

Guide* Guide::getInstance()
{
    if (instance == nullptr)
        instance = new Guide();
    return instance;
}

Guide::Guide(QWidget *parent) : QWidget(parent)
{
    player = new QMediaPlayer(this);
    videoWidget = new QVideoWidget(this);
    videoWidget->setMinimumSize(600,300);
    player->setVideoOutput(videoWidget);
    playlist.setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    player->setPlaylist(&playlist);

    initUI();
    onCtrlBtnClicked(); // 初始化按钮状态

    playlist.addMedia(QMediaContent()); // No Content
    for (auto item : videoForPage.values())
        playlist.addMedia(item);

    setCurrentVideo(0);

}

void Guide::initUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *headLine = new QHBoxLayout();
    stackedLayout = new QStackedLayout();
    QHBoxLayout *buttonLine = new QHBoxLayout();

    QLabel *iconLabel = new QLabel();
    iconLabel->setPixmap(QPixmap(appDir.absoluteFilePath("CuteTranslation.svg")));
    // iconLabel->setScaledContents(true);

    QLabel *title = new QLabel();
    title->setText("翻译星  使用教程");
    title->setFont(QFont("Helvetica", 15));
    headLine->addStretch();
    headLine->addWidget(iconLabel);
    headLine->addWidget(title);
    headLine->addStretch();

    skipBtn = new QPushButton();
    preBtn = new QPushButton();
    nextBtn = new QPushButton();
    skipBtn->setText("跳过");
    preBtn->setText("上一步");
    nextBtn->setText("下一步");
    connect(skipBtn, &QPushButton::clicked, this, &Guide::onCtrlBtnClicked);
    connect(preBtn, &QPushButton::clicked, this, &Guide::onCtrlBtnClicked);
    connect(nextBtn, &QPushButton::clicked, this, &Guide::onCtrlBtnClicked);
    buttonLine->addWidget(skipBtn);
    buttonLine->addStretch();
    buttonLine->addWidget(preBtn);
    buttonLine->addWidget(nextBtn);
    buttonLine->setMargin(10);
    buttonLine->setSpacing(30);

    mainLayout->addLayout(headLine);
    mainLayout->addLayout(stackedLayout);
    mainLayout->addWidget(videoWidget);
    mainLayout->addLayout(buttonLine);
    setLayout(mainLayout);

    connect(stackedLayout, &QStackedLayout::currentChanged, this, &Guide::setCurrentVideo);

    stackedLayout->addWidget(makePage1());
    stackedLayout->addWidget(makePage2());
    stackedLayout->addWidget(makePage3());
    stackedLayout->setCurrentIndex(0);
}

void Guide::setCurrentVideo(int index)
{
    player->stop();
    auto item = videoForPage.find(stackedLayout->widget(index));
    if (item != videoForPage.end())
    {
        for (int i = 0; i < playlist.mediaCount(); i++)
        {
            if (playlist.media(i) == item.value())
            {
                playlist.setCurrentIndex(i);
            }
        }
        videoWidget->show();
        player->play();
    }
    else
    {
        playlist.setCurrentIndex(0); // No Content
        videoWidget->hide();
    }
}

QWidget* Guide::makePage1()
{
    QWidget *page = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QLabel *desc = new QLabel();
    desc->setText("11111");
    QTextEdit *edit = new QTextEdit();
    edit->setText("111111");

    mainLayout->setSpacing(20);
    mainLayout->addWidget(desc);
    mainLayout->addWidget(edit);

    page->setLayout(mainLayout);
    videoForPage.insert(page, QUrl::fromLocalFile(appDir.filePath("video1.mp4")));
    return page;
}

QWidget* Guide::makePage2()
{
    QWidget *page = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QLabel *desc = new QLabel();
    desc->setText("222222");
    QLabel *picLabel = new QLabel();
    QPixmap pic(appDir.absoluteFilePath("test1.png"));

    pic = pic.scaled(400, 200);
    picLabel->setPixmap(pic);
    picLabel->setScaledContents(true);
    //picLabel->setFixedSize(430,265);
    picLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    mainLayout->setSpacing(20);
    mainLayout->addWidget(desc);
    mainLayout->addWidget(picLabel);

    page->setLayout(mainLayout);
    //videoForPage.insert(page, QUrl::fromLocalFile(appDir.filePath("video2.mp4")));
    return page;
}

QWidget* Guide::makePage3()
{
    QWidget *page = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QLabel *desc = new QLabel();
    desc->setText("查询这些单词");
    QLabel *wordLabel = new QLabel();


   wordLabel->setText("查询这些单词: \n\n"
                      "abrogate ['æbrəuɡeit]   cultivated ['kʌltiveitid] \n\n"
                      "cumbersome ['kʌmbəsəm] quiver ['kwivə]  quota ['kwəutə] \n\n"
                      "rag [ræɡ]  rage [reidʒ]  ragged ['ræɡid] raid [reid] \n\n"
                       );
   wordLabel->setAlignment(Qt::AlignCenter);

    mainLayout->setSpacing(20);
    mainLayout->addWidget(desc);
    mainLayout->addWidget(wordLabel);

    page->setLayout(mainLayout);
    videoForPage.insert(page, QUrl::fromLocalFile(appDir.filePath("video3.mp4")));
    return page;
}


void Guide::onCtrlBtnClicked()
{
    int index = stackedLayout->currentIndex();
    if (sender() == preBtn)
    {
        index--;
    }
    if (sender() == nextBtn)
    {
        index++;
    }
    if (sender() == skipBtn || index >= stackedLayout->count())
    {
        close();
    }

    if (index < 0)
        ;
    else if (index == 0)
    {
        preBtn->setEnabled(false);
        stackedLayout->setCurrentIndex(index);
    }
    else if (index > 0 && index < stackedLayout->count() - 1)
    {
        preBtn->setEnabled(true);
        nextBtn->setText("下一步");
        stackedLayout->setCurrentIndex(index);
    }
    if (index == stackedLayout->count() - 1)
    {
        nextBtn->setText("完成");
        stackedLayout->setCurrentIndex(index);
    }

}


void Guide::closeEvent(QCloseEvent *event)
{
    player->stop();
    QWidget::closeEvent(event);
    this->deleteLater();
    instance = nullptr;

}
