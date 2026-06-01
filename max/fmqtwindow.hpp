#pragma once

#ifndef FORZAQTWINDOW_H
#define FORZAQTWINDOW_H

#include <QtGlobal>

#ifdef Q_OS_WIN

#include <Windows.h>
#include "Qt/QmaxMainWindow.h" 
#include "Qt/QmaxToolBar.h"
#include "Qt/QmaxDockingWinHost.h"
#include "Qt/QmaxDockingWinHostDelegate.h"

#include "Max.h"

const float DEFAULT_DPI = 96.0;

class FMQtWindow
{
public:
    FMQtWindow() = default;

    ~FMQtWindow() = default;

    static QWidget* mainWindow();

    static int dpiScale(int size);
    static float dpiScale(float size);

private:

};

#endif //Q_OS_WIN

#endif // FMQTWINDOW_H