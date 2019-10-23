#ifndef CONFIG_H
#define CONFIG_H
#include <QString>
#include <QSettings>



class ConfigTool
{
  public:
    ConfigTool();
    QString Mode;
    int TriangleHeight;
    int TriangleWidth;
    int Edge;
    int Direction;
    int MainWindowWidth;
    int MainWindowHeight;
    int FloatButtonWidth;
    int FloatButtonHeight;
    QString Default;
    QString Show;
    QString NotShow;

private:
    QSettings settings;
};

extern ConfigTool configTool;

#endif // CONFIG_H
