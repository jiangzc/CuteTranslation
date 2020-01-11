#include <QCoreApplication>
#include <QDir>
#include <QString>
#include <QByteArray>
#include <QDebug>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


QString TranslateText(QString word, float timeLeft=0.7)
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

QString OCRTranslate(float timeLeft=1.0)
{
    // 截图,如果成功f1和f2不相等，返回0
    QString cmd = "bash " + QCoreApplication::applicationDirPath() + "/screenshot.sh";
    int cmd_res = system(cmd.toStdString().c_str());
    if (cmd_res != 0) // 截图失败
        return QString("");

    QString python_path = QCoreApplication::applicationDirPath() + "/BaiduOCR.py";
    pid_t pid;
    QString result;
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
                    qInfo() << result;
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
                        return TranslateText(result); // success
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
