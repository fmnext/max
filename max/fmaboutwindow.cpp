#include "fmaboutwindow.h"
#include "ui_fmaboutwindow.h"

FMAboutWindow::FMAboutWindow(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FMAboutWindow)
{
    ui->setupUi(this);

    QPixmap pixMap = QPixmap(":/logos/svg/Button_Built_with_Qt_Black_RGB.svg");
    QPixmap pixMapScaled = pixMap.scaled(QSize(FMQtWindow::dpiScale(120), FMQtWindow::dpiScale(50)), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->pushButton->setFont(QFont("Segoe UI", 9, QFont::Normal));

    ui->logo->setPixmap(pixMapScaled);

    QFile file(":/assets/about.md");
    if (file.open(QFile::ReadOnly))
    {
        ui->text_legal->setTextFormat(Qt::RichText);
        ui->text_legal->setOpenExternalLinks(true);
        ui->text_legal->setText(file.readAll());
        ui->text_legal->setFont(QFont("Segoe UI", 9, QFont::Normal));
    }

    ui->verticalLayout_2->setContentsMargins(QMargins(FMQtWindow::dpiScale(8), FMQtWindow::dpiScale(8), FMQtWindow::dpiScale(8), FMQtWindow::dpiScale(8)));
    ui->verticalLayout_2->setSpacing(FMQtWindow::dpiScale(6));

    ui->versionFrame->setFixedHeight(FMQtWindow::dpiScale(50));

    ui->verticalLayout->setContentsMargins(QMargins(0, 0, 0, FMQtWindow::dpiScale(10)));
    ui->verticalLayout->setSpacing(FMQtWindow::dpiScale(10));

    ui->text_version->setFont(QFont("Courier", 9, QFont::Normal));
}

FMAboutWindow::~FMAboutWindow()
{
    delete ui;
}

void FMAboutWindow::setSuperiorText(const QString& text)
{
    ui->text_version->setText(text);
}

void FMAboutWindow::on_pushButton_clicked()
{
    this->close();
}

