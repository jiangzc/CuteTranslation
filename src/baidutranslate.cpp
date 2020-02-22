#include <QCoreApplication>
#include <QDir>
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include "configtool.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "baidutranslate.h"

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
    qInfo() << this->TranslateText("hello, my name is jiangzc",1);

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
            execlp("nodejs", "nodejs", "-e",
                  (jsCode + QString("token(\"%1\",\"%2\");").arg(query).arg(gtk)).toStdString().c_str(),
                  "-p", nullptr);
        }
        else
        {
            int nread;
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

QJsonObject BaiduTranslate::dictionary(QString query, QString dst, QString src)
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
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
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
    bool is_latin = true;
    for (const auto &c : text)
    {
        if (c.unicode() > 127)
            is_latin = false;
    }
    QJsonObject obj;
    if (is_latin)
        obj = dictionary(text, "zh", "en");
    else
        obj = dictionary(text, "zh");

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
    return QString("error");
}

QString TranslateText(QString word, float timeLeft)
{
    auto byteArray = word.toUtf8();
    const char *word_chars = byteArray.constData();
    QString python_path = QCoreApplication::applicationDirPath() + "/translate_demo.py";

    QString result;
    // float timeLeft = 2.0; // max delay of sub process
    int pipes[2];
    pid_t pid;
    if (pipe(pipes) == 0)
    {
        // non-block reading from pipe
        if (fcntl(pipes[0], F_SETFL, O_NONBLOCK) < 0)
            exit(1);
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
            // exec program
            execl(python_path.toStdString().c_str(), "translate_demo.py", word_chars, (char *)NULL);
        }
        else
        {
            int nread;
            char buf[2000];
            close(pipes[1]);
            bool timeout = false;
            while (!timeout)
            {
                nread = read(pipes[0], buf, 2000);
                // read call  return -1 if pipe is empty (because of fcntl)
                switch (nread)
                {
                case -1:

                    // case -1 means pipe is empty and errono was set to EAGAIN
                    if (errno == EAGAIN)
                    {
                        printf("(pipe empty)\n");
                        usleep(100000); // sleep 100 ms
                        if (timeLeft > 0)
                        {
                            timeLeft -= 0.1;
                        }
                        else
                        {
                            timeout = true;
                        }
                        break;
                    }
                    else
                    {
                        perror("fail to read from pipe.\n");
                        return QString();
                    }

                // case 0 means all bytes are read and EOF(end of conv.)
                case 0:
                    printf("End of conversation\n");
                    close(pipes[0]);
                    int status;
                    wait(&status);
                    if (status == 0)
                        return result; // success
                    else
                        return QString("error"); // fail
                default:
                    // text read by default return n of bytes which read call read at that time
                    buf[nread] = '\0';
                    result += buf;
                }
            }
            // timeout
            close(pipes[0]);
            int status;
            kill(pid, SIGTERM);
            wait(&status);
            return QString("time out");
        }
    }
    else
    {
        perror("fail to create pipe.\n");
    }
    return QString(""); // if failed
}

int ScreenShot()
{
    // 截图,如果成功f1和f2不相等，返回0
    QString cmd = "bash " + QCoreApplication::applicationDirPath() + "/screenshot.sh";
    int cmd_res = system(cmd.toStdString().c_str());
    return cmd_res;
}

QString OCRTranslate(float timeLeft, bool screenshot=true)
{
    if (screenshot && ScreenShot() != 0)
        return QString("");

    QString python_path = QCoreApplication::applicationDirPath() + "/BaiduOCR.py";
    pid_t pid;
    QString result;
    QString res_short;
    // float timeLeft = 3.0; // max delay of sub process
    int pipes[2];

    if (pipe(pipes) == 0)
    {
        // non-block reading from pipe
        if (fcntl(pipes[0], F_SETFL, O_NONBLOCK) < 0)
            exit(1);
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
            // exec program
            execl(python_path.toStdString().c_str(), "BaiduOCR.py", (char *)NULL);
        }
        else
        {
            int nread;
            char buf[2000];
            close(pipes[1]);
            bool timeout = false;
            while (!timeout)
            {
                nread = read(pipes[0], buf, 2000);
                // read call  return -1 if pipe is empty (because of fcntl)
                switch (nread)
                {
                case -1:

                    // case -1 means pipe is empty and errono was set to EAGAIN
                    if (errno == EAGAIN)
                    {
                        printf("(pipe empty)\n");
                        usleep(100000); // sleep 100 ms
                        if (timeLeft > 0)
                        {
                            timeLeft -= 0.1;
                        }
                        else
                        {
                            timeout = true;
                        }
                        break;
                    }
                    else
                    {
                        perror("fail to read from pipe.\n");
                        return QString();
                    }

                // case 0 means all bytes are read and EOF(end of conv.)
                case 0:
                    printf("End of conversation\n");
                    close(pipes[0]);
                    int status;
                    wait(&status);
                    res_short = result;
                    res_short.truncate(20);
                    qInfo() << res_short << "...";
                    if (status == 0)
                    {
                        // word correction
                        if (result.size() < 20)
                        {
                            auto front = 0;
                            auto back = result.size() - 1;
                            while (front < result.size())
                            {
                                if (result[front].isLetter() == false)
                                    front++;
                                else
                                    break;
                            }
                            while (back > 0)
                            {
                                if (result[back].isLetter() == false)
                                    back--;
                                else
                                    break;
                            }
                            if (front < back)
                                result = result.mid(front, back - front + 1);
                        }
                        return TranslateText(result, configTool->TextTimeout); // success
                    }
                    else
                        return QString("error"); // fail
                default:
                    // text read by default return n of bytes which read call read at that time
                    buf[nread] = '\0';
                    result += buf;
                }
            }
            // timeout
            close(pipes[0]);
            int status;
            kill(pid, SIGTERM);
            wait(&status);
            return QString("time out");
        }
    }
    else
    {
        perror("fail to create pipe.\n");
    }
    return QString(""); // if failed
}
