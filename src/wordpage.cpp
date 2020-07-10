#include "wordpage.h"
#include "flowlayout.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QColor>
#include <QJsonObject>



WordPage::WordPage()
{
    initUI();
    QJsonObject obj;
    obj["n."]= "aaaa aaaa aaaa aaaa aaaa aaaa aaaa aaaa aaaa ";
    obj["an."]= "aaaa";
    obj["dn."]= "aaaa";
    updateDescription(obj);
    resize(200, this->heightForWidth(200));
}

void WordPage::initUI()
{
    QPalette palette;
    palette = this->palette();
    palette.setColor(QPalette::Background, QColor(255,255,255));
    this->setPalette(palette);

    titleLabel = new QLabel;
    leftAudioLabel = new QLabel;
    rightAudioLabel = new QLabel;
    mainlayout = new QVBoxLayout;
    this->setLayout(mainlayout);



    mainlayout->insertWidget(0, titleLabel);
    titleLabel->setText("Title");
    titleLabel->setFont(QFont("Noto Sans CJK SC Regular", 14));
    QHBoxLayout *audioLayout = new QHBoxLayout();
    audioLayout->addWidget(leftAudioLabel);
    leftAudioLabel->setText("comment1");
    //audioLayout->addStretch();
    audioLayout->addWidget(rightAudioLabel);
    rightAudioLabel->setText("comment2");
    mainlayout->insertLayout(1, audioLayout);
    this->adjustSize();
    palette =  titleLabel->palette();
    palette.setColor(QPalette::WindowText, QColor(88,86,83));
    // init descriptions
    for (int i = 0; i < 10; i++)
    {
        QHBoxLayout *l = new QHBoxLayout();
        l->setContentsMargins(0,5,0,5);

        QLabel *type = new QLabel();
        type->setFixedWidth(50);
        type->setFont(QFont("Noto Sans CJK SC Regular", 11));
        type->setPalette(palette);
        type->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        QLabel *desc = new QLabel();
        desc->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        QSizePolicy policy = desc->sizePolicy();
        policy.setHorizontalPolicy(QSizePolicy::Expanding);
        desc->setSizePolicy(policy);
        desc->setWordWrap(true);
        // desc->setText(" ");
        desc->setFont(QFont("Noto Sans CJK SC Regular", 11));
        l->insertWidget(0, type);
        l->insertWidget(1, desc);
        descriptions.append(l);
        // mainlayout->addLayout(l);
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
        tag->setFont(QFont("Noto Sans CJK SC Regular", 10));
        tag->setContentsMargins(5, 3, 5, 3);
        tag->setPalette(palette);
        tag->setAutoFillBackground(true);
        tags.append(tag);
    }
    mainlayout->addStretch();
    this->adjustSize();


}

void WordPage::updateDescription(QJsonObject obj)
{
    for (auto item : descriptions)
    {
        mainlayout->removeItem(item);
    }
    for (auto item : tags)
    {
        tagslayout->removeWidget(item);
    }

    auto keys = obj.keys();
    int i;
    for (i = 0; i < 10 && i < obj.count(); i++)
    {
        QLabel *type =  reinterpret_cast<QLabel*>(descriptions[i]->itemAt(0)->widget());
        type->setText(keys[i]);
        QLabel *desc =  reinterpret_cast<QLabel*>(descriptions[i]->itemAt(1)->widget());
        desc->setText(obj.value(keys[i]).toString());
        mainlayout->insertLayout(2 + i, descriptions[i]);
    }
    mainlayout->insertSpacing(2+i, 10);
    i++;

    mainlayout->insertLayout(2 + i, tagslayout);
    for (i = 0; i < 10 ; i++)
    {
        tagslayout->addWidget(tags[i]);
    }

}

//void WordPage::resizeEvent(QResizeEvent *event)
//{
//    int height = heightForWidth(event->size().width());
//    this->setFixedHeight(height);
//}
