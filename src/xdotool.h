#ifndef XDOTOOL_H
#define XDOTOOL_H
#include <QPoint>
#include "event_monitor.h"


class Xdotool
{
public:
    Xdotool();
    ~Xdotool();
    Display *display;
    int screen;
    Window root_window;
    bool isRecording;
    QPoint leftButtonPressedPosition;
    QPoint leftButtonReleasedPosition;
    int getMousePosition (int &root_x, int &root_y);
    unsigned long getActiveWindowPID();
    char* getActiveWindowName();
    EventMonitor eventMonitor;
    int screenWidth;
    int screenHeight;

private:
    unsigned char *prop;
    void check_status(int status, unsigned long window);
    unsigned char* get_string_property(const char* property_name, Window window);
    unsigned long get_long_property(const char* property_name, Window window);
};

extern Xdotool xdotool;
#endif // XDOTOOL_H
