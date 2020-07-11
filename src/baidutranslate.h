#ifndef BAIDUTRANSLATE_H
#define BAIDUTRANSLATE_H

class QNetworkAccessManager;
#include <QDebug>
#include <QEventLoop>
#include <QTimer>

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
    QString OCRText(float timeLeft, bool screenshot);
    QString getUrl(QString url);
    QByteArray getUrlContent(QString url_str);

private:
    BaiduTranslate();
    QEventLoop loop;
    QTimer timer;
    QNetworkAccessManager *manager;
    QString gtk;
    QString token;
    QString access_token;
    QString tokenURL;
    QString getSign(QString query);
    void loadMainPage();
    QString langDetect(QString query);
    QJsonObject dictionary(QString query, QString dst="zh", QString src="", float timeLeft=2.5);

    bool getAccessTokenFromURL(QString url);
    bool checkAccessToken();


};




#endif // BAIDUTRANSLATE_H
