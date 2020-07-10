#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDir>
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QProcess>
#include <QTimer>
#include "baidutranslate.h"
#include "configtool.h"
#include <cmath>

BaiduTranslate::BaiduTranslate()
{
    timer.setSingleShot(true);
    timer.setInterval(3000);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    manager = new QNetworkAccessManager();
    connect(qApp, &QCoreApplication::aboutToQuit, this, [=]{
        delete manager;
        manager = nullptr;
    });
    // loadMainPage(); 如果开机自动启动，请求会在Wifi连接成功之前发送
    tokenURL = configTool->TokenURL;
    checkAccessToken();

}

int BaiduTranslate::ScreenShot()
{
    // 截图,如果成功f1和f2不相等，返回0
    QString cmd = "bash " + appDir.filePath("screenshot.sh");
    int cmd_res = system(cmd.toStdString().c_str());
    return cmd_res;
}

QString BaiduTranslate::getUrl(QString url_str)
{
    QUrl url(url_str);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_2) "
                                                    "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36");
    QNetworkReply *reply = manager->get(req);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (reply->error() != QNetworkReply::NoError)
    {
        qWarning() << reply->errorString();
    }
    QString res = QString::fromUtf8(reply->readAll());
    reply->deleteLater();
    return res;
}

void BaiduTranslate::loadMainPage()
{
    QString res;
    res = getUrl("https://fanyi.baidu.com/");
    res = getUrl("https://fanyi.baidu.com/");
    QRegExp regToken("token: '(.*)'");
    QRegExp regGtk("window.gtk = '(.*)'");
    regToken.setMinimal(true);
    regGtk.setMinimal(true);

    if (regToken.indexIn(res) != -1 && regGtk.indexIn(res) != -1)
    {
        token = regToken.cap(1);
        gtk = regGtk.cap(1);
    }
    else
    {
        qWarning() << "无法获取token gtk";
    }
}

QString BaiduTranslate::langDetect(QString query)
{
    QUrlQuery data;
    data.addQueryItem("query", query);

    QNetworkRequest req(QUrl("https://fanyi.baidu.com/langdetect"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_2) "
                                                    "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36");

    QNetworkReply *reply = manager->post(req, data.toString(QUrl::FullyEncoded).replace("+", "%2B").toUtf8());
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject obj = doc.object();
    if (obj["error"].toInt() == 0)
    {
        return obj["lan"].toString();
    }
    reply->deleteLater();
    return QString();
}

QString BaiduTranslate::getSign(QString query)
{
    QProcess nodejs;
    QStringList args;
    args << appDir.absoluteFilePath("baidu.js") << query << gtk;
    nodejs.start("nodejs", args);
    nodejs.waitForFinished(2000);

    QString result;
    if (nodejs.state() == QProcess::NotRunning && nodejs.exitStatus() == QProcess::NormalExit && nodejs.exitCode() == 0)
        result = nodejs.readAll().trimmed();
    else
    {
        qWarning() << "get sign error";
        nodejs.terminate();
    }

    return result;
}

QJsonObject BaiduTranslate::dictionary(QString query, QString dst, QString src, float timeLeft)
{
    QUrl url("https://fanyi.baidu.com/v2transapi");
    if (src.isEmpty())
        src = langDetect(query);
    QUrlQuery data;
    data.addQueryItem("from", src);
    data.addQueryItem("to", dst);
    data.addQueryItem("query", query);
    data.addQueryItem("simple_means_flag", "3");
    data.addQueryItem("sign", getSign(query));
    data.addQueryItem("token", token);

    qDebug() << "sign" << getSign(query);

    QNetworkRequest req(QUrl("https://fanyi.baidu.com/v2transapi"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_2) "
                                                    "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36");

    QNetworkReply *reply = manager->post(req, data.toString(QUrl::FullyEncoded).replace("+", "%2B").toUtf8());
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    timer.start(int(timeLeft * 1000));
    loop.exec();
    disconnect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QJsonDocument doc;
    QJsonObject obj;
    if (timer.isActive())
    {
        qInfo() << "Text: is active";
        timer.stop();
        if (reply->error() != QNetworkReply::NoError)
        {
            qWarning() << reply->errorString();
            obj["error"] = reply->errorString();
        }
        else
        {
            doc = QJsonDocument::fromJson(reply->readAll());
            obj = doc.object();
            if (obj.contains("error") || obj.contains("errno"))
            {
                qInfo() << obj;
                qWarning() << "Text: baidu sdk error";
            }
        }

    }
    else
    {
        obj["error"] = "time out";
        reply->abort();
    }

    reply->deleteLater();
    return obj;

}

QString BaiduTranslate::TranslateText(QString text, float timeleft)
{
    // 加载token
    if (token.isEmpty() || gtk.isEmpty())
        loadMainPage();

    // 判断是否为英语
    bool is_latin = true;
    for (const auto &c : text)
    {
        if (c.unicode() > 127)
            is_latin = false;
    }
    QJsonObject obj;
    if (is_latin)
        obj = dictionary(text, "zh", "en", timeleft);
    else
        obj = dictionary(text, "zh", langDetect(text), timeleft);

    if (obj.contains("dict_result"))
    {
        obj = obj["dict_result"].toObject()["simple_means"].toObject();
        QJsonDocument doc;
        doc.setObject(obj);
        return doc.toJson(QJsonDocument::Compact);
    }
    else if (obj.contains("trans_result"))
    {
       auto data_list = obj["trans_result"].toObject()["data"].toArray();
       QString res;
       for (const auto item : data_list)
       {
           res += item.toObject()["dst"].toString() + "<br/>";
       }
       return res;
    }
    else if (obj.contains("error"))
    {
        return obj["error"].toString();
    }
    else if (obj.empty())
    {
        return QString("empty");
    }
    qInfo() << obj;

    // 998错误则意味需要重新加载主页获取新的token
    if (obj.contains("errno") && obj["errno"].toInt() == 998)
    {
        loadMainPage();
        qInfo() << "loadMainPage";
    }
    return QString("error");
}

bool BaiduTranslate::checkAccessToken()
{
    bool ret = true;
    QFile file(dataDir.filePath("token"));
    if (file.exists() == false)
        ret = getAccessTokenFromURL(tokenURL);
    else
    {
        if (file.open(QIODevice::ReadOnly))
        {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            if (time(nullptr) > doc.object()["expires_at"].toVariant().toLongLong())
                ret = getAccessTokenFromURL(tokenURL);
            else
                access_token =  doc.object()["access_token"].toString();
        }
    }
    if (!ret)
        qWarning() << "获取 OCR token 失败";
    return ret;
}

bool BaiduTranslate::getAccessTokenFromURL(QString url)
{
    QString res = getUrl(url);
    QJsonDocument doc = QJsonDocument::fromJson(res.toUtf8());
    if (doc.object().contains("access_token") == false ||  doc.object().contains("expires_at") == false)
        return false;

    access_token = doc.object()["access_token"].toString();
    QFile file(dataDir.filePath("token"));
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(res.toUtf8());
        file.close();
    }
    return true;
}

QString BaiduTranslate::OCRTranslate(float timeLeft, bool screenshot)
{
    QString result_string = OCRText(timeLeft, screenshot);
    if (result_string == "time out")
        return "time out";
    // word correction
    if (result_string.size() < 20)
    {
        auto front = 0;
        auto back = result_string.size() - 1;
        while (front < result_string.size())
        {
            if (result_string[front].isLetter() == false)
                front++;
            else
                break;
        }
        while (back > 0)
        {
            if (result_string[back].isLetter() == false)
                back--;
            else
                break;
        }
        if (front < back)
            result_string = result_string.mid(front, back - front + 1);
    }
    return TranslateText(result_string, configTool->TextTimeout);
}

QString BaiduTranslate::OCRText(float timeLeft, bool screenshot)
{
    if (screenshot && ScreenShot() != 0)
        return QString("");
    if (access_token.isEmpty())
        return QString("OCR token is empty");
    QString image_string;
    QFile picture_file("/tmp/ocr");
    if (picture_file.open(QIODevice::ReadOnly))
    {
        image_string = picture_file.readAll().toBase64(QByteArray::Base64Encoding).toPercentEncoding();
        picture_file.close();
    }
    QUrlQuery data;
    data.addQueryItem("image", image_string);
    data.addQueryItem("detect_language", "true");
    // qInfo() << image_string;
    QNetworkRequest req(QUrl("https://aip.baidubce.com/rest/2.0/ocr/v1/general_basic?access_token=" + access_token));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_2) "
                                                    "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36");
    QNetworkReply *reply = manager->post(req, data.toString().toUtf8());
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    timer.start(int(timeLeft * 1000));
    loop.exec();
    if (timer.isActive())
    {
        timer.stop();
        if (reply->error() != QNetworkReply::NoError)
        {
            qInfo() << reply->errorString();
        }
    }
    else
    {
        qInfo() << "time out";
        disconnect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        reply->abort();
        reply->deleteLater();
        return QString("time out");
    }
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject obj = doc.object();
    if (obj.contains("error"))
    {
        qInfo() << "OCR: " << obj;
        qWarning() << "OCR: baidu sdk error";
    }
    reply->deleteLater();
    QString result_string;
    auto array = obj["words_result"].toArray();
    for (auto item : array)
    {
        result_string += item.toObject()["words"].toString() + " ";
    }
    return result_string;
}


