#ifndef FMOPTIONSDIALOG_H
#define FMOPTIONSDIALOG_H

#include "preinclude.hpp"

#include <QWidget>
#include <QCombobox>
#include <QCheckBox>
#include <QPushButton>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QPoint>
#include <QSize>
#include <QFrame>
#include <QDebug>
#include <QSettings>
#include <string>

#include "MeshResolver.hpp"
#include "fmqtwindow.hpp"
#include "DCCManager.hpp"

class FMOptionsDialog : public QWidget
{
    Q_OBJECT
public:
    explicit FMOptionsDialog(QWidget *parent = nullptr);
    void setModelData(std::string& path);

private:
    QComboBox* levelOfDetail;
    QComboBox* geometryType;
    QCheckBox* materialOption;
    QCheckBox* skeletonOption;
    QPushButton* importButton;
    std::string m_path;

signals:
    // QWidget interface

private slots:
    void importData();
    void savePreferences();
    void getPreferences();

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // FMOPTIONSDIALOG_H
