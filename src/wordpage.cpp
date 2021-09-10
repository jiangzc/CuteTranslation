#include "wordpage.h"
#include "flowlayout.h"
#include "baidutranslate.h"
#include "configtool.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QColor>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QPushButton>
#include <QPainter>
#include <QStyleOption>
#include <QMediaPlayer>
#include <QBuffer>

ClickableLabel::ClickableLabel(QWidget* parent)
    : QLabel(parent) {

}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* ) {
    emit clicked();
}

WordPage::WordPage(QWidget *parent) : QWidget (parent)
{
    initUI();
}

void WordPage::initUI()
{

    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_TranslucentBackground);
    // 单词音频播放器
    player = new QMediaPlayer(this);
    player->setVolume(100);
    voiceBuffer = new QBuffer(this);

    // 声音图标
    QPixmap voicePic(":/pic/icons-voice.png");
    voicePic = voicePic.scaled(35, 35, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    titleLabel = new QLabel(this);
    leftAudioLabel = new QLabel(this);
    rightAudioLabel = new QLabel(this);
    titleLabel->setObjectName("titleLabel");
    leftAudioLabel->setObjectName("audioLabel");
    rightAudioLabel->setObjectName("audioLabel");
    mainlayout = new QVBoxLayout;

    titleLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    leftAudioLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    rightAudioLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    // 单词
    titleLabel->setText("Title");
    mainlayout->insertWidget(0, titleLabel);
    mainlayout->insertSpacing(1, 10);

    // 声音按钮

    ClickableLabel *leftVoiceButton = new ClickableLabel(this);
    leftVoiceButton->setObjectName("uk"); // 英国
    leftVoiceButton->setFixedHeight(35);
    leftVoiceButton->setContentsMargins(0, 2, 0, 0);
    leftVoiceButton->setPixmap(voicePic);
    connect(leftVoiceButton, &ClickableLabel::clicked, this, &WordPage::onVoiceButtonClicked);

    ClickableLabel *rightVoiceButton = new ClickableLabel(this);
    rightVoiceButton->setObjectName("en"); // 美国
    rightVoiceButton->setFixedHeight(35);
    rightVoiceButton->setContentsMargins(0, 2, 0, 0);
    rightVoiceButton->setPixmap(voicePic);
    connect(rightVoiceButton, &ClickableLabel::clicked, this, &WordPage::onVoiceButtonClicked);

    // 音标
    QHBoxLayout *audioLayout = new QHBoxLayout;
    audioLayout->addWidget(leftAudioLabel);
    audioLayout->addSpacing(10);
    audioLayout->addWidget(leftVoiceButton);
    audioLayout->addStretch();
    leftAudioLabel->setText("Phonetic symbol 1");

    audioLayout->addWidget(rightAudioLabel);
    audioLayout->addSpacing(10);
    audioLayout->addWidget(rightVoiceButton);
    audioLayout->addStretch();
    rightAudioLabel->setText("Phonetic symbol 2");
    mainlayout->insertLayout(2, audioLayout);

    // init descriptions
    for (int i = 0; i < 10; i++)
    {
        QHBoxLayout *l = new QHBoxLayout;
        l->setContentsMargins(0,5,0,5);

        QLabel *type = new QLabel(this);
        type->setObjectName("type");
        type->setFixedWidth(50);
        type->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        type->setTextInteractionFlags(Qt::TextSelectableByMouse);
        QLabel *desc = new QLabel();
        desc->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        desc->setWordWrap(true);
        desc->setTextInteractionFlags(Qt::TextSelectableByMouse);
        desc->setObjectName("desc");
        l->insertWidget(0, type);
        l->insertWidget(1, desc);
        descriptions.append(l);
        mainlayout->addLayout(l); // 12
    }

    // init tags
    tagslayout = new FlowLayout;
    for (int i = 0; i < 10; i++)
    {
        QLabel *tag = new QLabel;
        tag->setObjectName("tag");
        tag->setContentsMargins(5, 3, 5, 3);
        // tag->setPalette(palette);
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

    // 音标
    QJsonObject symbols = obj["symbols"].toArray().at(0).toObject();
    leftAudioLabel->setText("英 [ " + symbols["ph_en"].toString() + " ]");
    rightAudioLabel->setText("美 [ " + symbols["ph_am"].toString() + " ]");
    // 解释
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
    // 记忆技巧
    if (obj.contains("memory_skill"))
    {
        QLabel *type =  reinterpret_cast<QLabel*>(descriptions[i]->itemAt(0)->widget());
        type->setText("tip");
        QLabel *desc =  reinterpret_cast<QLabel*>(descriptions[i]->itemAt(1)->widget());
        desc->setText(obj["memory_skill"].toString());
        mainlayout->insertLayout(3 + i, descriptions[i]);
        type->show();
        desc->show();
    }
    i++;
    // 词形变换
    if (obj.contains("exchange"))
    {
        QJsonObject item = obj["exchange"].toObject();
        QLabel *type =  reinterpret_cast<QLabel*>(descriptions[i]->itemAt(0)->widget());
        type->setText("ex");
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

    // 单词标签
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

    mainlayout->update();
    this->update();

}

QString WordPage::getText()
{
    QString res = QString("%1 \n %2   %3  \n").arg(titleLabel->text()).arg(leftAudioLabel->text()).arg(rightAudioLabel->text());

    for (int i = 3; i < mainlayout->count(); i++)
    {
        QHBoxLayout *hlayout = dynamic_cast<QHBoxLayout *>(mainlayout->itemAt(i)->layout());
        if (hlayout)
        {
            QLabel *type = dynamic_cast<QLabel*>(hlayout->itemAt(0)->widget());
            QLabel *desc = dynamic_cast<QLabel*>(hlayout->itemAt(1)->widget());
            if (type && desc && type->text().endsWith("."))
            {
                res += type->text() + "  " + desc->text() + "\n";
            }
        }
    }
    return res;

}

void WordPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void WordPage::onVoiceButtonClicked()
{
    if (voiceBuffer->isOpen())
        voiceBuffer->close();
    QString voiceUrl = "https://fanyi.baidu.com/gettts?lan=" + sender()->objectName() +
                       "&text=" + titleLabel->text() + "&spd=3&source=web";
    if (voiceBuffer->property("voiceUrl").toString() != voiceUrl)
    {
        voiceBuffer->setData(BaiduTranslate::instance().getUrlRawContent(voiceUrl));
        voiceBuffer->setProperty("voiceUrl", voiceUrl);
    }
    voiceBuffer->open(QBuffer::ReadOnly);
    player->setMedia(QMediaContent(), voiceBuffer);
    player->play();
}

