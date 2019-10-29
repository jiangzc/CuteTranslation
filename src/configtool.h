#ifndef CONFIG_H
#define CONFIG_H
#include <QSettings>
#include <QString>

/* ConfigTool 中使用了匿名类来实现C#-style类的属性的功能
 * 重载了匿名类的赋值运算符作为set函数
 * 设置了匿名类的类型转换 class -> type 作为get函数
 * 具体参考 https://stackoverflow.com/questions/8368512/does-c11-have-c-style-properties
 */

class ConfigTool
{
  public:
    ConfigTool();

    static QSettings settings;
    // QString Mode ;
    class
    {

      public:
        QString value;
        QString &operator=(const QString &i)
        {
            ConfigTool::settings.setValue("/Genenal/Mode", i);
            return value = i;
        }
        bool operator==(const QString str)
        {
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
