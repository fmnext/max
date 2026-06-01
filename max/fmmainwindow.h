#ifndef FMMAINWINDOW_H
#define FMMAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QCloseEvent>
#include <qcombobox.h>
#include <QMessageBox>
#include <QProgressBar>
#include <QTreeWidget>
#include <QVariant>
#include <QPainter>
#include <QMenuBar>
#include <QSettings>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <string>
#include <sstream>

#include <QClipboard>
#include <QDir>
#include <QListview>
#include <QToolButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QUuid>
#include <QMenu>
#include <QDebug>
#include <QStyledItemDelegate>
#include <QMargins>
#include <QFont>

#include "MeshResolver.hpp"
#include "DCCManager.hpp"
#include "ContainerReader.hpp"
#include "GameResolver.hpp"
#include "TextureResolver.hpp"
#include "fmaboutwindow.h"
#include "fmfiledownloader.h"
#include "fmqtwindow.hpp"

#include "granny.h"
#include "gstate.h"

namespace Ui {
    class FMMainWindow;
}

class FMMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FMMainWindow(QWidget* parent = nullptr);
    ~FMMainWindow();

    void GetDatabase(const QString& path, const QString& mediaName);
    QString GetBuildNumber(const std::vector<char>& buffer);

    void setModel(const std::string& path);

private:
    Ui::FMMainWindow* ui;

    std::unique_ptr<fmnext::GameResolver> m_game = nullptr;
    //std::unique_ptr<fmnext::ContainerReader> m_container = nullptr;
    std::shared_ptr<fmnext::SceneReader::Scene> m_scene = nullptr;
    std::shared_ptr<fmnext::DataBaseRecords> m_records = nullptr;

    std::shared_ptr<fmnext::BundleReader::BundleData> m_tire = nullptr;
    std::shared_ptr<fmnext::BundleReader::BundleData> m_skel = nullptr;
    std::shared_ptr<fmnext::BundleReader::BundleData> m_proxyLOD = nullptr;
    std::shared_ptr<fmnext::BundleReader::BundleData> m_colors = nullptr;
    std::unique_ptr<fmnext::BundleReader::BundleData> m_thumbnail = nullptr;

    std::unordered_map<std::string, std::shared_ptr<fmnext::BundleReader::BundleData>> m_tires;

    QList<QVariant> m_DatabaseItems;

    QList<QTreeWidgetItem*> root_items;
    QList<QTreeWidgetItem*> scene_items;

    fmnext::ContainerReader m_container;
    std::vector<char> FindAssetInContainer(const std::string& path, int type = 0);

    std::vector<fmnext::ContainerReader> materials_container;
    std::vector<fmnext::ContainerReader> textures_container;

    std::unordered_map<std::string, std::vector<char>> material_list;
    std::unordered_map<std::string, std::string> texture_list;

    std::filesystem::path model_path;

    granny_file* m_CharacterFile = nullptr;

    QComboBox* current_lod = nullptr;
    QComboBox* upgrade_level = nullptr;
    QComboBox* color_override = nullptr;
    QComboBox* geometry_type = nullptr;

    std::map<int, bool> car_bodies;
    std::map<int, bool> car_upgrades;

    QList<fmnext::ModelItem> list_items;

    QList<QTreeWidgetItem*> bundle_entries;
    QList<QTreeWidgetItem*> material_entries;
    QList<QTreeWidgetItem*> texture_entries;

    QAction* filterObjectAct;
    QAction* filterMaterialAct;
    QAction* filterTextureAct;

    struct AssetItem
    {
        QString Name;
        QPixmap Icon;
    };

    bool has_db = false;

    QString RemoveGameBase(const std::string& path, const std::string& mediaName);

    void SetTextures(const std::shared_ptr<fmnext::SceneReader::CarRenderModel11>& model, const std::shared_ptr<fmnext::BundleReader::BundleData>& data);
    void SetTextures(const std::shared_ptr<fmnext::SceneReader::CarRenderModel11>& model, const std::shared_ptr<fmnext::BundleReader::BundleData>& data, std::shared_ptr<fmnext::BundleReader::BundleData>& primary_material, std::shared_ptr<fmnext::BundleReader::BundleData>& secondary_material);
    void SetSkeleton(const std::string& path);
    bool SetSkeleton(const std::vector<char>& blob);
    bool GetSkeleton();

    std::shared_ptr<fmnext::BundleReader::BundleData> SetBundleData(const std::shared_ptr<fmnext::SceneReader::CarRenderModel11>& model);
    std::shared_ptr<fmnext::BundleReader::BundleData> GetBundleData(std::vector<char>& data);
    std::shared_ptr<fmnext::BundleReader::BundleData> GetBundleData(const std::string& path);

    QString GetHexHash(int value);

    std::unordered_map<int32_t, fmnext::MaterialInstace> HandleShaders(const std::shared_ptr<fmnext::SceneReader::CarRenderModel11>& model, const std::shared_ptr<fmnext::BundleReader::BundleData>& bundle, const QString& scheme);
    std::vector<char> handleLibrary(const std::string& path);

    void savePreferences();
    void getPreferences();

    std::unique_ptr<FMAboutWindow> about_win = nullptr;

    int initStateMachine(const std::vector<char>& state);

    void Initialize(std::shared_ptr<fmnext::DataBaseRecords> p_records);

    void exportMaterialData(int bundle_index, const QString& path);
    void exportManufacturerColors(const QString& path);

    QJsonArray getShaderParametersArray(std::shared_ptr<fmnext::BundleReader::BundleData> bundle);

protected:
    void closeEvent(QCloseEvent* event);

};

#endif // FMMAINWINDOW_H