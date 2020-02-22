#ifndef BAIDUTRANSLATE_H
#define BAIDUTRANSLATE_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>

class BaiduTranslate : QObject
{
    Q_OBJECT

public:
    static BaiduTranslate& instance()
    {
        static BaiduTranslate baiduTranslate;
        return baiduTranslate;
    }
    BaiduTranslate(const BaiduTranslate& bai) = delete;
    BaiduTranslate& operator==(const BaiduTranslate& bai) = delete;
    QString TranslateText(QString text, float timeleft);

private:
    BaiduTranslate();
    QNetworkAccessManager *manager;
    QString gtk;
    QString token;
    QString jsCode;
    QString getSign(QString query);
    void loadMainPage();
    QString langDetect(QString query);
    QJsonObject dictionary(QString query, QString dst="zh", QString src="");
    QString getUrl(QString url);

};




#endif // BAIDUTRANSLATE_H
