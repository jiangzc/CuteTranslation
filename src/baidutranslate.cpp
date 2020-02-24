#include <QCoreApplication>
#include <QDir>
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QTimer>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "baidutranslate.h"
#include "configtool.h"

BaiduTranslate::BaiduTranslate()
{
    manager = new QNetworkAccessManager();
    connect(qApp, &QCoreApplication::aboutToQuit, this, [=]{
        delete manager;
        manager = nullptr;
    });
    loadMainPage();
    QFile jsFile(appDir.filePath("baidu.js"));
    if (jsFile.open(QIODevice::ReadOnly))
    {
        jsCode = QString::fromUtf8(jsFile.readAll());
        jsFile.close();
    }
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
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
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
        qInfo() << "无法获取token gtk";
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
    QNetworkReply *reply = manager->post(req, data.toString().toUtf8());
    QEventLoop loop;
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
    QString result;
    // float timeLeft = 2.0; // max delay of sub process
    int pipes[2];
    pid_t pid;
    if (pipe(pipes) == 0)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("cannot fork.\n");
            exit(2);
        }
        else if (pid == 0)
        {
            // sub process

            // redirect stdout to the pipe
            close(1);
            dup(pipes[1]);
            close(pipes[0]);
            close(pipes[1]);
            // Fix Bug: 字符串构造脚本,特殊字符要处理
            QString escape = "\\";
            query.replace(QChar(92), escape + escape);
            query.replace(QChar(39), escape + QChar(39));
            query.replace(QChar(34), escape + QChar(34));
            query.replace(QChar(38), escape + QChar(38));
            query.replace(QChar(10), escape + 'n');
            query.replace(QChar(13), escape + 'r');
            query.replace(QChar(9), escape + 't');
            query.replace(QChar(8), escape + 'b');
            query.replace(QChar(12), escape + 'f');

            execlp("nodejs", "nodejs", "-e",
                  (jsCode + QString("token(\"%1\",\"%2\");").arg(query).arg(gtk)).toStdString().c_str(),
                  "-p", nullptr);
        }
        else
        {
            long nread;
            char buf[100];
            close(pipes[1]);

            nread = read(pipes[0], buf, 100);
            buf[nread] = '\0';
            result = buf;
            close(pipes[0]);
            int status;
            wait(&status);
            return result.trimmed();
        }
    }
    else
    {
        perror("fail to create pipe.\n");
    }
    return QString(""); // if failed
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

    QNetworkRequest req(QUrl("https://fanyi.baidu.com/v2transapi"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_2) "
                                                    "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36");
    QNetworkReply *reply = manager->post(req, data.toString().toUtf8());
    QEventLoop loop;
    QTimer::singleShot(int(timeLeft * 1000), &loop, &QEventLoop::quit);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (reply->isFinished() == false)
        return QJsonObject();
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject obj = doc.object();
    if (obj.contains("error"))
    {
        qWarning() << "baidu sdk error";
    }
    reply->deleteLater();
    return obj;

}

QString BaiduTranslate::TranslateText(QString text, float timeleft)
{
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
    else if (obj.empty())
    {
        return QString("time out");
    }
    qInfo() << obj;
    return QString("error");
}

bool BaiduTranslate::checkAccessToken()
{
    QFile file(dataDir.filePath("token"));
    if (file.exists() == false)
        getAccessTokenFromURL(tokenURL);
    else
    {
        if (file.open(QIODevice::ReadOnly))
        {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            if (time(nullptr) > doc.object()["expires_at"].toString().toLong())
                getAccessTokenFromURL(tokenURL);
            else
                access_token =  doc.object()["access_token"].toString();
        }
    }
    return true;
}

bool BaiduTranslate::getAccessTokenFromURL(QString url)
{
    QString res = getUrl(url);
    QJsonDocument doc = QJsonDocument::fromJson(res.toUtf8());
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
    if (screenshot && ScreenShot() != 0)
        return QString("");
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
    qInfo() << image_string;
    QNetworkRequest req(QUrl("https://aip.baidubce.com/rest/2.0/ocr/v1/general_basic?access_token=" + access_token));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_2) "
                                                    "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36");
    QNetworkReply *reply = manager->post(req, data.toString().toUtf8());
    QEventLoop loop;
    QTimer::singleShot(int(timeLeft * 1000), &loop, &QEventLoop::quit);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (reply->isFinished() == false)
        return QString("time out");
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject obj = doc.object();
    if (obj.contains("error"))
    {
        qWarning() << "baidu sdk error" << obj;
    }
    reply->deleteLater();
    QString result_string;
    auto array = obj["words_result"].toArray();
    for (auto item : array)
    {
        result_string += item.toObject()["words"].toString() + " ";
    }
    qInfo() << result_string;
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


