#include "wordpage.h"
#include "flowlayout.h"
#include "baidutranslate.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QColor>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QPushButton>
#include <QMediaPlayer>
#include <QBuffer>

ClickableLabel::ClickableLabel(QWidget* parent)
    : QLabel(parent) {

}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}

WordPage::WordPage()
{
    initUI();
}

void WordPage::initUI()
{
    player = new QMediaPlayer;
    player->setVolume(100);

    QPalette palette;
    palette = this->palette();
    palette.setColor(QPalette::Background, QColor(255,255,255));
    this->setPalette(palette);

    titleLabel = new QLabel;
    leftAudioLabel = new QLabel;
    rightAudioLabel = new QLabel;
    mainlayout = new QVBoxLayout;

    QPixmap voicePic(":/pic/icons-voice.png");
    voicePic = voicePic.scaled(35, 35, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    ClickableLabel *leftvoiceButton = new ClickableLabel;
    leftvoiceButton->setFixedHeight(35);
    leftvoiceButton->setContentsMargins(0, 2, 0, 0);
    leftvoiceButton->setPixmap(voicePic);
    leftvoiceButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    connect(leftvoiceButton, &ClickableLabel::clicked, this, [=]{
        //player->stop();
        //player->setMedia(QMediaContent());
        //player->setMedia(QUrl("https://fanyi.baidu.com/gettts?lan=uk&text=" + titleLabel->text() + "&spd=3&source=web"));
        // player->stop();
        player->play();
    });

//    connect(player, &QMediaPlayer::mediaStatusChanged, this, [=](){
//        if (player->mediaStatus() == QMediaPlayer::LoadedMedia)
//            player->play();
//    });

    QLabel *rightvoiceButton = new QLabel;
    rightvoiceButton->setFixedHeight(35);
    rightvoiceButton->setContentsMargins(0, 2, 0, 0);
    rightvoiceButton->setPixmap(voicePic);
    rightvoiceButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);


    leftAudioLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    rightAudioLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    QFont font("Noto Sans CJK SC Regular");

    font.setWeight(QFont::DemiBold);

    titleLabel->setText("Title");
    font.setPixelSize(28);
    titleLabel->setFont(font);
    mainlayout->insertWidget(0, titleLabel);
    mainlayout->insertSpacing(1, 10);
    font.setWeight(QFont::Normal);

    //

    QHBoxLayout *audioLayout = new QHBoxLayout();
    audioLayout->addWidget(leftAudioLabel);
    audioLayout->addSpacing(10);
    audioLayout->addWidget(leftvoiceButton);
    leftAudioLabel->setText("comment1");

    audioLayout->addWidget(rightAudioLabel);
    audioLayout->addSpacing(10);
    audioLayout->addWidget(rightvoiceButton);
    rightAudioLabel->setText("comment2");
    mainlayout->insertLayout(2, audioLayout);

    font.setPixelSize(20);

    leftAudioLabel->setFont(font);
    rightAudioLabel->setFont(font);


    palette =  titleLabel->palette();
    palette.setColor(QPalette::WindowText, QColor(88,86,83));
    // init descriptions
    for (int i = 0; i < 10; i++)
    {
        QHBoxLayout *l = new QHBoxLayout();
        l->setContentsMargins(0,5,0,5);

        QLabel *type = new QLabel();
        type->setFixedWidth(50);
        font.setPixelSize(18);
        type->setFont(font);
        type->setPalette(palette);
        type->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        QLabel *desc = new QLabel();
        desc->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        QSizePolicy policy = desc->sizePolicy();
        policy.setHorizontalPolicy(QSizePolicy::Expanding);
        desc->setSizePolicy(policy);
        desc->setWordWrap(true);
        // desc->setText(" ");
        desc->setFont(font);
        l->insertWidget(0, type);
        l->insertWidget(1, desc);
        descriptions.append(l);
        mainlayout->addLayout(l); // 12
    }

    // init tags
    tagslayout = new FlowLayout;
    palette = QPalette();
    palette.setColor(QPalette::ColorRole::WindowText, Qt::gray);
    palette.setColor(QPalette::ColorRole::Window, QColor("#F2F1F6"));
    for (int i = 0; i < 10; i++)
    {
        QLabel *tag = new QLabel;
        // set color
        tag->setText("考研");
        tag->setFont(font);
        tag->setContentsMargins(5, 3, 5, 3);
        tag->setPalette(palette);
        tag->setAutoFillBackground(true);
        tagslayout->addWidget(tag);
        tags.append(tag);
    }
    mainlayout->insertSpacing(13, 10);
    mainlayout->insertLayout(14, tagslayout);
    mainlayout->addStretch();

    this->setLayout(mainlayout);
    this->adjustSize();


}

void WordPage::updateDescription(const QJsonObject &obj)
{
    for (auto item : descriptions)
    {
        // NOTE THIS
        // 从布局管理器移除元素，元素会显示在它的父控件上，所以这里要把它隐藏起来
        // 布局管理器只控制大小和位置，remove不改变可见性。
        mainlayout->removeItem(item);
        item->itemAt(0)->widget()->hide();
        item->itemAt(1)->widget()->hide();
    }
    for (auto item : tags)
    {
        // tagslayout->removeWidget(item);
        item->hide();
    }

    titleLabel->setText(obj["word_name"].toString());

    QJsonObject symbols = obj["symbols"].toArray().at(0).toObject();
    leftAudioLabel->setText("英 [ " + symbols["ph_en"].toString() + " ]");
    rightAudioLabel->setText("美 [ " + symbols["ph_am"].toString() + " ]");

    QJsonArray parts = symbols["parts"].toArray();
    int i = 0;
    for (i = 0; i < 9 && i < parts.count(); i++)
    {
        QJsonObject item = parts[i].toObject();
        QLabel *type =  reinterpret_cast<QLabel*>(descriptions[i]->itemAt(0)->widget());
        type->setText(item["part"].toString());
        QLabel *desc =  reinterpret_cast<QLabel*>(descriptions[i]->itemAt(1)->widget());
        QString res;
        for (const auto mean : item["means"].toArray())
            res.append(mean.toString() + "，");
        res.remove(res.count() - 1, 1);
        desc->setText(res);
        mainlayout->insertLayout(3 + i, descriptions[i]);
        type->show();
        desc->show();
    }
    if (obj.contains("exchange"))
    {
        QJsonObject item = obj["exchange"].toObject();
        QLabel *type =  reinterpret_cast<QLabel*>(descriptions[i]->itemAt(0)->widget());
        type->setText("ex.");
        QLabel *desc =  reinterpret_cast<QLabel*>(descriptions[i]->itemAt(1)->widget());
        QString res;
        for (const auto &form : item.keys())
        {
            res += form + ": ";
            for (const auto word : item[form].toArray())
                res.append(word.toString() + " ");
            res += "; ";
        }

        desc->setText(res);
        mainlayout->insertLayout(3 + i, descriptions[i]);
        type->show();
        desc->show();
    }

    i = 0;
    for (auto item : obj["tags"].toObject()["core"].toArray())
    {
        if (i >= 10 || item.toString().isEmpty())
            break;
        tags[i]->setText(item.toString());
        tags[i++]->show();
    }
    for (auto item : obj["tags"].toObject()["other"].toArray())
    {
        if (i >= 10 || item.toString().isEmpty())
            break;
        tags[i]->setText(item.toString());
        tags[i++]->show();
    }

    this->update();
    mainlayout->update();

   // player->setMedia(QUrl("https://fanyi.baidu.com/gettts?lan=uk&text=" + titleLabel->text() + "&spd=3&source=web"));
    QBuffer *buf = new QBuffer;
    buf->setData(BaiduTranslate::instance().getUrlContent("https://fanyi.baidu.com/gettts?lan=uk&text=" + titleLabel->text() + "&spd=3&source=web"));
    buf->open(QBuffer::ReadOnly);
    player->setMedia(QMediaContent(), buf);
}

//void WordPage::resizeEvent(QResizeEvent *event)
//{
//    int height = heightForWidth(event->size().width());
//    this->setFixedHeight(height);
//}
