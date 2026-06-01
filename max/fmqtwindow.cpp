#include "fmqtwindow.hpp"

QWidget* FMQtWindow::mainWindow()
{
    QWidget* ptr_mainwindow = GetCOREInterface()->GetQmaxMainWindow();

    return ptr_mainwindow;
}

int FMQtWindow::dpiScale(int size)
{
    HDC screen = GetDC(0);
    FLOAT dpiX = static_cast<FLOAT>(GetDeviceCaps(screen, LOGPIXELSX));
    ReleaseDC(0, screen);

    return static_cast<int>(size * (dpiX / DEFAULT_DPI));
}

float FMQtWindow::dpiScale(float size)
{
    HDC screen = GetDC(0);
    FLOAT dpiX = static_cast<FLOAT>(GetDeviceCaps(screen, LOGPIXELSX));
    ReleaseDC(0, screen);

    return size * (dpiX / DEFAULT_DPI);
}
