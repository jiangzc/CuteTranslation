#ifndef CONFIG_H
#define CONFIG_H
#include <QSettings>
#include <QString>
#include <QDebug>



class ConfigTool
{
  public:
    ConfigTool();

    static QSettings settings;
    // QString Mode;
    class
    {

      public:
        QString value;
        QString &operator=(const QString &i)
        {
            qDebug() << "aaa";
            qDebug() << ConfigTool::settings.value("/Genenal/Mode");
            ConfigTool::settings.setValue("/Genenal/Mode", i);
            return value = i;
        }
        bool operator==(const QString str)
        {
            qDebug() << "bbb";
            return (str == value);
        }
        operator QString() const
        {
            return value;
        }

    } Mode;

    int TriangleHeight;
    int TriangleWidth;
    int Edge;
    int Direction;
    int MainWindowWidth;
    int MainWindowHeight;
    int FloatButtonWidth;
    int FloatButtonHeight;
    QString Undefined;
    QString Show;
    QString NotShow;
};

extern ConfigTool configTool;

#endif // CONFIG_H
