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
    int ScreenShot();
    QString OCRTranslate(float timeLeft, bool screenshot);

private:
    BaiduTranslate();
    QNetworkAccessManager *manager;
    QString gtk;
    QString token;
    QString access_token;
    QString jsCode;
    QString tokenURL;
    QString getSign(QString query);
    void loadMainPage();
    QString langDetect(QString query);
    QJsonObject dictionary(QString query, QString dst="zh", QString src="");
    QString getUrl(QString url);
    bool getAccessTokenFromURL(QString url);
    bool checkAccessToken();


};




#endif // BAIDUTRANSLATE_H
