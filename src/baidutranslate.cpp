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


QString TranslateWord(QString word)
{
    auto byteArray = word.toUtf8();
    const char *word_chars = byteArray.constData();
    QString python_path = QCoreApplication::applicationDirPath() + "/translate_demo.py";

    QString result;
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
            while (1)
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
                    return result; // success
                default:
                    // text read by default return n of bytes which read call read at that time
                    buf[nread] = '\0';
                    result += buf;
                }
            }
        }
    }
    else
    {
        perror("fail to create pipe.\n");
    }
    return QString(""); // if failed
}
