#ifndef FMABOUTWINDOW_H
#define FMABOUTWINDOW_H

#include <QWidget>
#include <QFile>
#include <QFont>
#include <QSize>
#include <QMargins>

#include "fmqtwindow.hpp"

namespace Ui {
    class FMAboutWindow;
}

class FMAboutWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FMAboutWindow(QWidget* parent = nullptr);
    ~FMAboutWindow();

    void setSuperiorText(const QString& text);

private slots:
    void on_pushButton_clicked();

private:
    Ui::FMAboutWindow* ui;
};

#endif // FMABOUTWINDOW_H
