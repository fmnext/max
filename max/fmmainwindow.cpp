#include "fmmainwindow.h"
#include "ui_fmmainwindow.h"

FMMainWindow::FMMainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::FMMainWindow)
{
    ui->setupUi(this);

    QPixmap logo = QPixmap(":/logos/svg/logo_forza_4a9f76e71d.svg");
    //QPixmap logo_scaled = logo.scaled(ui->horizontalLayout_5->sizeHint(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap logo_scaled = logo.scaled(QSize(FMQtWindow::dpiScale(80), FMQtWindow::dpiScale(80)), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->label_logo->setPixmap(logo_scaled);

    ui->progressBar->setFixedHeight(FMQtWindow::dpiScale(5));
    ui->verticalLayout_3->setSpacing(FMQtWindow::dpiScale(8));
    ui->horizontalLayout_2->setContentsMargins(QMargins(FMQtWindow::dpiScale(8), FMQtWindow::dpiScale(8), FMQtWindow::dpiScale(8), FMQtWindow::dpiScale(8)));
    ui->horizontalLayout->setSpacing(FMQtWindow::dpiScale(10));
    ui->horizontalLayout->setContentsMargins(QMargins(FMQtWindow::dpiScale(10), FMQtWindow::dpiScale(8), FMQtWindow::dpiScale(10), FMQtWindow::dpiScale(8)));
    ui->treeWidget->setIndentation(FMQtWindow::dpiScale(24));

    QList<QString> columms{ "Name", "Type", "Data" };

    QList<AssetItem> scene_sections = {
        { "UUID", QPixmap(":/icons/svg/snow.svg") },
        { "Thumbnail", QPixmap(":/icons/svg/image.svg") },
        { "Media Name", QPixmap(":/icons/svg/code-slash.svg") },
        { "Ordinal", QPixmap(":/icons/svg/code-slash.svg") },
        { "Skeleton", QPixmap(":/icons/svg/stack.svg") },
        { "Level of Detail", QPixmap(":/icons/svg/sliders.svg") },
        { "Upgrade Level", QPixmap(":/icons/svg/sliders.svg") },
        { "Render Pass", QPixmap(":/icons/svg/sliders.svg") },
        { "Color Override", QPixmap(":/icons/svg/sliders.svg") },
        { "Geometry Type", QPixmap(":/icons/svg/sliders.svg") },
        { "Parts", QPixmap(":/icons/svg/layers-fill.svg") }
    };

    QList<AssetItem> sections = {
        {"Skeleton", QPixmap(":/icons/svg/skeleton.svg")},
        {"Scene", QPixmap(":/icons/svg/layers-fill.svg")},
        {"Manufacturer Colors", QPixmap(":/icons/svg/material.svg")},
        {"Build Number", QPixmap(":/icons/svg/snow.svg")},
        {"Animations", QPixmap(":/icons/svg/animation.svg")},
        {"Materials", QPixmap(":/icons/svg/material.svg")},
        {"Database", QPixmap(":/icons/svg/database-fill.svg")},
        {"Game Series", QPixmap(":/logos/svg/logo_forza_4a9f76e71d.svg")},
        {"Plugin", QPixmap(":/logos/svg/logo_forza_4a9f76e71d.svg")}
    };

    //ui->treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->treeWidget->setColumnCount(columms.size());
    ui->treeWidget->setHeaderLabels(columms);
    ui->treeWidget->setColumnWidth(0, FMQtWindow::dpiScale(180));
    ui->treeWidget->setColumnWidth(1, FMQtWindow::dpiScale(70));
    //ui->treeWidget->setColumnWidth(2, FMQtWindow::dpiScale(200));

    ui->pushButton_import->setFont(QFont("Segoe UI", 9, QFont::Normal));
    ui->pushButton_reset->setFont(QFont("Segoe UI", 9, QFont::Normal));
    ui->treeWidget->setFont(QFont("Segoe UI", 9, QFont::Normal));
    ui->label_2->setFont(QFont("Segoe UI", 9, QFont::Normal));

    QTreeWidgetItem* root = new QTreeWidgetItem();
    root->setText(0, "<Root>");
    //root->setIcon(0, QPixmap(":/icons/icons/Untitled-17.png"));
    //ui->treeWidget->addTopLevelItem(root);
    //root->setExpanded(true);

    for (auto& section : sections) {
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, section.Name);
        item->setIcon(0, section.Icon);

        if (!section.Name.contains("Build Number", Qt::CaseSensitive))
        {
            item->setText(1, "Scope");

            //QPixmap icon(":/icons/icons/Untitled-10.png");
            //item->setIcon(1, icon);
        }

        root_items.append(item);

        ui->treeWidget->addTopLevelItem(item);
    }

    for (auto& section_item : scene_sections)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setIcon(0, section_item.Icon);
        item->setText(0, section_item.Name);
        item->setText(1, "Scope");

        //item_sections[1]->addChild(item);

        root_items[1]->addChild(item);
        scene_items.append(item);
    }


    //root_items[0]->setDisabled(true);
    //root_items[2]->setDisabled(true);
    //root_items[4]->setDisabled(true);
    //root_items[5]->setDisabled(true);
    scene_items[4]->setDisabled(true);

    //ui->pushButton->hide();

    QObject::connect(ui->treeWidget, &QTreeWidget::itemSelectionChanged, this, [&]()
        {
            ui->pushButton_import->setText("Import");

            for (auto& data : list_items)
            {
                if (data.item->isSelected())
                {
                    ui->pushButton_import->setText("Import Selected");

                    continue;
                }
            }
        });

    QObject::connect(ui->treeWidget, &QTreeWidget::currentItemChanged, this, [&](QTreeWidgetItem* current, QTreeWidgetItem* previous) {

        });

    {
        geometry_type = new QComboBox();
        geometry_type->setDisabled(scene_items[9]->isDisabled());
        geometry_type->setFocusPolicy(Qt::NoFocus);
        geometry_type->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        geometry_type->setItemDelegate(new QStyledItemDelegate());

        geometry_type->addItem(QString("Triangles"));
        geometry_type->addItem(QString("Quads"));

        scene_items[9]->setText(1, "Flags");
        scene_items[9]->setSizeHint(2, geometry_type->sizeHint());
        ui->treeWidget->setItemWidget(scene_items[9], 2, geometry_type);

        QFrame* comboFrame2 = geometry_type->findChild<QFrame*>();
        comboFrame2->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        comboFrame2->setAttribute(Qt::WA_TranslucentBackground);
    }
    /*
    QObject::connect(geometry_type, &QComboBox::currentIndexChanged, this, [&](int index) {
        if (index == 1) {
            QMessageBox msgBox(FMQtWindow::mainWindow());
            msgBox.setWindowTitle("Warning");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Changing the geometry type to quads can lead to some data loss.\n\n EDGEVISFLAGS=(1, 1, 0)\n SMOOTH_GROUPS=0x0F\n WELD_VERTICES_THRESHOLD=0.0001f\n");
            msgBox.exec();
        }
        });
    */

    QObject::connect(ui->treeWidget, &QTreeView::customContextMenuRequested, this, [=](const QPoint& pos) {

        if (ui->treeWidget->currentItem())
        {
            QMenu* treeMenu = new QMenu(this);
            treeMenu->setWindowFlag(Qt::FramelessWindowHint);
            treeMenu->setWindowFlag(Qt::NoDropShadowWindowHint);
            treeMenu->setAttribute(Qt::WA_TranslucentBackground);
            /*
                    if (ui->treeWidget->currentItem()->text(0) == "Scene")
                    {
                        QAction* importAction = new QAction("Import");
                        QAction* selAllAction = new QAction("Select All");
                        QAction* extractAction = new QAction("Extract");
                        //QAction* deselAllAction = new QAction("Deselect All");

                        importAction->setDisabled(true);

                        QObject::connect(importAction, &QAction::triggered, this, [=](){
                            qDebug() << QString("Import data triggered: 1");
                        });

                        treeMenu->addActions({ importAction, selAllAction, extractAction });
                    }

                    if (ui->treeWidget->currentItem()->text(0) == "Skeleton"
                        || ui->treeWidget->currentItem()->text(0) == "Animations")
                    {
                        QAction* importAction = new QAction("Import");
                        QAction* selAllAction = new QAction("Select All");
                        QAction* extractAction = new QAction("Extract");
                        //QAction* deselAllAction = new QAction("Deselect All");

                        QObject::connect(importAction, &QAction::triggered, this, [=](){
                            qDebug() << QString("Import data triggered: 1");
                        });

                        treeMenu->addActions({ importAction, selAllAction, extractAction });
                    }

            */

            if (ui->treeWidget->currentItem()->text(0) == "Manufacturer Colors") {

                QAction* extractAction = new QAction("Extract");
                QAction* exportAction = new QAction("Export to JSON");

                QObject::connect(extractAction, &QAction::triggered, this, [&]() {
                    QString dir = QFileDialog::getSaveFileName(this, tr("Extract data"),
                        QString("/home/user/ManufacturerColors.bin"),
                        tr("ForzaTech Bundle (*.bin)"));
                    if (!dir.isEmpty()) {
                        std::vector<char> blob;
                        if (m_container.findName("ManufacturerColors.bin", blob))
                        {
                            QFile file(dir);
                            if (file.open(QIODevice::WriteOnly))
                            {
                                file.write(blob.data(), blob.size());
                            }

                            file.close();
                        }
                    }
                    });

                QObject::connect(exportAction, &QAction::triggered, this, [&]() {
                    QString dir = QFileDialog::getSaveFileName(this, tr("Export Colors Data"),
                        QString("/home/user/ManufacturerColors.json"),
                        tr("Colors Data JSON (*.json)"));
                    if (!dir.isEmpty()) {
                        exportManufacturerColors(dir);
                    }
                    });


                treeMenu->addActions({ extractAction, exportAction });
            }

            if (ui->treeWidget->currentItem()->text(1) == "Color")
            {
                QAction* extractAction = new QAction("Extract");
                QAction* copyPathAction = new QAction("Copy Path");

                QObject::connect(extractAction, &QAction::triggered, this, [&]() {

                    const auto& items = ui->treeWidget->selectedItems();

                    QString dir = QFileDialog::getExistingDirectory(this, tr("Select a Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
                    for (auto& item : items)
                    {
                        std::string original_path = item->text(2).toStdString();
                        std::string path = m_game->Remove(original_path).string();
                        std::replace(path.begin(), path.end(), '\\', '/');

                        std::vector<char> blob = FindAssetInContainer(path);
                        if (!blob.empty())
                        {
                            QFile file(QString("%0/%1").arg(dir, QString::fromStdString(std::filesystem::path(path).filename().string())));
                            if (file.open(QIODevice::WriteOnly))
                            {
                                file.write(blob.data(), blob.size());
                            }

                            file.close();
                        }
                    }

                    });


                QObject::connect(copyPathAction, &QAction::triggered, this, [=]() {
                    QClipboard* clipboard = QApplication::clipboard();
                    clipboard->setText(ui->treeWidget->currentItem()->text(2));
                    });


                treeMenu->addActions({ extractAction, copyPathAction });
            }

            if (ui->treeWidget->currentItem()->text(0) == "Skeleton")
            {
                QAction* extractAction = new QAction("Extract");
                QAction* importAction = new QAction("Import");

                QObject::connect(extractAction, &QAction::triggered, this, [&]() {
                    QString skeleton_name = QString("%0_skeleton.gr2").arg(QString::fromStdString(m_scene->media_name).toUpper());
                    QString skeleton_location = QString("scene/%0").arg(skeleton_name);

                    QString dir = QFileDialog::getSaveFileName(this, tr("Extract data"),
                        QString("/home/user/%0").arg(skeleton_name),
                        tr("Granny Data (*.gr2)"));

                    std::vector<char> blob;
                    if (m_container.findName(skeleton_location.toStdString(), blob)) {

                        if (!dir.isEmpty())
                        {
                            QFile file(dir);
                            if (file.open(QIODevice::WriteOnly))
                            {
                                file.write(blob.data(), blob.size());
                            }

                            file.close();
                        }
                    }
                    });

                QObject::connect(importAction, &QAction::triggered, this, [&]() {
                    if (GrannyVersionsMatch && m_CharacterFile == nullptr) {
                        QString skeleton_location = QString("scene/%0").arg(QString("%0_skeleton.gr2").arg(QString::fromStdString(m_scene->media_name).toUpper()));

                        std::vector<char> gr2_buffer;
                        if (m_container.findName(skeleton_location.toStdString(), gr2_buffer)) {
                            SetSkeleton(gr2_buffer);
                        }
                    }
                    else {
                        GetSkeleton();
                    }
                    });

                treeMenu->addActions({ extractAction, importAction });
            }

            if (ui->treeWidget->currentItem()->text(0) == "Animations")
            {
                QAction* extractAction = new QAction("Extract All");

                QObject::connect(extractAction, &QAction::triggered, this, [&]() {

                    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

                    if (!dir.isEmpty())
                    {
                        for (int idx = 0; idx < ui->treeWidget->currentItem()->childCount(); ++idx) {
                            auto item = ui->treeWidget->currentItem()->child(idx);

                            std::string original_path = item->text(2).toStdString();
                            std::string path = m_game->Remove(original_path, m_scene->media_name).string();
                            std::replace(path.begin(), path.end(), '\\', '/');

                            std::vector<char> blob;
                            if (m_container.findName(path, blob))
                            {
                                QFile file(QString("%0/%1").arg(dir, QString::fromStdString(std::filesystem::path(path).filename().string())));
                                if (file.open(QIODevice::WriteOnly))
                                {
                                    file.write(blob.data(), blob.size());
                                }

                                file.close();
                            }
                        }
                    }

                    });

                treeMenu->addAction(extractAction);
            }

            if (ui->treeWidget->currentItem()->text(1) == "Motion")
            {
                QAction* extractAction = new QAction("Extract");
                QAction* importAction = new QAction("Import");

                const auto& item = ui->treeWidget->currentItem();

                QObject::connect(extractAction, &QAction::triggered, this, [&]() {
                    QString motion_location = QString("animations/%0.gr2").arg(item->data(0, Qt::DisplayRole).toString());
                    QString dir = QFileDialog::getSaveFileName(this, tr("Extract motion data"),
                        QString("/home/user/%0.gr2").arg(item->data(0, Qt::DisplayRole).toString()),
                        tr("Granny Data (*.gr2)"));

                    if (!dir.isEmpty())
                    {
                        std::vector<char> blob;
                        if (m_container.findName(motion_location.toStdString(), blob)) {

                            QFile file(dir);
                            if (file.open(QIODevice::WriteOnly))
                            {
                                file.write(blob.data(), blob.size());
                            }

                            file.close();
                        }
                    }
                    });

                QObject::connect(importAction, &QAction::triggered, this, [=]() {

                    QVariant path = ui->treeWidget->currentItem()->data(1, Qt::UserRole);

                    //MGlobal::displayError(std::to_string(ind.toInt()).c_str());

                    if (m_CharacterFile == 0)
                    {
                        QMessageBox msgBox(FMQtWindow::mainWindow());
                        msgBox.setWindowTitle("Failure");
                        msgBox.setIcon(QMessageBox::Warning);
                        msgBox.setText("A instance of Granny2 skeleton was not initialized, try to import the model first.");
                        msgBox.exec();
                    }

                    if (m_CharacterFile)
                    {
                        granny_file_info* GrannyModelInfo = GrannyGetFileInfo(m_CharacterFile);

                        if (!DCCManager::skeletonExists(GrannyModelInfo->Skeletons[0])) //!m_joints.empty()
                        {
                            QMessageBox msgBox(FMQtWindow::mainWindow());
                            msgBox.setWindowTitle("Failure");
                            msgBox.setIcon(QMessageBox::Warning);
                            msgBox.setText("A Granny2 skeleton was not found on this scene, try to re-import.");
                            msgBox.exec();
                        }

                        if (DCCManager::skeletonExists(GrannyModelInfo->Skeletons[0])) //!m_joints.empty()
                        {
                            std::string motion = QString(ui->treeWidget->currentItem()->data(1, Qt::UserRole).toString()).toStdString();
                            //MGlobal::displayError(motion.c_str());

                            std::vector<char> buffer;
                            if (m_container.findName(motion, buffer))
                            {
                                granny_file* GrannyFileAnim = GrannyReadEntireFileFromMemory(static_cast<granny_int32x>(buffer.size()), buffer.data());
                                granny_file_info* GrannyAnimInfo = GrannyGetFileInfo(GrannyFileAnim);

                                //DCCManager::handleAnimation(GrannyModelInfo->Skeletons[0], GrannyAnimInfo->Animations[0], m_joints);

                                DCCManager::handleAnimation(GrannyModelInfo->Skeletons[0], GrannyAnimInfo->Animations[0]);
                                GrannyFreeFile(GrannyFileAnim);
                            }

                            //granny_file* GrannyFileAnim = GrannyReadEntireFileFromMemory(static_cast<granny_int32x>(buffer.size()), buffer.data());
                            //m_GrannyAnimInfos.emplace_back(GrannyGetFileInfo(GrannyFileAnim));
                            //DCCManager::handleAnimation(m_GrannyModelInfo->Skeletons[0], m_GrannyAnimInfos[ind.toInt()]->Animations[0], m_joints);
                        }

                    }

                });

                treeMenu->addActions({ extractAction, importAction });
            }

            if (ui->treeWidget->currentItem()->text(0) == "Materials")
            {
                QAction* extractAllMtlAction = new QAction("Extract All Materials");
                QAction* extractAllTexAction = new QAction("Extract All Textures");

                QAction* exportJSONAction = new QAction("Export All to JSON");
                QAction* exportDDSAction = new QAction("Export All to DDS");
                QAction* exportPNGAction = new QAction("Export All to PNG");

                exportPNGAction->setEnabled(false);

                QObject::connect(extractAllMtlAction, &QAction::triggered, this, [&]() {
                    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

                    if (!dir.isEmpty())
                    {
                        int index = 0;

                        for (auto& [path, blob] : material_list)
                        {
                            ui->progressBar->setValue(static_cast<int>((index + 1) * 100 / material_list.size()));

                            if (!dir.isEmpty())
                            {
                                QFile file(QString("%0/%1").arg(dir, QString::fromStdString(std::filesystem::path(path).filename().string())));
                                if (file.open(QIODevice::WriteOnly))
                                {
                                    file.write(blob.data(), blob.size());
                                }

                                file.close();
                            }

                            index++;
                        }
                    }

                    ui->progressBar->setValue(0);

                    });

                QObject::connect(extractAllTexAction, &QAction::triggered, this, [&]() {
                    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

                    if (!dir.isEmpty())
                    {
                        int index = 0;

                        for (auto& [path, filename] : texture_list)
                        {
                            auto library_blob = handleLibrary(path);

                            ui->progressBar->setValue(static_cast<int>((index + 1) * 100 / texture_list.size()));

                            if (!library_blob.empty() && !dir.isEmpty())
                            {
                                QFile file(QString("%0/%1").arg(dir, QString::fromStdString(filename)));
                                if (file.open(QIODevice::WriteOnly))
                                {
                                    file.write(library_blob.data(), library_blob.size());
                                }

                                qDebug() << QString("%0/%1").arg(dir, QString::fromStdString(filename));

                                file.close();
                            }
                            index++;
                        }
                    }

                    ui->progressBar->setValue(0);

                    });

                QObject::connect(exportPNGAction, &QAction::triggered, this, [&]() {
                    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
                    if (!dir.isEmpty())
                    {
                        int index = 0;

                        for (auto& [path, filename] : texture_list)
                        {
                            auto library_blob = handleLibrary(path);

                            ui->progressBar->setValue(static_cast<int>((index + 1) * 100 / texture_list.size()));

                            if (!library_blob.empty() && !dir.isEmpty())
                            {
                                auto texture_bundle = fmnext::BundleReader(library_blob);
                                if (texture_bundle.Init())
                                {
                                    std::string path_name = QString("%0/%1").arg(dir, QString::fromStdString(std::filesystem::path(filename).replace_extension(".png").string())).toStdString();
                                    auto texture_resolver = fmnext::TextureResolver(texture_bundle.bundle);

                                    texture_resolver.SaveToPNGFileNew(path_name);

                                    /*
                                    auto texture_resolver = fmnext::TextureResolver(texture_bundle.bundle);
                                    const DirectX::Blob& blob_png = texture_resolver.SaveToPNGMemory();

                                    QFile file(QString("%0/%1").arg(dir, QString::fromStdString(std::filesystem::path(filename).replace_extension(".png").string())));
                                    if (file.open(QIODevice::WriteOnly))
                                    {
                                        file.write(static_cast<char*>(blob_png.GetBufferPointer()), static_cast<uint32_t>(blob_png.GetBufferSize()));
                                    }

                                    file.close();
                                    */
                                }

                            }
                            index++;
                        }
                    }

                    ui->progressBar->setValue(0);

                    });

                QObject::connect(exportDDSAction, &QAction::triggered, this, [=]() {
                    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

                    if (!dir.isEmpty())
                    {
                        int index = 0;

                        for (auto& [path, filename] : texture_list)
                        {
                            auto library_blob = handleLibrary(path);

                            ui->progressBar->setValue(static_cast<int>((index + 1) * 100 / texture_list.size()));

                            if (!library_blob.empty() && !dir.isEmpty())
                            {
                                auto texture_bundle = fmnext::BundleReader(library_blob);
                                if (texture_bundle.Init())
                                {
                                    std::string path_name = QString("%0/%1").arg(dir, QString::fromStdString(std::filesystem::path(filename).replace_extension(".dds").string())).toStdString();
                                    auto texture_resolver = fmnext::TextureResolver(texture_bundle.bundle);
                                    texture_resolver.SaveToDDSFile(path_name);

                                    /*
                                    auto texture_resolver = fmnext::TextureResolver(texture_bundle.bundle);
                                    const DirectX::Blob& blob_png = texture_resolver.SaveToPNGMemory();

                                    QFile file(QString("%0/%1").arg(dir, QString::fromStdString(std::filesystem::path(filename).replace_extension(".png").string())));
                                    if (file.open(QIODevice::WriteOnly))
                                    {
                                        file.write(static_cast<char*>(blob_png.GetBufferPointer()), static_cast<uint32_t>(blob_png.GetBufferSize()));
                                    }

                                    file.close();
                                    */
                                }

                            }
                            index++;
                        }
                    }

                    ui->progressBar->setValue(0);

                    });

                QObject::connect(exportJSONAction, &QAction::triggered, this, [&]() {
                    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

                    if (!dir.isEmpty())
                    {
                        for (auto it = list_items.begin(); it != list_items.end(); ++it)
                        {
                            int index = std::distance(list_items.begin(), it);

                            ui->progressBar->setValue(static_cast<int>((index + 1) * 100 / list_items.size()));

                            if (!dir.isEmpty())
                            {
                                exportMaterialData(index, QString("%0/%1.json").arg(dir, it->item->data(0, Qt::DisplayRole).toString()));
                            }
                        }
                    }

                    ui->progressBar->setValue(0);
                    });

                if (material_list.empty())
                {
                    QMessageBox msgBox(FMQtWindow::mainWindow());
                    msgBox.setWindowTitle("Warning");
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.setText("Game base and shared content was not found, meaning that you wont have access to any material or texture of the selected asset.");
                    msgBox.exec();
                }
                else
                {
                    treeMenu->addActions({ extractAllMtlAction, extractAllTexAction, exportJSONAction, exportDDSAction, exportPNGAction });
                }
            }

            if (ui->treeWidget->currentItem()->text(1) == "Material")
            {
                QAction* extractAction = new QAction("Extract");

                if (material_list.empty())
                {
                    extractAction->setEnabled(false);
                }

                QObject::connect(extractAction, &QAction::triggered, this, [&]() {

                    const auto& items = ui->treeWidget->selectedItems();

                    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

                    for (auto& item : items)
                    {
                        std::string original_path = item->text(2).toStdString();

                        if (fmnext::GameResolver::Contains(original_path, m_scene->media_name))
                        {
                            std::string path = fmnext::GameResolver::Remove(original_path, m_scene->media_name).string();
                            std::replace(path.begin(), path.end(), '\\', '/');

                            std::vector<char> blob;
                            if (m_container.findName(path, blob))
                            {
                                QFile file(QString("%0/%1").arg(dir, QString::fromStdString(std::filesystem::path(path).filename().string())));
                                if (file.open(QIODevice::WriteOnly))
                                {
                                    file.write(blob.data(), blob.size());
                                }

                                file.close();
                            }
                        }
                        else
                        {
                            std::string path = m_game->Remove(original_path).string();
                            std::replace(path.begin(), path.end(), '\\', '/');

                            std::vector<char> blob = FindAssetInContainer(path);
                            if (!blob.empty())
                            {
                                QFile file(QString("%0/%1").arg(dir, QString::fromStdString(std::filesystem::path(path).filename().string())));
                                if (file.open(QIODevice::WriteOnly))
                                {
                                    file.write(blob.data(), blob.size());
                                }

                                file.close();
                            }
                        }
                    }

                    });


                treeMenu->addActions({ extractAction });
            }

            if (ui->treeWidget->currentItem()->parent() != nullptr && ui->treeWidget->currentItem()->parent()->text(0) == "Materials" && ui->treeWidget->currentItem()->text(1) == "Bundle")
            {
                QAction* exportAction = new QAction("Export to JSON");

                QObject::connect(exportAction, &QAction::triggered, this, [&]() {
                    QString dir = QFileDialog::getSaveFileName(this, tr("Export Material Data"),
                        QString("/home/user/%0.json").arg(ui->treeWidget->currentItem()->text(0)),
                        tr("Material Data JSON (*.json)"));

                    exportMaterialData(ui->treeWidget->currentIndex().row(), dir);
                    });


                treeMenu->addActions({ exportAction });
            }

            if (ui->treeWidget->currentItem()->text(1) == "Image" && m_thumbnail != nullptr)
            {
                QAction* exportPNGAction = new QAction("Save to...");
                QAction* copyPathAction = new QAction("Copy Path");
                QAction* exportDDSAction = new QAction("Export to DDS");

                if (m_thumbnail == nullptr)
                {
                    exportPNGAction->setEnabled(false);
                    exportDDSAction->setEnabled(false);
                    copyPathAction->setEnabled(false);
                }

                QObject::connect(exportPNGAction, &QAction::triggered, this, [=]() {

                    //QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
                    //std::string path = ui->treeWidget->currentItem()->toolTip(2).toStdString();

                    QString dir = QFileDialog::getSaveFileName(this, tr("Save Thumbnail to..."),
                        QString("/home/user/%0").arg(ui->treeWidget->currentItem()->toolTip(2)),
                        tr("Portable Network Graphics (*.PNG)"));

                    if (!dir.isEmpty())
                    {
                        //std::string path_name = QString("%0/%1").arg(dir, QString::fromStdString(std::filesystem::path(path).filename().string())).toStdString();

                        if (m_thumbnail != nullptr)
                        {
                            auto texture_resolver = fmnext::TextureResolver(*m_thumbnail);
                            texture_resolver.SaveToPNGFile(dir.toStdString());
                        }
                    }

                    });

                QObject::connect(copyPathAction, &QAction::triggered, this, [=]() {
                    QClipboard* clipboard = QApplication::clipboard();
                    clipboard->setText(ui->treeWidget->currentItem()->toolTip(2));
                    });

                QObject::connect(exportDDSAction, &QAction::triggered, this, [=]() {
                    //QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
                    //std::string path = ui->treeWidget->currentItem()->toolTip(2).toStdString();

                    QString dir = QFileDialog::getSaveFileName(this, tr("Export Thumbnail to..."),
                        QString("/home/user/%0").arg(QString(ui->treeWidget->currentItem()->toolTip(2)).replace("png", "dds", Qt::CaseInsensitive)),
                        tr("DirectDraw Surface (*.DDS)"));

                    if (!dir.isEmpty())
                    {
                        //std::string path_name = QString("%0/%1").arg(dir, QString::fromStdString(std::filesystem::path(path).filename().replace_extension(".dds").string())).toStdString();

                        if (m_thumbnail != nullptr)
                        {
                            auto texture_resolver = fmnext::TextureResolver(*m_thumbnail);
                            texture_resolver.SaveToDDSFile(dir.toStdString());
                        }
                    }
                    });

                treeMenu->addActions({ exportPNGAction, copyPathAction, exportDDSAction });
            }

            if (ui->treeWidget->currentItem()->text(1) == "Texture")
            {
                QAction* extractAction = new QAction("Extract");
                QAction* exportDDSAction = new QAction("Export to DDS");
                QAction* copyPathAction = new QAction("Copy Path");
                QAction* exportPNGAction = new QAction("Export to PNG");

                if (texture_list.empty())
                {
                    extractAction->setEnabled(false);
                    exportDDSAction->setEnabled(false);
                    copyPathAction->setEnabled(false);
                    exportPNGAction->setEnabled(false);
                }

                //exportPNGAction->setEnabled(false);

                /*
                QObject::connect(extractAction, &QAction::triggered, this, [=]() {
                    qDebug() << "Requested" << ui->treeWidget->currentItem()->text(2);
                });
                */

                QObject::connect(exportPNGAction, &QAction::triggered, this, [&]() {

                    const auto& items = ui->treeWidget->selectedItems();

                    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

                    for (auto& item : items)
                    {
                        std::string original_path = item->text(2).toStdString();

                        auto library_blob = handleLibrary(original_path);

                        if (!library_blob.empty() && !dir.isEmpty())
                        {
                            auto texture_bundle = fmnext::BundleReader(library_blob);
                            if (texture_bundle.Init())
                            {
                                std::string path_name = QString("%0/%1").arg(dir, QString::fromStdString(std::filesystem::path(original_path).filename().replace_extension(".png").string())).toStdString();
                                auto texture_resolver = fmnext::TextureResolver(texture_bundle.bundle);

                                texture_resolver.SaveToPNGFileNew(path_name);
                            }

                        }
                    }
                    });

                QObject::connect(exportDDSAction, &QAction::triggered, this, [&]() {

                    const auto& items = ui->treeWidget->selectedItems();

                    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

                    for (auto& item : items)
                    {
                        std::string original_path = item->text(2).toStdString();

                        auto library_blob = handleLibrary(original_path);

                        if (!library_blob.empty() && !dir.isEmpty())
                        {
                            auto texture_bundle = fmnext::BundleReader(library_blob);
                            if (texture_bundle.Init())
                            {
                                std::string path_name = QString("%0/%1").arg(dir, QString::fromStdString(std::filesystem::path(original_path).filename().replace_extension(".dds").string())).toStdString();
                                auto texture_resolver = fmnext::TextureResolver(texture_bundle.bundle);
                                texture_resolver.SaveToDDSFile(path_name);
                            }

                        }
                    }
                    });

                QObject::connect(extractAction, &QAction::triggered, this, [&]() {

                    const auto& items = ui->treeWidget->selectedItems();

                    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

                    for (auto& item : items)
                    {
                        std::string original_path = item->text(2).toStdString();
                        std::string path = m_game->Remove(original_path).string();

                        auto library_blob = handleLibrary(original_path);
                        if (!library_blob.empty())
                        {
                            QFile file(QString("%0/%1").arg(dir, QString::fromStdString(std::filesystem::path(path).filename().string())));
                            if (file.open(QIODevice::WriteOnly))
                            {
                                file.write(library_blob.data(), library_blob.size());
                            }

                            qDebug() << QString("%0/%1").arg(dir, QString::fromStdString(std::filesystem::path(path).filename().string()));

                            file.close();
                        }
                    }

                    });


                QObject::connect(copyPathAction, &QAction::triggered, this, [=]() {
                    QClipboard* clipboard = QApplication::clipboard();
                    clipboard->setText(ui->treeWidget->currentItem()->text(2));
                    });


                treeMenu->addActions({ extractAction, copyPathAction, exportDDSAction, exportPNGAction });
            }

            if (ui->treeWidget->currentItem()->text(1) == "String")
            {
                QAction* copyAction = new QAction("Copy");

                QObject::connect(copyAction, &QAction::triggered, this, [=]() {
                    QClipboard* clipboard = QApplication::clipboard();
                    clipboard->setText(ui->treeWidget->currentItem()->text(2));
                    });

                treeMenu->addAction(copyAction);
            }

            if (ui->treeWidget->currentItem()->text(0) == "Plugin")
            {
                QAction* aboutAction = new QAction("About");
                treeMenu->addAction(aboutAction);

                if (about_win == nullptr)
                {
                    about_win = std::make_unique<FMAboutWindow>(this);
                    about_win->resize(QSize(FMQtWindow::dpiScale(400), FMQtWindow::dpiScale(360)));
                    about_win->setMaximumSize(QSize(FMQtWindow::dpiScale(400), FMQtWindow::dpiScale(360)));
                    about_win->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
                    about_win->setSuperiorText("Forzatech Importer for Autodesk " + QString(FT_MAX_SCM_BRANCH) + " v" + QString(FT_MAX_VERSION_STR) +
                        "\n"
                        "Maintained by Apex (apex.wexfy.com)");
                }

                QObject::connect(aboutAction, &QAction::triggered, about_win.get(), &FMAboutWindow::show);
            }

            if (!treeMenu->isEmpty())
            {
                treeMenu->exec(QCursor::pos());
            }


        }

        });

    ui->lineEdit->findChild<QToolButton*>()->setIcon(QPixmap(":/icons/svg/close.svg"));

    QObject::connect(ui->pushButton_import, &QAbstractButton::clicked, this, [&]() {
        //if(current_lod->currentIndex() == -1)
        //qDebug() << "Select a level of detail first.";

        if (m_scene == nullptr)
        {
            QMessageBox msgBox(FMQtWindow::mainWindow());
            msgBox.setWindowTitle("Failure");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("No car scene was found, it may lead to a crash. \nThis plugin only supports untouched-unmodified files!");
            msgBox.exec();

            return;
        }

        ui->pushButton_import->setDisabled(true);
        ui->pushButton_reset->setDisabled(true);
        /*
        for (auto& data : list_items)
        {
            if (data.item->checkState(0) == Qt::Checked)
            {
                qDebug() << data.item->text(0) << "is checked.";

            }
        }
        */

        if (GrannyVersionsMatch && m_CharacterFile == nullptr) {
            QString skeleton_location = QString("scene/%0").arg(QString("%0_skeleton.gr2").arg(QString::fromStdString(m_scene->media_name).toUpper()));

            std::vector<char> gr2_buffer;
            if (m_container.findName(skeleton_location.toStdString(), gr2_buffer)) {
                SetSkeleton(gr2_buffer);
            }
        }
        else {
            GetSkeleton();
        }

        if (has_db)
        {
            Initialize(m_records);
        }
        else
        {
            Initialize(nullptr);
        }

        ui->pushButton_import->setDisabled(false);
        ui->pushButton_reset->setDisabled(false);
        ui->progressBar->setValue(0);

        });


    QObject::connect(ui->pushButton_reset, &QPushButton::clicked, this, [&]() {
        ui->treeWidget->clearSelection();
        ui->treeWidget->collapseAll();
        ui->lineEdit->clear();
        current_lod->setCurrentIndex(0);
        upgrade_level->setCurrentIndex(0);
        color_override->setCurrentIndex(0);
        geometry_type->setCurrentIndex(0);
        });

    /*
    QObject::connect(ui->lineEdit, &QLineEdit::textChanged, this, [&](const QString& arg) {

        //Qt::MatchFlags flags{ Qt::MatchContains, Qt::MatchRecursive };
        //QList<QTreeWidgetItem*> list = ui->treeWidget->findItems(arg, flags, 0);

        //qDebug() << "Finding: " << list << "\n";

        for (const auto& data : qAsConst(list_items))
        {
            if (data.item->text(0).contains(arg, Qt::CaseInsensitive)) {
                data.item->setHidden(false);
                data.item->parent()->setHidden(false);

            }
            else {
                data.item->setHidden(true);
                //data.item->parent()->setHidden(true);
            }
        }

        ui->treeWidget->update();
        });
    */

    {
        QMenu* filterMenu = new QMenu(this);
        filterMenu->setWindowFlag(Qt::FramelessWindowHint);
        filterMenu->setWindowFlag(Qt::NoDropShadowWindowHint);
        filterMenu->setAttribute(Qt::WA_TranslucentBackground);

        filterObjectAct = new QAction("Objects");
        filterMaterialAct = new QAction("Materials");
        filterTextureAct = new QAction("Textures");

        //Default
        filterObjectAct->setChecked(true);

        filterObjectAct->setCheckable(true);
        filterMaterialAct->setCheckable(true);
        filterTextureAct->setCheckable(true);

        QObject::connect(filterObjectAct, &QAction::triggered, this, [=](bool checked) {
            filterMaterialAct->setChecked(false);
            filterTextureAct->setChecked(false);
            });

        QObject::connect(filterMaterialAct, &QAction::triggered, this, [=](bool checked) {
            filterObjectAct->setChecked(false);
            filterTextureAct->setChecked(false);
            });

        QObject::connect(filterTextureAct, &QAction::triggered, this, [=](bool checked) {
            filterObjectAct->setChecked(false);
            filterMaterialAct->setChecked(false);
            });

        filterMenu->addActions({ filterObjectAct, filterMaterialAct, filterTextureAct });
        ui->filterButton->setIcon(QPixmap(":/icons/svg/filter_list_24dp_FFFFFF.svg"));
        ui->filterButton->setIconSize(QSize(FMQtWindow::dpiScale(25), FMQtWindow::dpiScale(25)));

        QObject::connect(ui->filterButton, &QPushButton::clicked, this, [=]() {
            filterMenu->exec(QCursor::pos());
            });
    }

    QObject::connect(ui->lineEdit, &QLineEdit::textChanged, this, [&](const QString& arg) {

        //Qt::MatchFlags flags{ Qt::MatchContains, Qt::MatchRecursive };
        //QList<QTreeWidgetItem*> list = ui->treeWidget->findItems(arg, Qt::MatchContains | Qt::MatchRecursive, 0);

#if QT_VERSION > QT_VERSION_CHECK(6, 6, 0)
        if (filterObjectAct->isChecked()) {

            if (!scene_items[10]->isExpanded()) {
                scene_items[10]->setExpanded(true); // Expand Parts
            }

            if (!scene_items[10]->isSelected()) {
                ui->treeWidget->clearSelection();

                scene_items[10]->setSelected(true);
            }

            for (const auto& data : std::as_const(list_items)) {
                if (!data.item->text(0).contains(arg, Qt::CaseInsensitive)) {
                    data.item->setHidden(true);
                    data.item->parent()->setHidden(true);
                    data.item->parent()->parent()->setHidden(true);
                }
            }

            for (const auto& data : std::as_const(list_items)) {
                if (data.item->text(0).contains(arg, Qt::CaseInsensitive)) {
                    data.item->setHidden(false);
                    data.item->parent()->setHidden(false);
                    data.item->parent()->parent()->setHidden(false);

                    data.item->parent()->setExpanded(true);
                    data.item->parent()->parent()->setExpanded(true);
                }
            }

            if (arg.isEmpty()) {

                scene_items[10]->setExpanded(false);

                for (const auto& data : std::as_const(list_items)) {
                    data.item->parent()->setExpanded(false);
                    data.item->parent()->parent()->setExpanded(false);
                }

                ui->treeWidget->clearSelection();
            }
        }
#else
        if (filterObjectAct->isChecked()) {

            if (!scene_items[10]->isExpanded()) {
                scene_items[10]->setExpanded(true); // Expand Parts
            }

            if (!scene_items[10]->isSelected()) {
                ui->treeWidget->clearSelection();

                scene_items[10]->setSelected(true);
            }

            for (const auto& data : qAsConst(list_items)) {
                if (!data.item->text(0).contains(arg, Qt::CaseInsensitive)) {
                    data.item->setHidden(true);
                    data.item->parent()->setHidden(true);
                    data.item->parent()->parent()->setHidden(true);
                }
            }

            for (const auto& data : qAsConst(list_items)) {
                if (data.item->text(0).contains(arg, Qt::CaseInsensitive)) {
                    data.item->setHidden(false);
                    data.item->parent()->setHidden(false);
                    data.item->parent()->parent()->setHidden(false);

                    data.item->parent()->setExpanded(true);
                    data.item->parent()->parent()->setExpanded(true);
                }
            }

            if (arg.isEmpty()) {

                scene_items[10]->setExpanded(false);

                for (const auto& data : qAsConst(list_items)) {
                    data.item->parent()->setExpanded(false);
                    data.item->parent()->parent()->setExpanded(false);
                }

                ui->treeWidget->clearSelection();
            }

        }
#endif

        if (filterMaterialAct->isChecked() || filterTextureAct->isChecked()) {

            if (root_items[1]->isExpanded()) {
                root_items[1]->setExpanded(false); // Hide Scene
            }

            if (!root_items[5]->isExpanded()) {
                root_items[5]->setExpanded(true); // Expand Materials
            }

            if (!root_items[5]->isSelected()) {
                ui->treeWidget->clearSelection();

                root_items[5]->setSelected(true);
            }
        }


#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        if (filterMaterialAct->isChecked()) {
            for (const auto& data : std::as_const(material_entries)) {
                if (!data->text(0).contains(arg, Qt::CaseInsensitive)) {
                    data->setHidden(true);
                    data->parent()->setHidden(true);
                    //data->parent()->parent()->setHidden(true);
                }
            }

            for (const auto& data : std::as_const(material_entries)) {
                if (data->text(0).contains(arg, Qt::CaseInsensitive)) {
                    data->setHidden(false);
                    data->parent()->setHidden(false);
                    //data->parent()->parent()->setHidden(true);

                    data->parent()->setExpanded(true);
                }
            }

            if (arg.isEmpty()) {

                scene_items[10]->setExpanded(false);

                for (const auto& data : std::as_const(material_entries)) {
                    data->setExpanded(false);
                    data->parent()->setExpanded(false);

                    if (data->childCount() == 0) {
                        data->setHidden(false);
                    }
                }

                if (!root_items[1]->isExpanded()) {
                    root_items[1]->setExpanded(true); // Show Scene
                }

                if (root_items[5]->isExpanded()) {
                    root_items[5]->setExpanded(false); // Hide Materials
                }

                ui->treeWidget->clearSelection();
            }
        }

        if (filterTextureAct->isChecked()) {

            for (const auto& data : std::as_const(material_entries)) {
                if (data->childCount() == 0) {
                    data->setHidden(true);
                    data->parent()->setHidden(true);
                }
            }

            for (const auto& data : std::as_const(texture_entries)) {
                if (!data->text(0).contains(arg, Qt::CaseInsensitive)) {
                    data->setHidden(true);
                    data->parent()->setHidden(true);
                    data->parent()->parent()->setHidden(true);
                }
            }

            for (const auto& data : std::as_const(texture_entries)) {
                if (data->text(0).contains(arg, Qt::CaseInsensitive)) {
                    data->setHidden(false);
                    data->parent()->setHidden(false);
                    data->parent()->parent()->setHidden(false);

                    data->parent()->setExpanded(true);
                    data->parent()->parent()->setExpanded(true);
                }
            }

            if (arg.isEmpty()) {

                scene_items[10]->setExpanded(false);

                for (const auto& data : std::as_const(material_entries)) {
                    data->setExpanded(false);
                    data->parent()->setExpanded(false);

                    if (data->childCount() == 0) {
                        data->setHidden(false);
                    }
                }

                if (!root_items[1]->isExpanded()) {
                    root_items[1]->setExpanded(true); // Show Scene
                }

                if (root_items[5]->isExpanded()) {
                    root_items[5]->setExpanded(false); // Hide Materials
                }

                ui->treeWidget->clearSelection();
            }
        }

#else

        if (filterMaterialAct->isChecked()) {
            for (const auto& data : qAsConst(material_entries)) {
                if (!data->text(0).contains(arg, Qt::CaseInsensitive)) {
                    data->setHidden(true);
                    data->parent()->setHidden(true);
                    //data->parent()->parent()->setHidden(true);
                }
            }

            for (const auto& data : qAsConst(material_entries)) {
                if (data->text(0).contains(arg, Qt::CaseInsensitive)) {
                    data->setHidden(false);
                    data->parent()->setHidden(false);
                    //data->parent()->parent()->setHidden(true);

                    data->parent()->setExpanded(true);
                }
            }

            if (arg.isEmpty()) {

                scene_items[10]->setExpanded(false);

                for (const auto& data : qAsConst(material_entries)) {
                    data->setExpanded(false);
                    data->parent()->setExpanded(false);

                    if (data->childCount() == 0) {
                        data->setHidden(false);
                    }
                }

                if (!root_items[1]->isExpanded()) {
                    root_items[1]->setExpanded(true); // Show Scene
                }

                if (root_items[5]->isExpanded()) {
                    root_items[5]->setExpanded(false); // Hide Materials
                }

                ui->treeWidget->clearSelection();
            }
        }

        if (filterTextureAct->isChecked()) {

            for (const auto& data : qAsConst(material_entries)) {
                if (data->childCount() == 0) {
                    data->setHidden(true);
                    data->parent()->setHidden(true);
                }
            }

            for (const auto& data : qAsConst(texture_entries)) {
                if (!data->text(0).contains(arg, Qt::CaseInsensitive)) {
                    data->setHidden(true);
                    data->parent()->setHidden(true);
                    data->parent()->parent()->setHidden(true);
                }
            }

            for (const auto& data : qAsConst(texture_entries)) {
                if (data->text(0).contains(arg, Qt::CaseInsensitive)) {
                    data->setHidden(false);
                    data->parent()->setHidden(false);
                    data->parent()->parent()->setHidden(false);

                    data->parent()->setExpanded(true);
                    data->parent()->parent()->setExpanded(true);
                }
            }

            if (arg.isEmpty()) {

                scene_items[10]->setExpanded(false);

                for (const auto& data : qAsConst(material_entries)) {
                    data->setExpanded(false);
                    data->parent()->setExpanded(false);

                    if (data->childCount() == 0) {
                        data->setHidden(false);
                    }
                }

                if (!root_items[1]->isExpanded()) {
                    root_items[1]->setExpanded(true); // Show Scene
                }

                if (root_items[5]->isExpanded()) {
                    root_items[5]->setExpanded(false); // Hide Materials
                }

                ui->treeWidget->clearSelection();
            }
        }

#endif

        ui->treeWidget->update();
        });

    // Plugin Version
    {
        root_items[8]->setText(1, "Version");
        root_items[8]->setText(2, QString(FT_MAX_VERSION_STR));
    }

    ui->treeWidget->setAnimated(false);
    ui->treeWidget->setAlternatingRowColors(true);
    ui->treeWidget->setMouseTracking(false);

    FMFileDownloader* m_pVerCtrl = new FMFileDownloader(QUrl("https://forza.quixel.net/api/max"), this);

    QObject::connect(m_pVerCtrl, &FMFileDownloader::downloaded, this, [=]()
        {
            QByteArray byte_data = m_pVerCtrl->downloadedData();

            if (!byte_data.isEmpty())
            {
                QJsonObject json_obj = QJsonDocument::fromJson(byte_data).object();
                QString json_version = json_obj["version"].toString();
                int json_ordinal = json_obj["ordinal"].toInt();

                //if (root_items[8]->text(2).compare(json_version, Qt::CaseInsensitive) && !json_version.isEmpty())
                if (json_ordinal > FT_MAX_VERSION_NUMBER)
                {
                    QLabel* updateVersionLabel = new QLabel(this);
                    updateVersionLabel->setText(QString("%1 - <a href=\"https://forza.quixel.net\" style=\"font-size: 9pt; text-decoration:none; color: #93c5fd;\">Update to %2 is now available.</a>").arg(root_items[8]->text(2), json_version));
                    updateVersionLabel->setFont(QFont("Segoe UI", 9, QFont::Normal));
                    updateVersionLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
                    updateVersionLabel->setOpenExternalLinks(true);
                    updateVersionLabel->setFocusPolicy(Qt::NoFocus);

                    ui->treeWidget->setItemWidget(root_items[8], 2, updateVersionLabel);
                    root_items[8]->setText(2, "");
                    root_items[8]->setIcon(0, QPixmap(":/icons/svg/warning.svg"));

                    //root_items[8]->setText(2, QString("%1 (Update to %2 is now available)").arg(root_items[8]->text(2), json_version));
                }
            }

        });

    getPreferences();
}

FMMainWindow::~FMMainWindow()
{
    delete ui;
}

void FMMainWindow::GetDatabase(const QString& path, const QString& mediaName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (db.open()) {
        QSqlQuery query(QString("SELECT Data_Car.MediaName, "
            "Data_Car.Id, "
            "List_UpgradeCarBody.CarBodyID, "
            "List_UpgradeTireCompound.TireModelName, "
            "Data_Car.FrontTireWidthMM, "
            "Data_Car.FrontTireAspect, "
            "Data_Car.FrontWheelDiameterIN, "
            "Data_Car.RearTireWidthMM, "
            "Data_Car.RearTireAspect, "
            "Data_Car.RearWheelDiameterIN, "
            "Data_Car.Thumbnail, "
            "Data_CarBody.ModelWheelbase, "
            "Data_CarBody.ModelFrontTrackOuter, "
            "Data_CarBody.ModelRearTrackOuter, "
            "Data_CarBody.ModelFrontStockRideHeight, "
            "Data_CarBody.ModelRearStockRideHeight, "
            "Data_CarBody.BottomCenterWheelbasePosx, "
            "Data_CarBody.BottomCenterWheelbasePosy, "
            "Data_CarBody.BottomCenterWheelbasePosZ "
            "FROM Data_Car "
            "INNER JOIN List_UpgradeTireCompound ON List_UpgradeTireCompound.Ordinal = Data_Car.Id "
            "INNER JOIN List_UpgradeCarBody ON List_UpgradeCarBody.Ordinal = Data_Car.Id "
            "INNER JOIN Data_CarBody ON Data_CarBody.Id = List_UpgradeCarBody.CarBodyID "
            "WHERE MediaName LIKE '%0' "
            "AND List_UpgradeTireCompound.IsStock = 1 "
            "ORDER BY List_UpgradeCarBody.CarBodyID").arg(mediaName));

        while (query.first())
        {
            m_records = std::make_shared<fmnext::DataBaseRecords>();

            m_records->MediaName = query.value(0).toString().toStdString();
            m_records->CarId = query.value(1).toInt();
            m_records->CarBodyID = query.value(2).toInt();
            m_records->TireModelName = query.value(3).toString().toStdString();
            m_records->fallback = false;
            m_records->FrontTireWidthMM = query.value(4).toInt();
            m_records->FrontTireAspect = query.value(5).toInt();
            m_records->FrontWheelDiameterIN = query.value(6).toInt();
            m_records->RearTireWidthMM = query.value(7).toInt();
            m_records->RearTireAspect = query.value(8).toInt();
            m_records->RearWheelDiameterIN = query.value(9).toInt();
            m_records->Thumbnail = query.value(10).toString().toStdString();
            m_records->ModelWheelbase = query.value(11).toFloat();
            m_records->ModelFrontTrackOuter = query.value(12).toFloat();
            m_records->ModelRearTrackOuter = query.value(13).toFloat();
            m_records->ModelFrontStockRideHeight = query.value(14).toFloat();
            m_records->ModelRearStockRideHeight = query.value(15).toFloat();
            m_records->BottomCenterWheelbasePosX = query.value(16).toFloat();
            m_records->BottomCenterWheelbasePosY = query.value(17).toFloat();
            m_records->BottomCenterWheelbasePosZ = query.value(18).toFloat();

            has_db = true;

            query.finish();
        }
    }

    db.close();
}

QString FMMainWindow::GetBuildNumber(const std::vector<char>& buffer)
{
    std::string data(buffer.begin(), buffer.end() - 2);
    QString result = QString::fromStdString(data);

    /*
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        result.resize(file.size() - 2);
        result = file.read(result.size());
    }
    */
    return result;
}

//std::unordered_map<std::filesystem::path, granny_file_info*> references;
std::vector<std::filesystem::path> references;

void FMMainWindow::setModel(const std::string& path)
{
    m_container = fmnext::ContainerReader(path);
    m_game = std::make_unique<fmnext::GameResolver>(path);

    std::filesystem::path mpath(path);

    bool can_open = false;

    std::vector<char> buffer{};
    if (m_container.findName(mpath.filename().replace_extension(std::string(".carbin")).string(), buffer)) {
        auto reader = fmnext::SceneReader(buffer, fmnext::Series::Auto);
        if (reader.Init()) {
            can_open = true;
            m_scene = std::make_shared<fmnext::SceneReader::Scene>(reader.scene);
        }
    }

    // Game Series
    {
        QString SeriesName = m_game->GetSeriesName() == std::string("Unknown") ? "Unknown" : QString("Forza %0").arg(m_game->GetSeriesName().c_str());

        root_items[7]->setText(2, SeriesName);
    }

    if (can_open)
    {
        root_items[1]->setText(2, QString::fromStdString(m_scene->media_name).toUpper());

        GetDatabase(m_game->GetDatabase().string().c_str(), QString::fromStdString(m_scene->media_name));

        QString skeleton_path = QString("game:\\media\\cars\\%0\\scene\\%1").arg(QString::fromStdString(m_scene->media_name), QString("%0_skeleton.gr2").arg(QString::fromStdString(m_scene->media_name).toUpper()));
        root_items[0]->setText(2, skeleton_path);
        root_items[0]->setToolTip(2, skeleton_path);

        if (GrannyVersionsMatch)
        {
            printf("Granny Version: %s \n", GrannyGetVersionString());

            int mt_index = 0;

            {
                QString state_machine = QString("animations/%0").arg(QString("%0.gsf").arg(QString::fromStdString(m_scene->media_name).toUpper()));

                std::vector<char> gsf_buffer;
                if (m_container.findName(state_machine.toStdString(), gsf_buffer)) {
                    //qDebug() << ani.c_str() << "Readed";
                }

                initStateMachine(gsf_buffer);

                for (auto& ref : references)
                {
                    std::string msg = "Reference ";
                    msg += ref.string();
                    msg += " \n";

                    //MGlobal::displayInfo(msg.c_str());

                    QTreeWidgetItem* item = new QTreeWidgetItem();
                    item->setIcon(0, QPixmap(":/icons/svg/animated.svg"));
                    item->setText(0, QString::fromStdString(std::filesystem::path(ref.string()).filename().stem().string()));

                    QString motion_path = QString("game:\\media\\cars\\%0\\animations\\%1").arg(QString::fromStdString(m_scene->media_name), QString::fromStdString(std::filesystem::path(ref.string()).filename().string()));

                    item->setText(1, "Motion");
                    item->setText(2, motion_path);
                    item->setToolTip(2, motion_path);
                    item->setData(1, Qt::UserRole, QVariant(QString("Animations/%0").arg(ref.filename().c_str())));

                    root_items[4]->addChild(item);

                    ++mt_index;
                }

            }




        }
        else
        {
            printf("Warning: the Granny DLL currently loaded "
                "doesn't match the .h file used during compilation\n");
        }


        if (!has_db)
        {
            root_items[6]->setText(2, "Unavailable");
            root_items[6]->setIcon(0, QPixmap(":/icons/svg/database-fill-exclamation.svg"));
        }

        if (has_db)
        {
            auto tires_directory = m_game->GetSharedTires(m_records->TireModelName);
            auto tires_container = fmnext::ContainerReader(tires_directory.string());
            auto tires = tires_container.getMediaTireEntries();

            for (auto& tire_bundle_name : tires) {
                std::vector<char> tire_buffer{};
                if (tires_container.findName(tire_bundle_name, tire_buffer)) {
                    auto reader = fmnext::BundleReader(tire_buffer);
                    if (reader.Init()) {
                        m_tires.try_emplace(tire_bundle_name, std::make_shared<fmnext::BundleReader::BundleData>(reader.bundle));
                    }
                }
            }

            tires_container.release();
        }

        // media/_library and media/cars/_library
        {
            materials_container.push_back(fmnext::ContainerReader(m_game->GetPrimaryMaterialsLibrary().string()));
            materials_container.push_back(fmnext::ContainerReader(m_game->GetMediaMaterialsLibrary().string()));
        }

        {
            textures_container.push_back(fmnext::ContainerReader(m_game->GetPrimaryTexturesLibrary().string()));
            textures_container.push_back(fmnext::ContainerReader(m_game->GetMediaTexturesLibrary().string()));
        }


        // media/cars/_library

        auto sec_mat = m_game->GetSecondaryMaterialsLibrary();
        auto sec_tex = m_game->GetSecondaryTexturesLibrary();

        for (auto& mat_lib_entry : sec_mat)
        {
            std::string message;
            message += "Secondary Material Library Found: ";
            message += mat_lib_entry.string();
            message += " \n";

            materials_container.push_back(fmnext::ContainerReader(mat_lib_entry.string()));

            //MGlobal::displayInfo(message.c_str());
        }

        for (auto& tex_lib_entry : sec_tex)
        {
            std::string message;
            message += "Secondary Texture Library Found: ";
            message += tex_lib_entry.string();
            message += " \n";

            textures_container.push_back(fmnext::ContainerReader(tex_lib_entry.string()));

            //MGlobal::displayInfo(message.c_str());
        }


        // expand parts
        root_items[1]->setExpanded(true);

        if (false)
        {
            QPixmap logo1 = QPixmap("C:\\QtProjects\\ForzaEditor\\images\\Thumbnail_1667_Big.png");
            QPixmap logo1_scaled = logo1.scaled(QSize(200, 200), Qt::KeepAspectRatio, Qt::SmoothTransformation);

            scene_items[1]->setText(1, "Image");
            scene_items[1]->setData(2, Qt::DecorationRole, logo1_scaled);
            scene_items[1]->setToolTip(2, "Thumbnail_1667_Big");
        }

        // Build Number
        {
            root_items[3]->setText(1, "String");

            std::vector<char> buffer;
            if (m_container.findName("BuildNumber.txt", buffer)) {
                root_items[3]->setText(2, GetBuildNumber(buffer));
            }

        }

        if (!has_db)
        {
            scene_items[1]->setIcon(0, QPixmap(":/icons/svg/error.svg"));
            scene_items[1]->setText(1, "Image");
            scene_items[1]->setText(2, "Unavailable");
            scene_items[1]->setToolTip(2, "Thumbnail not available.");
        }

        /* DATABASE SET */
        if (has_db)
        {
            if (!m_records->Thumbnail.empty())
            {
                for (const auto& path : m_game->GetResourceContainer(m_records->Thumbnail))
                {
                    for (const auto& name : fmnext::GameResolver::GetThumbnailNames(m_records->Thumbnail))
                    {
                        auto thumbnail_container = fmnext::ContainerReader(path.string());

                        std::vector<char> thumb_blob{};
                        if (thumbnail_container.findName(name, thumb_blob)) {
                            auto thumb = fmnext::BundleReader(thumb_blob);
                            if (thumb.Init() && m_thumbnail == nullptr)
                            {
                                m_thumbnail = std::make_unique<fmnext::BundleReader::BundleData>(thumb.bundle);

                                auto texture_resolver = fmnext::TextureResolver(thumb.bundle);
                                const DirectX::Blob& blob_png = texture_resolver.SaveToPNGMemory();

                                QPixmap default_thumb;
                                default_thumb.loadFromData(static_cast<unsigned char*>(blob_png.GetBufferPointer()), static_cast<uint32_t>(blob_png.GetBufferSize()));

                                QPixmap scaled_thumb = default_thumb.scaled(QSize(FMQtWindow::dpiScale(200), FMQtWindow::dpiScale(200)), Qt::KeepAspectRatio, Qt::SmoothTransformation);

                                scene_items[1]->setText(1, "Image");
                                scene_items[1]->setData(2, Qt::DecorationRole, scaled_thumb);
                                scene_items[1]->setToolTip(2, m_records->Thumbnail.c_str());
                            }
                        }

                        thumbnail_container.release();
                    }
                }
            }

            root_items[6]->setText(2, "Available");
            root_items[6]->setIcon(0, QPixmap(":/icons/svg/database-fill-check.svg"));
        }

        if (!m_thumbnail)
        {
            scene_items[1]->setIcon(0, QPixmap(":/icons/svg/error.svg"));
            scene_items[1]->setText(1, "Image");
            scene_items[1]->setText(2, "Unavailable");
            scene_items[1]->setToolTip(2, "Thumbnail not available.");
        }

        scene_items[0]->setText(1, "UUID");
        scene_items[0]->setText(2, QUuid(m_scene->build_guid).toString(QUuid::WithoutBraces).toUpper());
        scene_items[0]->setToolTip(2, QUuid(m_scene->build_guid).toString(QUuid::WithoutBraces).toUpper());

        scene_items[2]->setText(1, "String");
        scene_items[2]->setText(2, m_scene->media_name.c_str());

        scene_items[3]->setText(1, "Integer");
        scene_items[3]->setText(2, QString("%0").arg(m_scene->ordinal));

        scene_items[4]->setText(1, "Bundle");
        scene_items[4]->setText(2, QString::fromStdString(m_scene->skeleton_modelbin_path));
        scene_items[4]->setToolTip(2, QString::fromStdString(m_scene->skeleton_modelbin_path));

        SetSkeleton(m_scene->skeleton_modelbin_path);

        current_lod = new QComboBox();
        current_lod->setFocusPolicy(Qt::NoFocus);

        //current_lod->addItems({ "LODS", "LOD0", "LOD1", "LOD2", "LOD3", "LOD4", "LOD5" });
        current_lod->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        //current_lod->setMaximumSize(QSize(150, 30));

        if (m_scene->levels_of_detail.LODS)
        {
            current_lod->addItem("LODS");
        }

        if (m_scene->levels_of_detail.LOD0)
        {
            current_lod->addItem("LOD0");
        }

        if (m_scene->levels_of_detail.LOD1)
        {
            current_lod->addItem("LOD1");
        }

        if (m_scene->levels_of_detail.LOD2)
        {
            current_lod->addItem("LOD2");
        }

        if (m_scene->levels_of_detail.LOD3)
        {
            current_lod->addItem("LOD3");
        }

        if (m_scene->levels_of_detail.LOD4)
        {
            current_lod->addItem("LOD4");
        }

        if (m_scene->levels_of_detail.LOD5)
        {
            current_lod->addItem("LOD5");
        }

        //current_lod->setPlaceholderText("Select");
        current_lod->setCurrentIndex(0);
        current_lod->setEditable(false);

        QStyledItemDelegate* itemDelegate = new QStyledItemDelegate();
        current_lod->setItemDelegate(itemDelegate);

        scene_items[5]->setText(1, "Flags");
        scene_items[5]->setSizeHint(2, current_lod->sizeHint());
        ui->treeWidget->setItemWidget(scene_items[5], 2, current_lod);

        QFrame* comboFrame = current_lod->findChild<QFrame*>();
        comboFrame->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        comboFrame->setAttribute(Qt::WA_TranslucentBackground);

        //scene_items[6]->setCheckState(0, Qt::Unchecked);

        //ui->progressBar->setMaximum(m_scene.non_upgradable_parts.size());

        int index = 0;

        for (const auto& upgradable_part : m_scene->upgradable_parts)
        {
            for (const auto& upgrade : upgradable_part.upgrade_models)
            {
                if (upgrade.car_body_id == -1)
                    continue;

                if (auto item = std::find_if(car_bodies.begin(), car_bodies.end(), [&](auto& car_body) { return car_body.first == upgrade.id; }); item != std::end(car_bodies))
                {
                    if (car_bodies[upgrade.car_body_id] != upgrade.parent_is_stock)
                    {
                        printf("Warning: CarBody %i is marked as both stock and non-stock. \n", upgrade.car_body_id);
                    }
                }
                else
                {
                    car_bodies.emplace(upgrade.car_body_id, upgrade.parent_is_stock);
                }
            }
        }

        if (m_records)
        {
            car_upgrades.try_emplace(m_records->CarBodyID, true);
        }

        for (const auto& [id, parent_istock] : car_bodies) {
            if (parent_istock) {
                car_upgrades.try_emplace(id, true);
            }
        }

        for (const auto& upgradable_part : m_scene->upgradable_parts)
        {
            for (const auto& upgrade : upgradable_part.upgrade_models)
            {
                if (auto item = std::find_if(car_upgrades.begin(), car_upgrades.end(), [&](auto& car_upgrade) { return car_upgrade.first == upgrade.id; }); item == std::end(car_upgrades))
                {
                    if (auto result = std::find_if(std::begin(upgradable_part.shared_models), std::end(upgradable_part.shared_models), [&](const auto& data) { for (auto& upgrade_id : data.upgrade_ids) { return upgrade_id == upgrade.id; } return false;  }); result != std::end(upgradable_part.shared_models))
                    {
                        car_upgrades.emplace(upgrade.id, upgrade.is_stock);
                    }
                }
            }
        }

        {
            upgrade_level = new QComboBox();
            upgrade_level->setFocusPolicy(Qt::NoFocus);
            upgrade_level->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
            //upgrade_level->setMaximumSize(QSize(175, 30));
            upgrade_level->setItemDelegate(new QStyledItemDelegate());

            for (auto [upgrade_id, is_stock] : car_upgrades)
            {
                if (is_stock)
                {
                    upgrade_level->addItem(QString("%0 (Stock)").arg(upgrade_id), upgrade_id);
                    continue;
                }

                upgrade_level->addItem(QString("%0 (Upgrade)").arg(upgrade_id), upgrade_id);
            }


            if (car_upgrades.empty() && m_records)
            {
                upgrade_level->addItem(QString("%0").arg(m_records->CarBodyID), m_records->CarBodyID);
            }

            if (car_upgrades.empty() && !m_records)
            {
                upgrade_level->addItem(QString("%0").arg(m_scene->ordinal), m_scene->ordinal);
            }

            scene_items[6]->setText(1, "Flags");
            scene_items[6]->setSizeHint(2, upgrade_level->sizeHint());
            ui->treeWidget->setItemWidget(scene_items[6], 2, upgrade_level);

            QFrame* comboFrame1 = upgrade_level->findChild<QFrame*>();
            comboFrame1->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
            comboFrame1->setAttribute(Qt::WA_TranslucentBackground);
        }


        {
            QComboBox* render_pass = new QComboBox();
            render_pass->setFocusPolicy(Qt::NoFocus);
            render_pass->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
            render_pass->setItemDelegate(new QStyledItemDelegate());

            render_pass->addItem(QString("Default"));

            scene_items[7]->setText(1, "Flags");
            scene_items[7]->setSizeHint(2, render_pass->sizeHint());
            ui->treeWidget->setItemWidget(scene_items[7], 2, render_pass);

            QFrame* comboFrame2 = render_pass->findChild<QFrame*>();
            comboFrame2->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
            comboFrame2->setAttribute(Qt::WA_TranslucentBackground);
        }

        for (const auto& part : m_scene->upgradable_parts)
        {
            QTreeWidgetItem* root_item = new QTreeWidgetItem();
            root_item->setIcon(0, QPixmap(":/icons/svg/layers-fill.svg"));
            root_item->setText(0, QString::fromStdString(fmnext::SceneReader::PartsToString(part.type)));
            //root_item->setText(1, "Scope");
            //root_item->setCheckState(0, Qt::Unchecked);
            //root_item->setText(2, "-");

            QTreeWidgetItem* shared_parts = new QTreeWidgetItem();
            shared_parts->setIcon(0, QPixmap(":/icons/svg/layers-fill.svg"));
            shared_parts->setText(0, "Shared Parts");
            shared_parts->setText(1, "Scope");
            //shared_parts->setCheckState(0, Qt::Unchecked);


            for (auto& upgrade : part.upgrade_models)
            {
                QTreeWidgetItem* upgrade_holder = new QTreeWidgetItem();
                upgrade_holder->setIcon(0, QPixmap(":/icons/svg/circle.svg"));
                upgrade_holder->setData(0, Qt::DisplayRole, upgrade.id);

                if (upgrade.is_stock)
                {
                    upgrade_holder->setText(1, "Stock");
                }
                else {
                    upgrade_holder->setText(1, QString("Upgrade"));
                }

                root_item->addChild(upgrade_holder);
            }


            for (auto& [upgrade_ids, model] : part.shared_models)
            {
                for (auto& id : upgrade_ids)
                {
                    if (auto upgrade_item = std::find_if(std::begin(part.upgrade_models), std::end(part.upgrade_models), [&](const auto& data) { return data.id == id; }); upgrade_item != std::end(part.upgrade_models))
                    {
                        //if (!item->is_stock)
                        //continue;

                        QTreeWidgetItem* item = new QTreeWidgetItem();
                        item->setIcon(0, QPixmap(":/icons/svg/stack.svg"));
                        item->setText(0, QString::fromStdString(std::filesystem::path(model->path).filename().stem().string()));
                        item->setToolTip(0, QString::fromStdString(model->path));
                        /*
                        if (upgrade_item->is_stock)
                        {
                            item->setText(1, QString("Stock %0").arg(upgrade_item->id));
                        }
                        else
                        {
                            item->setText(1, QString("%0").arg(upgrade_item->id));
                        }
                        */
                        item->setText(1, QString("%0").arg(upgrade_item->id));
                        item->setText(2, QString::fromStdString(model->path));
                        item->setToolTip(2, QString::fromStdString(model->path));
                        //root_item->addChild(item);

                        root_item->child(std::distance(std::begin(part.upgrade_models), upgrade_item))->addChild(item);

                        //auto bundle = SetBundleData(model);
                        //list_items.append({ item, static_cast<uint32_t>(part.type), model, bundle });
                        //SetTextures(model, bundle);

                        QString scheme = QString("%0/%1/%2").arg(std::to_string(id).c_str(), root_item->data(0, Qt::DisplayRole).toString(), model->type.c_str());

                        auto bundle = SetBundleData(model);
                        if (bundle)
                        {
                            auto materials = HandleShaders(model, bundle, scheme);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                            list_items.emplace_back(item, static_cast<uint32_t>(part.type), model, bundle, materials, scheme.toStdString());
#else
                            list_items.push_back({ item, static_cast<uint32_t>(part.type), model, bundle, materials, scheme.toStdString() });
#endif
                        }


                    }
                }
            }

            scene_items[10]->addChild(root_item);
        }

        for (const auto& part : m_scene->non_upgradable_parts)
        {
            QTreeWidgetItem* root_item = new QTreeWidgetItem();
            root_item->setIcon(0, QPixmap(":/icons/svg/layers-fill.svg"));
            root_item->setText(0, QString::fromStdString(fmnext::SceneReader::PartsToString(part.type)));

            //root_item->setCheckState(0, Qt::Unchecked);

            QTreeWidgetItem* upgrade_item = new QTreeWidgetItem();

            if (!car_upgrades.empty() && m_records)
            {
                upgrade_item->setText(0, QString("%0").arg(m_records->CarBodyID));
            }

            if (car_upgrades.empty() && m_records)
            {
                upgrade_item->setText(0, QString("%0").arg(m_records->CarBodyID));
            }

            if (car_upgrades.empty() && !m_records)
            {
                upgrade_item->setText(0, QString("%0").arg(m_scene->ordinal));
            }

            upgrade_item->setText(1, "Stock");
            upgrade_item->setIcon(0, QPixmap(":/icons/svg/circle.svg"));

            for (const auto& [id, parent_istock] : car_bodies) {
                if (parent_istock) {
                    upgrade_item->setData(0, Qt::DisplayRole, id);
                }
            }

            for (const auto& model : part.models)
            {
                QTreeWidgetItem* item = new QTreeWidgetItem();
                item->setIcon(0, QPixmap(":/icons/svg/stack.svg"));

                switch (part.type) {
                case fmnext::CCarParts_WheelStyle:
                {
                    root_item->setText(0, "Wheels");
                    item->setText(0, QString::fromStdString(model->bone_name).replace("spindle", "wheel_"));
                    item->setToolTip(2, QString::fromStdString(model->path));
                    break;
                }
                case fmnext::CCarParts_Brakes:
                {
                    item->setText(0, QString::fromStdString(model->bone_name));
                    item->setToolTip(2, QString::fromStdString(model->path));
                    break;
                }
                default:
                {
                    item->setText(0, QString::fromStdString(std::filesystem::path(model->path).filename().stem().string()));
                    item->setToolTip(2, QString::fromStdString(model->path));
                    break;
                }
                }

                //item->setText(1, QString("Bundle"));
                item->setText(1, QString::fromStdString(model->type));
                item->setText(2, QString::fromStdString(model->path));
                item->setToolTip(0, QString::fromStdString(model->path));
                item->setToolTip(2, QString::fromStdString(model->path));
                //item->setCheckState(0, Qt::Unchecked);

                upgrade_item->addChild(item);
                root_item->addChild(upgrade_item);

                //root_item->addChild(item);

                //auto bundle = SetBundleData(model);
                //list_items.append({ item, static_cast<uint32_t>(part.type), model, bundle });
                //SetTextures(model, bundle);

                QString scheme = QString("%0/%1/%2").arg(upgrade_item->data(0, Qt::DisplayRole).toString(), root_item->data(0, Qt::DisplayRole).toString(), model->type.c_str());

                auto bundle = SetBundleData(model);
                if (bundle)
                {
                    auto materials = HandleShaders(model, bundle, scheme);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    list_items.emplace_back(item, static_cast<uint32_t>(part.type), model, bundle, materials, scheme.toStdString());
#else
                    list_items.push_back({ item, static_cast<uint32_t>(part.type), model, bundle, materials, scheme.toStdString() });
#endif
                }

                index++;
            }

            scene_items[10]->addChild(root_item);

            if (part.type == fmnext::CCarParts_WheelStyle && has_db && !m_tires.empty())
            {
                QTreeWidgetItem* root_tires = new QTreeWidgetItem();
                root_tires->setIcon(0, QPixmap(":/icons/svg/layers-fill.svg"));
                root_tires->setText(0, "Tires");

                QTreeWidgetItem* upgrade_tire = new QTreeWidgetItem();

                if (!car_upgrades.empty() && m_records)
                {
                    upgrade_tire->setText(0, QString("%0").arg(m_records->CarBodyID));
                }

                if (car_upgrades.empty() && m_records)
                {
                    upgrade_tire->setText(0, QString("%0").arg(m_records->CarBodyID));
                }

                if (car_upgrades.empty() && !m_records)
                {
                    upgrade_tire->setText(0, QString("%0").arg(m_scene->ordinal));
                }

                upgrade_tire->setText(1, "Stock");
                upgrade_tire->setIcon(0, QPixmap(":/icons/svg/circle.svg"));

                for (const auto& [id, parent_istock] : car_bodies) {
                    if (parent_istock) {
                        upgrade_item->setData(0, Qt::DisplayRole, id);
                    }
                }

                for (const auto& model : part.models)
                {
                    QTreeWidgetItem* item = new QTreeWidgetItem();
                    item->setIcon(0, QPixmap(":/icons/svg/stack.svg"));
                    item->setText(0, QString::fromStdString(model->bone_name).replace("spindle", "tire_"));
                    item->setText(1, "Tires");

                    if (has_db && !m_tires.empty())
                    {
                        std::shared_ptr<fmnext::SceneReader::CarRenderModel11> tire_model = std::make_shared<fmnext::SceneReader::CarRenderModel11>();
                        tire_model->bone_id = model->bone_id;
                        tire_model->bone_name = model->bone_name;
                        tire_model->id = model->id;
                        tire_model->levels_of_detail = model->levels_of_detail;
                        tire_model->draw_groups = model->draw_groups;
                        tire_model->transform = model->transform;
                        tire_model->version = model->version;
                        tire_model->type = "Tires";

                        std::string position("tire");
                        position += DCCManager::GetContainerDirection(model->bone_name);

                        //{ "tireL", "tireL", "tireL", "tireL" };     =  previous
                        //{ "tireL", "tireL", "tireR", "tireR" };     =  previous/current
                        //{ "tireLF", "tireLR", "tireRF", "tireRR" }; =  current/fixed

                        for (const auto& [key, data] : m_tires)
                        {
                            std::smatch match_tire{};
                            std::string regex = (m_tires.size() <= 2) ? std::string(position.begin(), position.end() - 1) : position;

                            QString scheme = QString("%0/%1/%2").arg(upgrade_item->data(0, Qt::DisplayRole).toString(), root_item->data(0, Qt::DisplayRole).toString(), tire_model->type.c_str());

                            auto materials = HandleShaders(tire_model, data, scheme);

                            switch (static_cast<uint32_t>(m_tires.size()))
                            {
                            case 0x01:
                            {
                                tire_model->path = "game:\\media\\cars\\_library\\scene\\tires\\";
                                tire_model->path += m_records->TireModelName;
                                tire_model->path += std::string(std::string("\\") + position + std::string(key.begin() + regex.size(), key.end()));

                                if (std::find_if(list_items.begin(), list_items.end(), [&](const auto& pitem) { return pitem.model->path == tire_model->path; }) == std::end(list_items))
                                {
                                    item->setText(2, tire_model->path.c_str());
                                    item->setToolTip(0, tire_model->path.c_str());
                                    item->setToolTip(2, tire_model->path.c_str());

                                    upgrade_tire->addChild(item);

                                    auto materials = HandleShaders(tire_model, data, scheme);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                    list_items.emplace_back(item, 8, tire_model, data, materials, scheme.toStdString());
#else
                                    list_items.push_back({ item, 8, tire_model, data, materials, scheme.toStdString() });
#endif
                                }
                                break;
                            }
                            case 0x02:
                            case 0x04:
                            {
                                if (std::regex_search(key, match_tire, std::regex(regex, std::regex::icase)))
                                {
                                    tire_model->path = "game:\\media\\cars\\_library\\scene\\tires\\";
                                    tire_model->path += m_records->TireModelName;
                                    tire_model->path += std::string(std::string("\\") + position + std::string(key.begin() + regex.size(), key.end()));

                                    if (std::find_if(list_items.begin(), list_items.end(), [&](const auto& pitem) { return pitem.model->path == tire_model->path; }) == std::end(list_items))
                                    {
                                        item->setText(2, tire_model->path.c_str());
                                        item->setToolTip(0, tire_model->path.c_str());
                                        item->setToolTip(2, tire_model->path.c_str());

                                        upgrade_tire->addChild(item);

                                        auto materials = HandleShaders(tire_model, data, scheme);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                        list_items.emplace_back(item, 8, tire_model, data, materials, scheme.toStdString());
#else
                                        list_items.push_back({ item, 8, tire_model, data, materials, scheme.toStdString() });
#endif
                                    }
                                }
                                break;
                            }
                            default:
                                printf("Warning: Unsupported number of tire entries found: 0x%02X \n", static_cast<uint32_t>(m_tires.size()));
                                break;
                            }
                        }
                    }

                }

                root_tires->addChild(upgrade_tire);
                scene_items[10]->addChild(root_tires);
            }
        }

        m_colors = GetBundleData("ManufacturerColors.bin");

        if (m_colors != nullptr)
        {
            int color_indice = 0;

            color_override = new QComboBox();
            color_override->setFocusPolicy(Qt::NoFocus);
            color_override->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
            color_override->setItemDelegate(new QStyledItemDelegate());

            scene_items[8]->setText(1, "Flags");
            scene_items[8]->setSizeHint(2, color_override->sizeHint());
            ui->treeWidget->setItemWidget(scene_items[8], 2, color_override);

            QFrame* comboFrame3 = color_override->findChild<QFrame*>();
            comboFrame3->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
            comboFrame3->setAttribute(Qt::WA_TranslucentBackground);

            for (auto& color : m_colors->ManufacturerColors)
            {
                QTreeWidgetItem* root_item = new QTreeWidgetItem();
                root_item->setText(0, QString("Color Set [%0]").arg(color_indice));
                root_item->setIcon(0, QPixmap(":/icons/svg/layers-half.svg"));
                root_item->setText(1, "Group");

                color_override->addItem(QString("Color Set [%0]").arg(color_indice));

                for (auto& inst : color)
                {
                    QTreeWidgetItem* child_item = new QTreeWidgetItem();
                    child_item->setText(0, QString::fromStdString(std::filesystem::path(inst.path).filename().stem().string()));
                    child_item->setText(1, "Material");
                    child_item->setText(2, QString::fromStdString(inst.path));

                    //QPixmap pixmap(50,50);
                    //pixmap.fill(QColor::fromRgbF(inst.preview_color.x, inst.preview_color.y, inst.preview_color.z, 1));
                    //QIcon redIcon(pixmap);

                    QPixmap pixmap(FMQtWindow::dpiScale(17), FMQtWindow::dpiScale(17));
                    pixmap.fill(Qt::transparent);

                    QPainter painter(&pixmap);
                    painter.setRenderHint(QPainter::Antialiasing);
                    painter.setOpacity(1);
                    painter.setPen(QPen(Qt::transparent, 2));
                    painter.setBrush(QColor::fromRgbF(inst.preview_color.x, inst.preview_color.y, inst.preview_color.z, 1));
                    painter.drawRoundedRect(pixmap.rect(), FMQtWindow::dpiScale(20), FMQtWindow::dpiScale(20));

                    {

                        if (fmnext::GameResolver::Contains(inst.path, m_scene->media_name))
                        {
                            std::string path = fmnext::GameResolver::Remove(inst.path, m_scene->media_name).string();
                            std::replace(path.begin(), path.end(), '\\', '/');

                            std::vector<char> mcolor_blob;
                            if (m_container.findName(path, mcolor_blob))
                            {
                                auto material = GetBundleData(mcolor_blob);
                                //auto material_shader = GetBundleData(material->MaterialInstances[0]);

                                for (auto& param : material->ShaderParameters)
                                {
                                    if (param.type == fmnext::ShaderParameter_Texture2D)
                                    {
                                        std::string texture = std::any_cast<std::string>(param.value);

                                        QTreeWidgetItem* tex_item = new QTreeWidgetItem();
                                        tex_item->setText(0, QString::fromStdString(std::filesystem::path(texture).filename().stem().string()));
                                        tex_item->setIcon(0, QPixmap(":/icons/svg/image.svg"));
                                        tex_item->setText(1, "Texture");
                                        tex_item->setText(2, QString::fromStdString(texture));

                                        child_item->addChild(tex_item);
                                    }

                                }

                            }
                        }
                        else
                        {
                            std::string path = m_game->Remove(inst.path).string();
                            std::replace(path.begin(), path.end(), '\\', '/');

                            std::vector<char> mcolor_blob = FindAssetInContainer(path);
                            if (!mcolor_blob.empty())
                            {
                                auto material = GetBundleData(mcolor_blob);
                                //auto material_shader = GetBundleData(material->MaterialInstances[0]);

                                for (auto& param : material->ShaderParameters)
                                {
                                    if (param.type == fmnext::ShaderParameter_Texture2D)
                                    {
                                        std::string texture = std::any_cast<std::string>(param.value);

                                        QTreeWidgetItem* tex_item = new QTreeWidgetItem();
                                        tex_item->setText(0, QString::fromStdString(std::filesystem::path(texture).filename().stem().string()));
                                        tex_item->setIcon(0, QPixmap(":/icons/svg/image.svg"));
                                        tex_item->setText(1, "Texture");
                                        tex_item->setText(2, QString::fromStdString(texture));

                                        child_item->addChild(tex_item);
                                    }

                                }
                            }
                        }


                    }

                    child_item->setData(0, Qt::DecorationRole, pixmap);
                    root_item->addChild(child_item);
                }

                root_items[2]->addChild(root_item);

                ++color_indice;
            }
        }
        else
        {
            scene_items[8]->setDisabled(true);
            scene_items[8]->setText(1, "Flags");
            scene_items[8]->setIcon(0, QPixmap(":/icons/svg/error.svg"));
            scene_items[8]->setText(2, "Unavailable");

            root_items[2]->setDisabled(true);
        }
    }

}

std::vector<char> FMMainWindow::handleLibrary(const std::string& path)
{
    std::string base_path = path;
    std::replace(base_path.begin(), base_path.end(), '\\', '/');

    std::vector<std::string> libraries{ "cars/_library", "media/_library", std::string("media/cars/" + m_scene->media_name) };
    std::vector<char> blob;
    std::smatch match_result;
    for (auto& library : libraries)
    {
        if (std::regex_search(base_path, match_result, std::regex(library, std::regex_constants::icase)))
            break;
    }

    // lowercase the result
    std::string library = match_result.str();
    std::transform(library.begin(), library.end(), library.begin(), [](char c) { return std::tolower(c); });

    auto media = std::find(libraries.begin(), libraries.end(), library);

    if (media != std::end(libraries))
    {
        int64_t type = std::distance(libraries.begin(), media);

        switch (type)
        {
        case 0:
        case 1:
        {
            std::string path_fix = m_game->Remove(path).string();
            std::replace(path_fix.begin(), path_fix.end(), '\\', '/');

            blob = FindAssetInContainer(path_fix, 1);

            if (!blob.empty())
            {
                return blob;
            }

            break;
        }
        case 2:
        {
            std::string path_fix = m_game->Remove(path, m_scene->media_name).string();
            std::replace(path_fix.begin(), path_fix.end(), '\\', '/');

            if (m_container.findName(path_fix, blob))
            {
                return blob;
            }

            break;
        }
        }

    }

    return blob;
}


QString FMMainWindow::RemoveGameBase(const std::string& path, const std::string& mediaName)
{
    QString result = QString::fromStdString(path);
    QString base = QString("game:\\media\\cars\\%0\\").arg(QString::fromStdString(mediaName));
    result.replace(base, "", Qt::CaseInsensitive);
    result.replace('\\', '/');

    return result;
}

std::shared_ptr<fmnext::BundleReader::BundleData> FMMainWindow::SetBundleData(const std::shared_ptr<fmnext::SceneReader::CarRenderModel11>& model)
{
    std::string path = RemoveGameBase(model->path, m_scene->media_name).toStdString();

    std::vector<char> bundle_buffer;
    if (m_container.findName(path, bundle_buffer))
    {
        auto reader = fmnext::BundleReader(bundle_buffer);

        if (reader.Init())
        {
            return std::make_shared<fmnext::BundleReader::BundleData>(reader.bundle);
        }
    }

    return nullptr;
}

std::shared_ptr<fmnext::BundleReader::BundleData> FMMainWindow::GetBundleData(const std::string& path)
{
    std::vector<char> bundle_buffer;
    if (m_container.findName(path, bundle_buffer))
    {
        auto reader = fmnext::BundleReader(bundle_buffer);

        if (reader.Init())
        {
            return std::make_shared<fmnext::BundleReader::BundleData>(reader.bundle);
        }
    }

    return nullptr;
}


granny_file_info* GrannyBindingCallback(gstate_character_info* BindingInfo, char const* SourceFilename, void* UserData)
{
    //printf("SourceFilename: %s \n", SourceFilename);

    auto result = std::find(references.begin(), references.end(), std::string(SourceFilename));
    if (result == references.end())
    {
        references.push_back(std::string(SourceFilename));
    }

    return nullptr;
}

//ContextUID_state_machine
int FMMainWindow::initStateMachine(const std::vector<char>& state)
{
    if (!GrannyVersionsMatch)
    {
        printf("Warning: the Granny DLL currently loaded "
            "doesn't match the .h file used during compilation\n");
        return EXIT_FAILURE;
    }

    granny_file* CharacterFile = 0;
    gstate_character_info* CharacterInfo = 0;
    granny_file_reader* StateFile = GrannyCreateMemoryFileReader(static_cast<granny_int32x>(state.size()), state.data());

    if (GStateReadCharacterInfoFromReader(StateFile, CharacterFile, CharacterInfo) == false)
    {
        // handle error and bail
        return EXIT_FAILURE;
    }

    for (granny_int32x i = 0; i < GStateGetNumAnimationSets(CharacterInfo); ++i)
    {
        std::string AnimationSetName = GStateGetAnimationSetName(CharacterInfo, i);

        if (GStateBindCharacterFileReferences(CharacterInfo, GrannyBindingCallback, 0) == false)
        {
            // handle error and bail
            //return EXIT_FAILURE;
        }

        printf("ContextUID_state_machine: %s \n", AnimationSetName.c_str());
    }

    GrannyFreeFile(CharacterFile);
    GrannyCloseFileReader(StateFile);

    CharacterInfo = 0;
    CharacterFile = 0;

    return EXIT_SUCCESS;
}

void FMMainWindow::closeEvent(QCloseEvent* event)
{
    event->accept();

    savePreferences();

    m_container.release();

    for (auto& mat_lib : materials_container)
    {
        mat_lib.release();
    }

    for (auto& tex_lib : textures_container)
    {
        tex_lib.release();
    }

    references.clear();

    GrannyFreeFile(m_CharacterFile);

    delete this;
}

std::shared_ptr<fmnext::BundleReader::BundleData> FMMainWindow::GetBundleData(std::vector<char>& data)
{
    auto reader = fmnext::BundleReader(data);

    if (reader.Init())
    {
        return std::make_shared<fmnext::BundleReader::BundleData>(reader.bundle);
    }

    return nullptr;
}

void FMMainWindow::SetTextures(const std::shared_ptr<fmnext::SceneReader::CarRenderModel11>& model, const std::shared_ptr<fmnext::BundleReader::BundleData>& bundle)
{
    QTreeWidgetItem* base_item = new QTreeWidgetItem();

    base_item->setText(0, QString::fromStdString(std::filesystem::path(model->path).filename().stem().string()));

    if (model->type == "Wheels")
    {
        base_item->setText(0, QString::fromStdString(model->bone_name).replace("spindle", "wheel_"));
    }

    base_item->setIcon(0, QPixmap(":/icons/svg/stack.svg"));
    base_item->setText(1, "Bundle");
    base_item->setText(3, QString("%0").arg(10000000));

    for (auto& materialBundle : bundle->MaterialInstanceBundles)
    {
        QTreeWidgetItem* mtl_root = new QTreeWidgetItem();
        mtl_root->setText(0, QString::fromStdString(std::any_cast<std::string>(materialBundle.metadata["Name"])));
        mtl_root->setIcon(0, QPixmap(":/icons/svg/material.svg"));

        auto instances = GetBundleData(materialBundle.data);

        if (instances)
        {
            for (auto& instance : instances->MaterialInstances)
            {
                //QTreeWidgetItem* mtl_item = new QTreeWidgetItem();
                //mtl_item->setText(0, QString::fromStdString(std::any_cast<std::string>(materialBundle.metadata["Name"])));
                //mtl_item->setIcon(0, QPixmap(":/icons/svg/stack.svg"));
                mtl_root->setText(1, "Material");
                mtl_root->setText(2, QString::fromStdString(instance));

                std::string pt = m_game->Remove(instance).string();
                std::replace(pt.begin(), pt.end(), '\\', '/');

                std::vector<char> material_buffer = FindAssetInContainer(pt);
                if (!material_buffer.empty())
                {
                    auto material = GetBundleData(material_buffer);
                    //auto material_shader = GetBundleData(material->MaterialInstances[0]);

                    for (auto& param : material->ShaderParameters)
                    {
                        if (param.type == fmnext::ShaderParameter_Texture2D)
                        {
                            std::string texture = std::any_cast<std::string>(param.value);

                            QTreeWidgetItem* tex_item = new QTreeWidgetItem();
                            tex_item->setText(0, QString::fromStdString(std::filesystem::path(texture).filename().stem().string()));
                            tex_item->setIcon(0, QPixmap(":/icons/svg/image.svg"));
                            tex_item->setText(1, "Texture");
                            tex_item->setText(2, QString::fromStdString(texture));

                            //root_items[5]->addChild(tex_item);

                            //mtl_item->addChild(tex_item);

                            mtl_root->setIcon(0, QPixmap(":/icons/svg/material.svg"));
                            mtl_root->addChild(tex_item);
                        }

                    }

                }
            }

            for (auto& param : instances->ShaderParameters)
            {
                if (param.type == fmnext::ShaderParameter_Texture2D)
                {
                    std::string texture = std::any_cast<std::string>(param.value);

                    QTreeWidgetItem* tex_item = new QTreeWidgetItem();
                    tex_item->setText(0, QString::fromStdString(std::filesystem::path(texture).filename().stem().string()));
                    tex_item->setIcon(0, QPixmap(":/icons/svg/image.svg"));
                    tex_item->setText(1, "Texture");
                    tex_item->setText(2, QString::fromStdString(texture));

                    //root_items[5]->addChild(tex_item);

                    //mtl_item->addChild(tex_item);

                    mtl_root->setIcon(0, QPixmap(":/icons/svg/material.svg"));
                    mtl_root->addChild(tex_item);
                }

            }
        }

        base_item->addChild(mtl_root);
    }


    root_items[5]->addChild(base_item);
}


void FMMainWindow::SetTextures(const std::shared_ptr<fmnext::SceneReader::CarRenderModel11>& model, const std::shared_ptr<fmnext::BundleReader::BundleData>& bundle, std::shared_ptr<fmnext::BundleReader::BundleData>& primary_material, std::shared_ptr<fmnext::BundleReader::BundleData>& secondary_material)
{
    QTreeWidgetItem* base_item = new QTreeWidgetItem();

    base_item->setText(0, QString::fromStdString(std::filesystem::path(model->path).filename().stem().string()));

    if (model->type == "Wheels")
    {
        base_item->setText(0, QString::fromStdString(model->bone_name).replace("spindle", "wheel_"));
    }

    if (model->type == "Tires")
    {
        base_item->setText(0, QString::fromStdString(model->bone_name).replace("spindle", "tire_"));
    }

    base_item->setIcon(0, QPixmap(":/icons/svg/stack.svg"));
    base_item->setText(1, "Bundle");
    base_item->setText(3, QString("%0").arg(10000000));

    for (auto& materialBundle : bundle->MaterialInstanceBundles)
    {
        QTreeWidgetItem* mtl_root = new QTreeWidgetItem();
        mtl_root->setText(0, QString::fromStdString(std::any_cast<std::string>(materialBundle.metadata["Name"])));
        mtl_root->setIcon(0, QPixmap(":/icons/svg/material.svg"));

        primary_material = GetBundleData(materialBundle.data);

        if (primary_material)
        {
            for (auto& instance : primary_material->MaterialInstances)
            {
                //QTreeWidgetItem* mtl_item = new QTreeWidgetItem();
                //mtl_item->setText(0, QString::fromStdString(std::any_cast<std::string>(materialBundle.metadata["Name"])));
                //mtl_item->setIcon(0, QPixmap(":/icons/svg/stack.svg"));
                mtl_root->setText(1, "Material");
                mtl_root->setText(2, QString::fromStdString(instance));

                std::string pt = m_game->Remove(instance).string();
                std::replace(pt.begin(), pt.end(), '\\', '/');

                std::vector<char> material_buffer = FindAssetInContainer(pt);
                if (!material_buffer.empty())
                {
                    secondary_material = GetBundleData(material_buffer);
                    //auto material_shader = GetBundleData(secondary_material->MaterialInstances[0]);

                    for (const auto& param : secondary_material->ShaderParameters)
                    {
                        if (param.type == fmnext::ShaderParameter_Texture2D)
                        {
                            std::string texture = std::any_cast<std::string>(param.value);

                            QTreeWidgetItem* tex_item = new QTreeWidgetItem();
                            tex_item->setText(0, QString::fromStdString(std::filesystem::path(texture).filename().stem().string()));
                            tex_item->setIcon(0, QPixmap(":/icons/svg/image.svg"));
                            tex_item->setText(1, "Texture");
                            tex_item->setText(2, QString::fromStdString(texture));

                            //root_items[5]->addChild(tex_item);

                            //mtl_item->addChild(tex_item);

                            mtl_root->setIcon(0, QPixmap(":/icons/svg/material.svg"));
                            mtl_root->addChild(tex_item);
                        }

                    }

                }
            }

            for (const auto& param : primary_material->ShaderParameters)
            {
                if (param.type == fmnext::ShaderParameter_Texture2D)
                {
                    std::string texture = std::any_cast<std::string>(param.value);

                    QTreeWidgetItem* tex_item = new QTreeWidgetItem();
                    tex_item->setText(0, QString::fromStdString(std::filesystem::path(texture).filename().stem().string()));
                    tex_item->setIcon(0, QPixmap(":/icons/svg/image.svg"));
                    tex_item->setText(1, "Texture");
                    tex_item->setText(2, QString::fromStdString(texture));

                    //root_items[5]->addChild(tex_item);

                    //mtl_item->addChild(tex_item);

                    mtl_root->setIcon(0, QPixmap(":/icons/svg/material.svg"));
                    mtl_root->addChild(tex_item);
                }

            }
        }

        base_item->addChild(mtl_root);
    }


    root_items[5]->addChild(base_item);
}

QString FMMainWindow::GetHexHash(int value)
{
    std::stringstream result;
    result << std::uppercase << std::hex << value;

    return QString(result.str().c_str());
}

std::unordered_map<int32_t, fmnext::MaterialInstace> FMMainWindow::HandleShaders(const std::shared_ptr<fmnext::SceneReader::CarRenderModel11>& model, const std::shared_ptr<fmnext::BundleReader::BundleData>& bundle, const QString& scheme)
{
    std::unordered_map<int32_t, fmnext::MaterialInstace> result;

    QTreeWidgetItem* base_item = new QTreeWidgetItem();

    base_item->setText(0, QString::fromStdString(std::filesystem::path(model->path).filename().stem().string()));

    if (model->type == "Wheels")
    {
        base_item->setText(0, QString::fromStdString(model->bone_name).replace("spindle", "wheel_"));
    }

    if (model->type == "Tires")
    {
        base_item->setText(0, QString::fromStdString(model->bone_name).replace("spindle", "tire_"));
    }

    base_item->setIcon(0, QPixmap(":/icons/svg/stack.svg"));
    base_item->setText(1, "Bundle");
    base_item->setText(2, scheme);

    for (auto& materialBundle : bundle->MaterialInstanceBundles)
    {
        std::shared_ptr<fmnext::BundleReader::BundleData> primary_material = GetBundleData(materialBundle.data);

        QTreeWidgetItem* mtl_root = new QTreeWidgetItem();
        mtl_root->setText(0, QString::fromStdString(std::any_cast<std::string>(materialBundle.metadata["Name"])));
        mtl_root->setIcon(0, QPixmap(":/icons/svg/material.svg"));

        if (primary_material)
        {
            for (auto& param : primary_material->ShaderParameters)
            {
                if (param.type == fmnext::ShaderParameter_Texture2D)
                {
                    std::string texture = std::any_cast<std::string>(param.value);

                    QTreeWidgetItem* tex_item = new QTreeWidgetItem();
                    tex_item->setText(0, QString::fromStdString(std::filesystem::path(texture).filename().stem().string()));
                    tex_item->setIcon(0, QPixmap(":/icons/svg/image.svg"));
                    tex_item->setText(1, "Texture");
                    tex_item->setText(2, QString::fromStdString(texture));
                    tex_item->setToolTip(2, QString::fromStdString(texture));

                    texture_list.try_emplace(texture, std::filesystem::path(texture).filename().string());

                    texture_entries.push_back(tex_item);
                    mtl_root->addChild(tex_item);
                }
            }

            for (auto& instance : primary_material->MaterialInstances)
            {
                if (fmnext::GameResolver::Contains(instance, m_scene->media_name))
                {
                    std::string path = m_game->Remove(instance, m_scene->media_name).string();
                    std::replace(path.begin(), path.end(), '\\', '/');

                    mtl_root->setText(1, "Material");
                    mtl_root->setText(2, QString::fromStdString(instance));
                    mtl_root->setToolTip(2, QString::fromStdString(instance));

                    std::vector<char> material_buffer;
                    if (m_container.findName(path, material_buffer))
                    {
                        auto secondary_material = GetBundleData(material_buffer);

                        material_list.try_emplace(path, material_buffer);

                        result.emplace(std::any_cast<int32_t>(materialBundle.metadata["Id"]), fmnext::MaterialInstace(instance, primary_material, secondary_material));

                        for (const auto& param : secondary_material->ShaderParameters)
                        {
                            if (param.type == fmnext::ShaderParameter_Texture2D)
                            {
                                std::string texture = std::any_cast<std::string>(param.value);

                                QTreeWidgetItem* tex_item = new QTreeWidgetItem();
                                tex_item->setText(0, QString::fromStdString(std::filesystem::path(texture).filename().stem().string()));
                                tex_item->setIcon(0, QPixmap(":/icons/svg/image.svg"));
                                tex_item->setText(1, "Texture");
                                tex_item->setText(2, QString::fromStdString(texture));
                                tex_item->setToolTip(2, QString::fromStdString(texture));

                                texture_list.try_emplace(texture, std::filesystem::path(texture).filename().string());

                                texture_entries.push_back(tex_item);
                                mtl_root->addChild(tex_item);
                            }
                        }
                    }

                }
                else
                {
                    std::string path = m_game->Remove(instance).string();
                    std::replace(path.begin(), path.end(), '\\', '/');

                    mtl_root->setText(1, "Material");
                    mtl_root->setText(2, QString::fromStdString(instance));
                    mtl_root->setToolTip(2, QString::fromStdString(instance));

                    std::vector<char> material_buffer = FindAssetInContainer(path);
                    if (!material_buffer.empty())
                    {
                        auto secondary_material = GetBundleData(material_buffer);

                        material_list.try_emplace(path, material_buffer);

                        result.emplace(std::any_cast<int32_t>(materialBundle.metadata["Id"]), fmnext::MaterialInstace(instance, primary_material, secondary_material));

                        for (const auto& param : secondary_material->ShaderParameters)
                        {
                            if (param.type == fmnext::ShaderParameter_Texture2D)
                            {
                                std::string texture = std::any_cast<std::string>(param.value);

                                QTreeWidgetItem* tex_item = new QTreeWidgetItem();
                                tex_item->setText(0, QString::fromStdString(std::filesystem::path(texture).filename().stem().string()));
                                tex_item->setIcon(0, QPixmap(":/icons/svg/image.svg"));
                                tex_item->setText(1, "Texture");
                                tex_item->setText(2, QString::fromStdString(texture));
                                tex_item->setToolTip(2, QString::fromStdString(texture));

                                texture_list.try_emplace(texture, std::filesystem::path(texture).filename().string());

                                texture_entries.push_back(tex_item);
                                mtl_root->addChild(tex_item);
                            }
                        }

                    }
                }

            }
        }

        material_entries.push_back(mtl_root);
        base_item->addChild(mtl_root);
    }

    /*
    if (auto search_result = std::find_if(bundle_entries.begin(), bundle_entries.end(), [&](auto& item) { return item->text(0) == base_item->text(0); }); search_result == std::end(bundle_entries))
    {
        bundle_entries.append(base_item);
        root_items[5]->addChild(base_item);
    }
    */
    //root_items[5]->addChild(base_item);

    bundle_entries.append(base_item);
    root_items[5]->addChild(base_item);

    return result;
}

void FMMainWindow::SetSkeleton(const std::string& path)
{
    std::string skeleton_modelbin_path = RemoveGameBase(path, m_scene->media_name).toStdString();
    std::vector<char> skel_buffer;
    m_container.findName(skeleton_modelbin_path, skel_buffer);

    m_skel = GetBundleData(skel_buffer);

    QList<QTreeWidgetItem*> joints;

    for (auto& joint : m_skel->Skeleton)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setIcon(0, QPixmap(":/icons/svg/joint.svg"));
        item->setText(0, QString::fromStdString(joint.name));
        item->setText(1, "Joint");
        item->setText(2, "XMMATRIX");

        joints.append(item);
    }

    for (int index = 0; index < m_skel->Skeleton.size(); ++index)
    {
        int child = m_skel->Skeleton[index].child_index;
        int parent = m_skel->Skeleton[index].parent_index;

        if (parent != -1)
        {
            joints[parent]->addChild(joints[index]);
        }

        if (child == -1)
        {
            joints[parent]->addChild(joints[index]);
        }

    }

    scene_items[4]->addChildren(joints);
}

bool FMMainWindow::SetSkeleton(const std::vector<char>& blob)
{
    printf("Granny Version: %s \n", GrannyGetVersionString());

    m_CharacterFile = GrannyReadEntireFileFromMemory(static_cast<granny_int32x>(blob.size()), blob.data());
    granny_file_info* m_GrannyFileInfo = GrannyGetFileInfo(m_CharacterFile);

    if (m_CharacterFile == 0 && m_GrannyFileInfo == 0)
    {
        printf("Could not extract a granny_file_info from the file.\n");
    }
    else
    {
        if (m_GrannyFileInfo->Skeletons != nullptr)
        {
            // Assume there is only a single skeleton on each gr2
            DCCManager::handleSkeleton(m_GrannyFileInfo->Skeletons[0]);
        }

        return true;
    }

    return false;
}

bool FMMainWindow::GetSkeleton()
{
    printf("Granny Version: %s \n", GrannyGetVersionString());

    if (m_CharacterFile != nullptr)
    {
        granny_file_info* m_GrannyFileInfo = GrannyGetFileInfo(m_CharacterFile);

        if (m_CharacterFile == 0 && m_GrannyFileInfo == 0)
        {
            printf("Could not extract a granny_file_info from the file.\n");
        }
        else
        {
            if (m_GrannyFileInfo->Skeletons != nullptr)
            {
                // Assume there is only a single skeleton on each gr2
                DCCManager::handleSkeleton(m_GrannyFileInfo->Skeletons[0]);
            }

            return true;
        }
    }

    return false;
}

void FMMainWindow::savePreferences()
{
    QSettings settings("Apex Software", "ForzaTech");

    settings.setValue("geometry", saveGeometry());

    settings.setValue("column_primary", ui->treeWidget->columnWidth(0));
    settings.setValue("column_secondary", ui->treeWidget->columnWidth(1));

}

void FMMainWindow::getPreferences()
{
    QSettings settings("Apex Software", "ForzaTech");

    if (!settings.contains("geometry"))
    {
        this->resize(QSize(FMQtWindow::dpiScale(550), FMQtWindow::dpiScale(850)));

        printf("Setting up initial size for preferences. \n");
    }

    restoreGeometry(settings.value("geometry", QByteArray()).toByteArray());

    int column_primary = (settings.value("column_primary", QVariant()).toUInt() == 0) ? ui->treeWidget->columnWidth(0) : settings.value("column_primary", QVariant()).toUInt();
    int column_secondary = (settings.value("column_secondary", QVariant()).toUInt() == 0) ? ui->treeWidget->columnWidth(1) : settings.value("column_secondary", QVariant()).toUInt();

    ui->treeWidget->setColumnWidth(0, column_primary);
    ui->treeWidget->setColumnWidth(1, column_secondary);
}

std::vector<char> FMMainWindow::FindAssetInContainer(const std::string& path, int type)
{
    std::vector<char> blob;

    switch (type)
    {
    case 0:
    {
        for (auto& mat_container : materials_container)
        {
            if (mat_container.findName(path, blob))
            {
                return blob;
            }
        }
        break;
    }
    case 1:
    {
        for (auto& tex_container : textures_container)
        {
            if (tex_container.findName(path, blob))
            {
                return blob;
            }
        }
        break;
    }
    }

    return blob;
}

void FMMainWindow::exportMaterialData(int bundle_index, const QString& path)
{
    QJsonDocument json_document{};

    QJsonArray document_entries{};
    QJsonObject root_json_object{};

    QJsonObject metadata_object;
    metadata_object.insert("version", 1);
    metadata_object.insert("type", "MaterialData");
    metadata_object.insert("generator", "ForzaTech Importer for Autodesk 3ds Max");

    root_json_object.insert("metadata", metadata_object);

    for (auto it = list_items[bundle_index].bundle->Meshes.begin(); it != list_items[bundle_index].bundle->Meshes.end(); ++it)
    {
        auto materials = list_items[bundle_index];

        auto result = std::find_if(list_items[bundle_index].materials.begin(), list_items[bundle_index].materials.end(), [&](auto& data) {
            return data.first == it->mesh.material_id.value();
            });

        //int mesh_index = std::distance(list_items[bundle_index].bundle->Meshes.begin(), it);

        auto material_instance = std::find_if(list_items[bundle_index].bundle->MaterialInstanceBundles.begin(), list_items[bundle_index].bundle->MaterialInstanceBundles.end(), [&](auto& data) {
            return std::any_cast<int32_t>(data.metadata["Id"]) == it->mesh.material_id;
            });

        if (result != list_items[bundle_index].materials.end()) {

            auto& [index, bundle_ptr] = *result;

            //qDebug() << "mesh:" << QString("%0/%1_%2").arg(list_items[bundle_index].scheme.c_str(), std::any_cast<std::string>(it->metadata["Name"]).c_str()).arg(mesh_index);

            if (material_instance != list_items[bundle_index].bundle->MaterialInstanceBundles.end()) {
                //qDebug() << "instance:" << std::any_cast<std::string>(material_instance->metadata["Name"]);
            }

            QJsonObject json_object;

            QJsonArray json_matloc_array = getShaderParametersArray(materials.materials.find(index)->second.local);
            QJsonArray json_matins_array = getShaderParametersArray(materials.materials.find(index)->second.instace);

            //json_object.insert("Mesh", QString("%0/%1_%2").arg(list_items[bundle_index].scheme.c_str(), std::any_cast<std::string>(it->metadata["Name"]).c_str()).arg(mesh_index));
            json_object.insert("Mesh", QString(std::any_cast<std::string>(it->metadata["Name"]).c_str()));

            if (material_instance != list_items[bundle_index].bundle->MaterialInstanceBundles.end()) {
                json_object.insert("Name", std::any_cast<std::string>(material_instance->metadata["Name"]).c_str());
            }

            json_object.insert("Material", bundle_ptr.path.c_str());
            json_object.insert("Shader", bundle_ptr.instace->MaterialInstances[0].c_str());
            json_object.insert("Local", json_matloc_array);
            json_object.insert("Instance", json_matins_array);
            json_object.insert("Id", it->mesh.material_id.value());

            document_entries.push_back(json_object);
        }
    }

    root_json_object.insert("Materials", document_entries);
    root_json_object.insert("Bundle", QString(list_items[bundle_index].model->path.c_str()));
    root_json_object.insert("Schema", QString(list_items[bundle_index].schema.c_str()));
    root_json_object.insert("Version", list_items[bundle_index].model->version);
    root_json_object.insert("GUID", QUuid(list_items[bundle_index].model->guid_v13).toString(QUuid::WithoutBraces).toUpper());
    json_document.setObject(root_json_object);

    if (!path.isEmpty()) {
        std::filesystem::path filePath = DCCManager::deduplicatePath(path.toStdString());

        QFile jsonFile = QFile(QString(filePath.string().c_str()));
        if (jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) {

            jsonFile.write(json_document.toJson());
        }

        jsonFile.close();
    }
}

void FMMainWindow::exportManufacturerColors(const QString& path)
{
    QJsonDocument json_document{};

    QJsonArray document_entries{};
    QJsonObject root_json_object{};

    QJsonObject metadata_object;
    metadata_object.insert("version", 1);
    metadata_object.insert("type", "ManufacturerColors");
    metadata_object.insert("generator", "ForzaTech Importer for Autodesk 3ds Max");

    root_json_object.insert("metadata", metadata_object);

    if (m_colors != nullptr) {

        for (auto it = m_colors->ManufacturerColors.begin(); it != m_colors->ManufacturerColors.end(); ++it)
        {
            int index = std::distance(m_colors->ManufacturerColors.begin(), it);

            QJsonObject json_object;
            json_object.insert("Color_Set", index);

            QJsonArray array;

            for (auto colors = it->begin(); colors != it->end(); ++colors)
            {
                int idx = std::distance(it->begin(), colors);

                QJsonObject color_object;

                color_object.insert("Index_Mask", QJsonValue::fromVariant(QVariant(colors->material_index_mask.value())));

                if (colors->masks.has_value())
                {
                    QJsonArray mask_array{};

                    for (const auto& mask : colors->masks.value())
                    {
                        mask_array.push_back(QString(mask.c_str()));
                    }

                    color_object.insert("Masks", mask_array);
                }
                else
                {
                    color_object.insert("Masks", QJsonValue(QJsonValue::Null));
                }

                color_object.insert("Path", QString(colors->path.c_str()));
                color_object.insert("Preview_Color", QJsonArray({ colors->preview_color.x, colors->preview_color.y, colors->preview_color.z }));

                {
                    std::string path = m_game->Remove(colors->path).string();
                    std::replace(path.begin(), path.end(), '\\', '/');

                    std::vector<char> blob = FindAssetInContainer(path, 0);

                    if (!blob.empty())
                    {
                        auto data = GetBundleData(blob);

                        QJsonArray json_mat_array = getShaderParametersArray(data);

                        color_object.insert("Shader_Parameters", json_mat_array);
                    }
                }

                array.push_back(color_object);
            }

            json_object.insert("Data", array);
            document_entries.push_back(json_object);
        }
        root_json_object.insert("Colors", document_entries);
        json_document.setObject(root_json_object);
    }

    if (!path.isEmpty()) {
        QFile jsonFile = QFile(path);
        if (jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) {

            jsonFile.write(json_document.toJson());
        }

        jsonFile.close();
    }
}

QJsonArray FMMainWindow::getShaderParametersArray(std::shared_ptr<fmnext::BundleReader::BundleData> bundle)
{
    QJsonArray array;

    for (auto it = bundle->ShaderParameters.begin(); it != bundle->ShaderParameters.end(); ++it) {
        int itx = std::distance(bundle->ShaderParameters.begin(), it);

        switch (it->type) {
        case fmnext::ShaderParameter_Vector: {
            auto result = std::any_cast<DirectX::XMFLOAT4>(it->value);

            QJsonObject object;
            QJsonArray jsonArray = { result.x, result.y, result.z, result.w };

            object.insert(QString("Id"), itx);
            object.insert(QString("Hash"), GetHexHash(it->hash));
            object.insert(QString("Name"), QString(fmnext::NameHashToString(it->hash)).contains("null", Qt::CaseSensitive) ? QJsonValue(QJsonValue::Null) : QString(fmnext::NameHashToString(it->hash)));
            object.insert(QString("GUID"), QUuid(it->guid).toString(QUuid::WithoutBraces).toUpper());
            object.insert(QString("Type"), QString("Vector"));
            object.insert(QString("Data"), jsonArray);

            array.push_back(object);

            break;
        }
        case fmnext::ShaderParameter_Color: {
            auto result =
                std::any_cast<DirectX::XMFLOAT4>(it->value);

            QJsonObject object;
            QJsonArray jsonArray = { result.x, result.y, result.z, result.w };

            object.insert(QString("Id"), itx);
            object.insert(QString("Hash"), GetHexHash(it->hash));
            object.insert(QString("Name"), QString(fmnext::NameHashToString(it->hash)).contains("null", Qt::CaseSensitive) ? QJsonValue(QJsonValue::Null) : QString(fmnext::NameHashToString(it->hash)));
            object.insert(QString("GUID"), QUuid(it->guid).toString(QUuid::WithoutBraces).toUpper());
            object.insert(QString("Type"), QString("Color"));
            object.insert(QString("Data"), jsonArray);

            array.push_back(object);

            break;
        }
        case fmnext::ShaderParameter_Float: {
            auto result = std::any_cast<float>(it->value);

            QJsonObject object;

            object.insert(QString("Id"), itx);
            object.insert(QString("Hash"), GetHexHash(it->hash));
            object.insert(QString("Name"), QString(fmnext::NameHashToString(it->hash)).contains("null", Qt::CaseSensitive) ? QJsonValue(QJsonValue::Null) : QString(fmnext::NameHashToString(it->hash)));
            object.insert(QString("GUID"), QUuid(it->guid).toString(QUuid::WithoutBraces).toUpper());
            object.insert(QString("Type"), QString("Float"));
            object.insert(QString("Data"), result);

            array.push_back(object);

            break;
        }
        case fmnext::ShaderParameter_Bool: {
            auto result = std::any_cast<bool>(it->value);

            QJsonValue jsonValue = QJsonValue(QJsonValue::Bool);
            jsonValue.toBool(result);

            QJsonObject object;
            object.insert(QString("Id"), itx);
            object.insert(QString("Hash"), GetHexHash(it->hash));
            object.insert(QString("Name"), QString(fmnext::NameHashToString(it->hash)).contains("null", Qt::CaseSensitive) ? QJsonValue(QJsonValue::Null) : QString(fmnext::NameHashToString(it->hash)));
            object.insert(QString("GUID"), QUuid(it->guid).toString(QUuid::WithoutBraces).toUpper());
            object.insert(QString("Type"), QString("Bool"));
            object.insert(QString("Data"), jsonValue);

            array.push_back(object);

            break;
        }
        case fmnext::ShaderParameter_Int:
        {
            auto result = std::any_cast<int32_t>(it->value);

            QJsonObject object;
            object.insert(QString("Id"), itx);
            object.insert(QString("Hash"), GetHexHash(it->hash));
            object.insert(QString("Name"), QString(fmnext::NameHashToString(it->hash)).contains("null", Qt::CaseSensitive) ? QJsonValue(QJsonValue::Null) : QString(fmnext::NameHashToString(it->hash)));
            object.insert(QString("GUID"), QUuid(it->guid).toString(QUuid::WithoutBraces).toUpper());
            object.insert(QString("Type"), QString("Int"));
            object.insert(QString("Data"), result);

            array.push_back(object);

            break;
        }
        case fmnext::ShaderParameter_Swizzle: {
            auto result = std::any_cast<std::array<uint8_t, 16>>(it->value);

            QJsonObject object;
            object.insert(QString("Id"), itx);
            object.insert(QString("Hash"), GetHexHash(it->hash));
            object.insert(QString("Name"), QString(fmnext::NameHashToString(it->hash)).contains("null", Qt::CaseSensitive) ? QJsonValue(QJsonValue::Null) : QString(fmnext::NameHashToString(it->hash)));
            object.insert(QString("GUID"), QUuid(it->guid).toString(QUuid::WithoutBraces).toUpper());
            object.insert(QString("Type"), QString("Swizzle"));
            object.insert(QString("Data"), QString("No suitable data parser defined."));

            array.push_back(object);

            break;
        }
        case fmnext::ShaderParameter_Texture2D: {
            auto result = std::any_cast<std::string>(it->value);

            QJsonObject object;
            object.insert(QString("Id"), itx);
            object.insert(QString("Hash"), GetHexHash(it->hash));
            object.insert(QString("Name"), QString(fmnext::NameHashToString(it->hash)).contains("null", Qt::CaseSensitive) ? QJsonValue(QJsonValue::Null) : QString(fmnext::NameHashToString(it->hash)));
            object.insert(QString("GUID"), QUuid(it->guid).toString(QUuid::WithoutBraces).toUpper());
            object.insert(QString("Type"), QString("Texture2D"));
            object.insert(QString("Data"), QString::fromStdString(result));

            array.push_back(object);

            break;
        }
        case fmnext::ShaderParameter_Vector2:
        {
            auto result = std::any_cast<DirectX::XMFLOAT2>(it->value);

            QJsonArray jsonArray = { result.x, result.y };

            QJsonObject object;
            object.insert(QString("Id"), itx);
            object.insert(QString("Hash"), GetHexHash(it->hash));
            object.insert(QString("Name"), QString(fmnext::NameHashToString(it->hash)).contains("null", Qt::CaseSensitive) ? QJsonValue(QJsonValue::Null) : QString(fmnext::NameHashToString(it->hash)));
            object.insert(QString("GUID"), QUuid(it->guid).toString(QUuid::WithoutBraces).toUpper());
            object.insert(QString("Type"), QString("Vector2"));
            object.insert(QString("Data"), jsonArray);

            array.push_back(object);

            break;
        }
        case fmnext::ShaderParameter_Sampler:
        case fmnext::ShaderParameter_ColorGradient:
        case fmnext::ShaderParameter_FunctionRange:
            break;
        }
    };

    return array;
}

void FMMainWindow::Initialize(std::shared_ptr<fmnext::DataBaseRecords> p_records)
{
    std::unordered_map<std::string, DirectX::XMMATRIX> suspension_transforms;
    std::unordered_map<std::string, INode*> suspensions;

    std::unordered_map<std::string, float> rotor_center_offsets;
    std::unordered_map<std::string, float> spindle_offsets;

    std::unordered_map<std::string, DirectX::XMMATRIX> rotor_transforms;
    std::unordered_map<std::string, INode*> rotors;

    std::unordered_map<std::string, DirectX::XMMATRIX> caliper_transforms;
    std::unordered_map<std::string, INode*> calipers;

    std::unordered_map<std::string, DirectX::XMMATRIX> spindle_transforms;

    auto result = std::find_if(list_items.begin(), list_items.end(), [](auto& data) { return data.item->isSelected(); });

    int count = 0;
    int index = 0;

    if (result == std::end(list_items))
    {
        for (auto& data : list_items)
        {
            int v1 = data.item->parent()->data(0, Qt::DisplayRole).toInt();
            int v2 = upgrade_level->itemData(upgrade_level->currentIndex(), Qt::UserRole).toInt();

            if (v1 == v2 || data.item->parent()->text(0).isEmpty() == true)
            {
                ++count;
            }
        }
    }
    else {

        for (auto& data : list_items)
        {
            if (data.item->isSelected())
            {
                ++count;
            }
        }
    }

    ui->progressBar->setMaximum(100);

    INode* sceneRoot = GetCOREInterface()->GetRootNode();

    if (result == std::end(list_items))
    {
        std::string lod_group_name = current_lod->currentText().toStdString();
        lod_group_name += "_";
        lod_group_name += upgrade_level->currentData(Qt::UserRole).toString().toStdString();

        INode* lodGroupObj = nullptr;

        if (!DCCManager::objExists(lod_group_name))
        {
            lodGroupObj = DCCManager::createLocator();
            lodGroupObj->SetName(fmnext::convertStdStringToWide(lod_group_name).c_str());

            for (auto& data : list_items)
            {
                if (data.item->parent()->data(0, Qt::DisplayRole).toInt() == upgrade_level->itemData(upgrade_level->currentIndex(), Qt::UserRole).toInt() ||
                    data.item->parent()->text(0).isEmpty() == true)
                {
                    int amount = static_cast<int>((index + 1) * 100 / count);

                    ui->progressBar->setValue(amount);

                    if (data.type == 44) // Wheels
                    {
                        fmnext::PartLocation direction = DCCManager::GetPartDirection(data.model->bone_name);

                        if (p_records)
                        {
                            float ModelTrackOuter{};
                            float ModelStockRideHeight{};

                            if (direction.end == fmnext::PartLocation::FRONT)
                            {
                                ModelTrackOuter = p_records->ModelFrontTrackOuter;
                                ModelStockRideHeight = p_records->ModelFrontStockRideHeight;
                            }

                            if (direction.end == fmnext::PartLocation::MID)
                            {
                                ModelTrackOuter = p_records->ModelFrontTrackOuter;
                                ModelStockRideHeight = p_records->ModelFrontStockRideHeight;
                            }

                            if (direction.end == fmnext::PartLocation::REAR)
                            {
                                ModelTrackOuter = p_records->ModelRearTrackOuter;
                                ModelStockRideHeight = p_records->ModelRearStockRideHeight;
                            }

                            int TireWidthMM = -1;
                            int TireAspect = -1;
                            int WheelOriginalDiameterIN = -1;
                            int WheelDiameterIN = -1;

                            switch (direction.end)
                            {
                            case fmnext::PartLocation::FRONT:
                                TireWidthMM = p_records->FrontTireWidthMM;
                                TireAspect = p_records->FrontTireAspect;
                                WheelOriginalDiameterIN = p_records->FrontWheelDiameterIN;
                                WheelDiameterIN = p_records->FrontWheelDiameterIN;
                                break;
                            case fmnext::PartLocation::MID:
                                TireWidthMM = p_records->FrontTireWidthMM;
                                TireAspect = p_records->FrontTireAspect;
                                WheelOriginalDiameterIN = p_records->FrontWheelDiameterIN;
                                WheelDiameterIN = p_records->FrontWheelDiameterIN;
                                break;
                            case fmnext::PartLocation::REAR:
                                TireWidthMM = p_records->RearTireWidthMM;
                                TireAspect = p_records->RearTireAspect;
                                WheelOriginalDiameterIN = p_records->RearWheelDiameterIN;
                                WheelDiameterIN = p_records->RearWheelDiameterIN;
                                break;
                            default:
                                // unreachable
                                break;
                            }

                            float half_wheel_outer_diameter_m = static_cast<float>(((TireAspect * 0.01) * (TireWidthMM * 0.001)) + WheelDiameterIN * 0.0254 / 2);

                            DirectX::XMFLOAT4 translate_v1 = DirectX::XMFLOAT4((ModelTrackOuter / 2), (half_wheel_outer_diameter_m - ModelStockRideHeight), (p_records->ModelWheelbase / 2), 0);
                            // y = or (min + max) / 2

                            if (direction.side == fmnext::PartLocation::LEFT)
                            {
                                translate_v1.x = -translate_v1.x;
                            }

                            if (direction.end == fmnext::PartLocation::REAR)
                            {
                                translate_v1.z = -translate_v1.z;
                            }

                            translate_v1.x += p_records->BottomCenterWheelbasePosX;
                            translate_v1.y += p_records->BottomCenterWheelbasePosY;
                            translate_v1.z -= p_records->BottomCenterWheelbasePosZ;

                            if (direction.end == fmnext::PartLocation::MID && direction.side == fmnext::PartLocation::LEFT)
                            {
                                //translate_v1.z = -translate_v1.z;

                                DirectX::XMVECTOR outScale, outQuat, outTrans;
                                DirectX::XMMatrixDecompose(&outScale, &outQuat, &outTrans, data.model->transform);

                                translate_v1.z = DirectX::XMVectorGetZ(outTrans);
                            }

                            if (direction.end == fmnext::PartLocation::MID && direction.side == fmnext::PartLocation::RIGHT)
                            {
                                //translate_v1.z = -translate_v1.z;

                                DirectX::XMVECTOR outScale, outQuat, outTrans;
                                DirectX::XMMatrixDecompose(&outScale, &outQuat, &outTrans, data.model->transform);

                                translate_v1.z = DirectX::XMVectorGetZ(outTrans);
                            }

                            DirectX::XMMATRIX spidle_transform = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat4(&translate_v1));

                            if (direction.side == fmnext::PartLocation::RIGHT)
                            {
                                DirectX::XMVECTOR v1(spidle_transform.r[0]);
                                DirectX::XMVECTOR v2(spidle_transform.r[2]);

                                spidle_transform.r[0] = DirectX::XMVectorSet(-DirectX::XMVectorGetX(v1), DirectX::XMVectorGetY(v1), DirectX::XMVectorGetZ(v1), DirectX::XMVectorGetW(v1));
                                spidle_transform.r[2] = DirectX::XMVectorSet(DirectX::XMVectorGetX(v2), DirectX::XMVectorGetY(v2), -DirectX::XMVectorGetZ(v2), DirectX::XMVectorGetW(v2));
                            }

                            spindle_transforms.emplace(data.model->bone_name, spidle_transform);

                            {
                                float spindle_offset{};
                                float control_arm_offset = 0.30480003f; // 12 inch(0x3E9C0EC0)

                                std::string boneName = "spindle";

                                for (auto& bone : data.bundle->Skeleton)
                                {
                                    if (bone.name == boneName)
                                    {
                                        spindle_offset = DirectX::XMVectorGetX(bone.transform.r[3]);
                                        break;
                                    }
                                }

                                for (auto& bone : m_skel->Skeleton)
                                {
                                    if (bone.name == "controlArm")
                                    {
                                        control_arm_offset = DirectX::XMVectorGetX(bone.transform.r[3]);
                                        break;
                                    }
                                }

                                if (auto rotor_data = std::find_if(std::begin(list_items), std::end(list_items), [&](auto& mdl) { return mdl.model->bone_name == data.model->bone_name && mdl.model->type == "Brakes"; });
                                    rotor_data != std::end(list_items)) {
                                    for (auto& bone : rotor_data->bundle->Skeleton)
                                    {
                                        if (bone.name == "controlArm")
                                        {
                                            control_arm_offset = DirectX::XMVectorGetX(bone.transform.r[3]);
                                            break;
                                        }
                                    };
                                }

                                spindle_offsets.emplace(data.model->bone_name, spindle_offset);


                                DirectX::XMFLOAT4 translate_v2 = DirectX::XMFLOAT4(spindle_offset, 0.f, 0.f, 1.f);

                                translate_v2.x += control_arm_offset;

                                if (direction.side == fmnext::PartLocation::RIGHT)
                                {
                                    translate_v2.x = -translate_v2.x;
                                }

                                translate_v2.x += DirectX::XMVectorGetX(spidle_transform.r[3]);
                                translate_v2.y += DirectX::XMVectorGetY(spidle_transform.r[3]);
                                translate_v2.z += DirectX::XMVectorGetZ(spidle_transform.r[3]);
                                translate_v2.w = 1.f;

                                std::string suspension_name = "controlArm_" + DCCManager::GetContainerDirection(data.model->bone_name);

                                DirectX::XMMATRIX controlArm_transform = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat4(&translate_v2));

                                suspension_transforms.emplace(suspension_name, controlArm_transform);

                                // wheels
                                auto resolver = fmnext::MeshResolver(data.bundle, current_lod->currentIndex(), static_cast<fmnext::GeometryType>(geometry_type->currentIndex()), m_records, data.type, direction.end);

                                std::string wheel_name = "wheel_" + DCCManager::GetContainerDirection(data.model->bone_name);

                                if (DCCManager::objExists(wheel_name))
                                {
                                    //continue;
                                }

                                INode* locatorObj = nullptr;
                                //MFnDagNode fnDagNode;

                                if (!resolver.GetMeshes().empty())
                                {
                                    locatorObj = DCCManager::createLocator(spidle_transform);

                                    //fnDagNode.setObject(wheelLocatorObj);
                                    locatorObj->SetName(fmnext::convertStdStringToWide(wheel_name).c_str());
                                }

                                for (const auto& mesh : resolver.GetMeshes())
                                {
                                    INode* mesh_obj = nullptr;
                                    Mtl* material_obj = nullptr;

                                    std::string mesh_name(mesh.name);

                                    auto material = std::find_if(data.bundle->MaterialInstanceBundles.begin(), data.bundle->MaterialInstanceBundles.end(), [&](auto& mtl) {
                                        return std::any_cast<int32_t>(mtl.metadata["Id"]) == mesh.material_index;
                                        });

                                    if (material != std::end(data.bundle->MaterialInstanceBundles))
                                    {
                                        mesh_name += "_";
                                        mesh_name += std::any_cast<std::string>(material->metadata["Name"]);
                                    }

                                    if (auto material_it = data.materials.find(mesh.material_index); material_it != std::end(data.materials))
                                    {
                                        const auto& [key, material_data] = *material_it;

                                        material_obj = DCCManager::createMaterialfromMemory(std::any_cast<std::string>(material->metadata["Name"]), material_data.instace);
                                    }

                                    mesh_obj = DCCManager::createMesh(&mesh, mesh_name, material_obj, geometry_type->currentIndex());
                                    DCCManager::setNodeTransformation(mesh_obj, mesh.matrix);

                                    locatorObj->AttachChild(mesh_obj, 0);
                                }


                                lodGroupObj->AttachChild(locatorObj);
                            }
                        }

                        // wheels
                        if (!p_records)
                        {
                            auto resolver = fmnext::MeshResolver(data.bundle, current_lod->currentIndex(), static_cast<fmnext::GeometryType>(geometry_type->currentIndex()), nullptr, data.type, direction.end);

                            std::string wheel_name = "wheel_" + DCCManager::GetContainerDirection(data.model->bone_name);

                            if (DCCManager::objExists(wheel_name))
                            {
                                //continue;
                            }

                            INode* locatorObj = nullptr;

                            if (!resolver.GetMeshes().empty())
                            {
                                locatorObj = DCCManager::createLocator(data.model->transform);

                                locatorObj->SetName(fmnext::convertStdStringToWide(wheel_name).c_str());
                            }

                            for (const auto& mesh : resolver.GetMeshes())
                            {
                                INode* mesh_obj = nullptr;
                                Mtl* material_obj = nullptr;

                                std::string mesh_name(mesh.name);

                                auto material = std::find_if(data.bundle->MaterialInstanceBundles.begin(), data.bundle->MaterialInstanceBundles.end(), [&](auto& mtl) {
                                    return std::any_cast<int32_t>(mtl.metadata["Id"]) == mesh.material_index;
                                    });

                                if (material != std::end(data.bundle->MaterialInstanceBundles))
                                {
                                    mesh_name += "_";
                                    mesh_name += std::any_cast<std::string>(material->metadata["Name"]);
                                }


                                if (auto material_it = data.materials.find(mesh.material_index); material_it != std::end(data.materials))
                                {
                                    const auto& [key, material_data] = *material_it;

                                    material_obj = DCCManager::createMaterialfromMemory(std::any_cast<std::string>(material->metadata["Name"]), material_data.instace);
                                }

                                mesh_obj = DCCManager::createMesh(&mesh, mesh_name, material_obj, geometry_type->currentIndex());
                                DCCManager::setNodeTransformation(mesh_obj, mesh.matrix);

                                locatorObj->AttachChild(mesh_obj, 0);
                            }

                            lodGroupObj->AttachChild(locatorObj);
                        }



                        continue;
                    }

                    if (data.type == 8) // Tires
                    {
                        if ((current_lod->currentIndex() == 0) && data.model->levels_of_detail.LODS || (current_lod->currentIndex() >= 1) && !data.model->levels_of_detail.LODS || (current_lod->currentIndex() >= 0) && data.model->levels_of_detail.LODS)
                        {
                            fmnext::PartLocation direction = DCCManager::GetPartDirection(data.model->bone_name);

                            auto resolver = fmnext::MeshResolver(data.bundle, current_lod->currentIndex(), static_cast<fmnext::GeometryType>(geometry_type->currentIndex()), m_records, 8, direction.end);

                            std::string tire_name = "tire_" + DCCManager::GetContainerDirection(data.model->bone_name);

                            if (DCCManager::objExists(tire_name))
                            {
                                //continue;
                            }

                            INode* locatorObj = nullptr;

                            if (!resolver.GetMeshes().empty())
                            {
                                auto trs = std::find_if(spindle_transforms.begin(), spindle_transforms.end(), [&](auto& d) { return d.first == data.model->bone_name; });

                                locatorObj = DCCManager::createLocator(trs->second);

                                locatorObj->SetName(fmnext::convertStdStringToWide(tire_name).c_str());
                            }

                            for (const auto& mesh : resolver.GetMeshes())
                            {
                                INode* mesh_obj = nullptr;
                                Mtl* material_obj = nullptr;

                                std::string mesh_name(mesh.name);

                                auto material = std::find_if(data.bundle->MaterialInstanceBundles.begin(), data.bundle->MaterialInstanceBundles.end(), [&](auto& mtl) {
                                    return std::any_cast<int32_t>(mtl.metadata["Id"]) == mesh.material_index;
                                    });

                                if (material != std::end(data.bundle->MaterialInstanceBundles))
                                {
                                    mesh_name += "_";
                                    mesh_name += std::any_cast<std::string>(material->metadata["Name"]);
                                }


                                if (auto material_it = data.materials.find(mesh.material_index); material_it != std::end(data.materials))
                                {
                                    const auto& [key, material_data] = *material_it;

                                    material_obj = DCCManager::createMaterialfromMemory(std::any_cast<std::string>(material->metadata["Name"]), material_data.instace);
                                }

                                mesh_obj = DCCManager::createMesh(&mesh, mesh_name, material_obj, geometry_type->currentIndex());
                                DCCManager::setNodeTransformation(mesh_obj, mesh.matrix);

                                locatorObj->AttachChild(mesh_obj, 0);
                            }

                            lodGroupObj->AttachChild(locatorObj);
                        }

                        continue;
                    }

                    if (data.model->type == "ControlArm") // Suspensions
                    {
                        std::string suspension_name = "suspension_" + DCCManager::GetContainerDirection(data.model->bone_name);

                        if (DCCManager::objExists(suspension_name))
                        {
                            //continue;
                        }

                        INode* locatorObj = DCCManager::createLocator(data.model->transform);

                        //MFnDagNode fnDagNode(locatorObj);
                        //fnDagNode.setName(model->bone_name.c_str());
                        //fnDagNode.setName(suspension_name.c_str());

                        locatorObj->SetName(fmnext::convertStdStringToWide(suspension_name).c_str());

                        auto resolver = fmnext::MeshResolver(data.bundle, current_lod->currentIndex(), static_cast<fmnext::GeometryType>(geometry_type->currentIndex()));

                        for (const auto& mesh : resolver.GetMeshes())
                        {
                            INode* mesh_obj = nullptr;
                            Mtl* material_obj = nullptr;

                            std::string mesh_name(mesh.name);

                            auto material = std::find_if(data.bundle->MaterialInstanceBundles.begin(), data.bundle->MaterialInstanceBundles.end(), [&](auto& mtl) {
                                return std::any_cast<int32_t>(mtl.metadata["Id"]) == mesh.material_index;
                                });

                            if (material != std::end(data.bundle->MaterialInstanceBundles))
                            {
                                mesh_name += "_";
                                mesh_name += std::any_cast<std::string>(material->metadata["Name"]);
                            }


                            if (auto material_it = data.materials.find(mesh.material_index); material_it != std::end(data.materials))
                            {
                                const auto& [key, material_data] = *material_it;

                                material_obj = DCCManager::createMaterialfromMemory(std::any_cast<std::string>(material->metadata["Name"]), material_data.instace);
                            }

                            mesh_obj = DCCManager::createMesh(&mesh, mesh_name, material_obj, geometry_type->currentIndex());
                            DCCManager::setNodeTransformation(mesh_obj, mesh.matrix);

                            locatorObj->AttachChild(mesh_obj, 0);
                        }

                        if (true) //has_db
                        {
                            suspensions.emplace(data.model->bone_name, locatorObj);
                        }

                        lodGroupObj->AttachChild(locatorObj);

                        continue;
                    }

                    // caliper
                    if (data.model->bone_name.find("hub") != std::string::npos && data.model->type == "Brakes")
                    {
                        std::string caliper_name = "caliper_" + DCCManager::GetContainerDirection(data.model->bone_name);

                        if (DCCManager::objExists(caliper_name))
                        {
                            //continue;
                        }

                        INode* locatorObj = DCCManager::createLocator(data.model->transform);

                        //MFnDagNode fnDagNode(locatorObj);
                        //fnDagNode.setName(model->bone_name.c_str());
                        //fnDagNode.setName(caliper_name.c_str());

                        locatorObj->SetName(fmnext::convertStdStringToWide(caliper_name).c_str());

                        auto resolver = fmnext::MeshResolver(data.bundle, current_lod->currentIndex(), static_cast<fmnext::GeometryType>(geometry_type->currentIndex()));

                        for (const auto& mesh : resolver.GetMeshes())
                        {
                            INode* mesh_obj = nullptr;
                            Mtl* material_obj = nullptr;

                            std::string mesh_name(mesh.name);

                            auto material = std::find_if(data.bundle->MaterialInstanceBundles.begin(), data.bundle->MaterialInstanceBundles.end(), [&](auto& mtl) {
                                return std::any_cast<int32_t>(mtl.metadata["Id"]) == mesh.material_index;
                                });

                            if (material != std::end(data.bundle->MaterialInstanceBundles))
                            {
                                mesh_name += "_";
                                mesh_name += std::any_cast<std::string>(material->metadata["Name"]);
                            }


                            if (auto material_it = data.materials.find(mesh.material_index); material_it != std::end(data.materials))
                            {
                                const auto& [key, material_data] = *material_it;

                                material_obj = DCCManager::createMaterialfromMemory(std::any_cast<std::string>(material->metadata["Name"]), material_data.instace);
                            }

                            mesh_obj = DCCManager::createMesh(&mesh, mesh_name, material_obj, geometry_type->currentIndex());
                            DCCManager::setNodeTransformation(mesh_obj, mesh.matrix);

                            locatorObj->AttachChild(mesh_obj, 0);
                        }

                        if (true) //has_db
                        {
                            calipers.emplace(data.model->bone_name, locatorObj);
                            caliper_transforms.emplace(data.model->bone_name, data.model->transform);

                            //MGlobal::displayInfo(std::string(path + "\n").c_str());
                        }

                        lodGroupObj->AttachChild(locatorObj);

                        continue;
                    }

                    // rotor
                    if (data.model->bone_name.find("spindle") != std::string::npos && data.model->type == "Brakes")
                    {
                        std::string rotor_name = "rotor_" + DCCManager::GetContainerDirection(data.model->bone_name);

                        if (DCCManager::objExists(rotor_name))
                        {
                            //continue;
                        }

                        INode* locatorObj = DCCManager::createLocator(data.model->transform);

                        //MFnDagNode fnDagNode(locatorObj);
                        //fnDagNode.setName(model->bone_name.c_str());
                        //fnDagNode.setName(rotor_name.c_str());

                        locatorObj->SetName(fmnext::convertStdStringToWide(rotor_name).c_str());

                        float control_arm_offset = 0.30480003f; // 12 inch(0x3E9C0EC0)

                        for (auto& bone : data.bundle->Skeleton)
                        {
                            if (bone.name == "controlArm")
                            {
                                control_arm_offset = DirectX::XMVectorGetX(bone.transform.r[3]);
                                break;
                            }
                        }

                        //float rotor_center_offset = 0.f;

                        if (true) //has_db
                        {
                            for (auto& bone : data.bundle->Skeleton)
                            {
                                if (bone.name.find("rotor") != std::string::npos)
                                {
                                    //rotor_center_offset = DirectX::XMVectorGetX(bone.transform.r[3]);
                                    rotor_center_offsets.emplace(data.model->bone_name, DirectX::XMVectorGetX(bone.transform.r[3]));

                                    break;
                                }
                            }
                        }

                        auto resolver = fmnext::MeshResolver(data.bundle, current_lod->currentIndex(), static_cast<fmnext::GeometryType>(geometry_type->currentIndex()));

                        for (const auto& mesh : resolver.GetMeshes())
                        {
                            INode* mesh_obj = nullptr;
                            Mtl* material_obj = nullptr;

                            std::string mesh_name(mesh.name);

                            auto material = std::find_if(data.bundle->MaterialInstanceBundles.begin(), data.bundle->MaterialInstanceBundles.end(), [&](auto& mtl) {
                                return std::any_cast<int32_t>(mtl.metadata["Id"]) == mesh.material_index;
                                });

                            if (material != std::end(data.bundle->MaterialInstanceBundles))
                            {
                                mesh_name += "_";
                                mesh_name += std::any_cast<std::string>(material->metadata["Name"]);
                            }


                            if (auto material_it = data.materials.find(mesh.material_index); material_it != std::end(data.materials))
                            {
                                const auto& [key, material_data] = *material_it;

                                material_obj = DCCManager::createMaterialfromMemory(std::any_cast<std::string>(material->metadata["Name"]), material_data.instace);
                            }

                            mesh_obj = DCCManager::createMesh(&mesh, mesh_name, material_obj, geometry_type->currentIndex());
                            DCCManager::setNodeTransformation(mesh_obj, mesh.matrix);

                            locatorObj->AttachChild(mesh_obj, 0);
                        }

                        if (true) //has_db
                        {
                            rotors.emplace(data.model->bone_name, locatorObj);
                            rotor_transforms.emplace(data.model->bone_name, data.model->transform);

                            //MGlobal::displayInfo(std::string(path + "\n").c_str());
                        }

                        lodGroupObj->AttachChild(locatorObj);

                        continue;
                    }

                    if (true) //DCCManager::objExists(std::filesystem::path(data.model->path).stem().string())
                    {
                        INode* locatorObj = nullptr;

                        auto resolver = fmnext::MeshResolver(data.bundle, current_lod->currentIndex(), static_cast<fmnext::GeometryType>(geometry_type->currentIndex()));

                        if (!resolver.GetMeshes().empty())
                        {
                            locatorObj = DCCManager::createLocator(data.model->transform);

                            std::string bundle_name = std::filesystem::path(data.model->path).stem().string();

                            locatorObj->SetName(fmnext::convertStdStringToWide(bundle_name).c_str());
                        }

                        for (const auto& mesh : resolver.GetMeshes())
                        {
                            INode* mesh_obj = nullptr;
                            Mtl* material_obj = nullptr;

                            std::string mesh_name(mesh.name);
                            std::string material_instance_name{};

                            auto material = std::find_if(data.bundle->MaterialInstanceBundles.begin(), data.bundle->MaterialInstanceBundles.end(), [&](auto& mtl) {
                                return std::any_cast<int32_t>(mtl.metadata["Id"]) == mesh.material_index;
                                });

                            if (material != std::end(data.bundle->MaterialInstanceBundles))
                            {
                                mesh_name += "_";
                                mesh_name += std::any_cast<std::string>(material->metadata["Name"]);
                            }

                            auto material_bundle_reader = fmnext::BundleReader(material->data);

                            if (material_bundle_reader.Init())
                            {
                                for (auto& inst : material_bundle_reader.bundle.MaterialInstances)
                                {
                                    //MString message;
                                    //message.format(MString("Mesh ^1s Material path ^2s"), mesh_name.c_str(), inst.c_str());

                                    material_instance_name = std::filesystem::path(inst).stem().string();

                                    //MGlobal::displayInfo(message);
                                }
                            }

                            if (auto material_it = data.materials.find(mesh.material_index); material_it != std::end(data.materials))
                            {
                                const auto& [key, material_data] = *material_it;

                                std::string material_name = std::any_cast<std::string>(material->metadata["Name"]);

                                bool carpaint_v0 = QString::fromStdString(material_name).contains("carpaint", Qt::CaseInsensitive);
                                bool carpaint_v1 = QString::fromStdString(material_instance_name).contains("carpaint", Qt::CaseInsensitive);
                                bool carpaint_v2 = QString::fromStdString(material_instance_name).contains("carpaint_secondary", Qt::CaseInsensitive);

                                bool glass_clear_v0 = QString::fromStdString(material_instance_name).contains("gls", Qt::CaseInsensitive);
                                bool glass_clear_v1 = QString::fromStdString(material_name).contains("gls", Qt::CaseInsensitive);
                                bool gls_clear_custom = QString::fromStdString(material_name).contains("gls_clear_custom", Qt::CaseInsensitive);
                                bool smooth_glass = QString::fromStdString(material_name).contains("smoothGlass", Qt::CaseInsensitive);

                                if (m_colors && !m_colors->ManufacturerColors.empty())
                                {
                                    if (carpaint_v0 || carpaint_v1 || carpaint_v2)
                                    {
                                        auto carpaint = m_colors->ManufacturerColors[color_override->currentIndex()][0].preview_color;
                                        material_obj = DCCManager::createCarpaintfromMemory(std::any_cast<std::string>(material->metadata["Name"]), carpaint);
                                    }
                                    else if (glass_clear_v0 || glass_clear_v1 || gls_clear_custom || smooth_glass)
                                    {
                                        material_obj = DCCManager::createGlassfromMemory(material_name);
                                    }
                                    else
                                    {
                                        material_obj = DCCManager::createMaterialfromMemory(std::any_cast<std::string>(material->metadata["Name"]), material_data.instace);
                                    }
                                }
                                else
                                {
                                    material_obj = DCCManager::createMaterialfromMemory(std::any_cast<std::string>(material->metadata["Name"]), material_data.instace);
                                }
                            }

                            mesh_obj = DCCManager::createMesh(&mesh, mesh_name, material_obj, geometry_type->currentIndex());
                            DCCManager::setNodeTransformation(mesh_obj, mesh.matrix);

                            locatorObj->AttachChild(mesh_obj, 0);
                        }

                        lodGroupObj->AttachChild(locatorObj);
                    }

                    index++;
                }

            }

            for (auto& [key, obj] : suspensions)
            {
                if (auto transform = suspension_transforms.find(key); transform != suspension_transforms.end())
                {
                    DCCManager::setNodeTransformation(obj, transform->second);

                    //MGlobal::displayInfo("Suspensions found!");
                }
                else {
                    //MGlobal::displayWarning("Suspensions not found!");
                }
            }

            for (auto& [key, obj] : calipers)
            {
                std::string spindle_key = "spindle" + DCCManager::GetContainerDirection(key);

                if (auto offset = rotor_center_offsets.find(spindle_key); offset != rotor_center_offsets.end())
                {
                    DirectX::XMMATRIX caliper_bone = caliper_transforms[key];
                    DirectX::XMMATRIX rotor_bone = rotor_transforms[spindle_key];

                    DirectX::XMMATRIX caliper_local_transform{};
                    DirectX::XMVECTOR caliper_local_translate{};

                    DirectX::XMMATRIX translate_x = DirectX::XMMatrixTranslation(spindle_offsets[spindle_key], 0, 0);

                    DirectX::XMMATRIX brake_transform{};
                    {
                        brake_transform += (translate_x * spindle_transforms[spindle_key]);

                        DCCManager::setNodeTransformation(rotors[spindle_key], brake_transform);
                    }

                    caliper_local_translate = DirectX::XMVectorSet(offset->second, DirectX::XMVectorGetY(caliper_bone.r[3]) - DirectX::XMVectorGetY(rotor_bone.r[3]), DirectX::XMVectorGetZ(caliper_bone.r[3]) - DirectX::XMVectorGetZ(rotor_bone.r[3]), 1.f);

                    auto direction = DCCManager::GetPartDirection(key);

                    if (direction.side == fmnext::PartLocation::RIGHT)
                    {
                        caliper_local_transform += (caliper_bone * DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(180)));
                        caliper_local_translate = DirectX::XMVectorSet(DirectX::XMVectorGetX(caliper_local_translate), DirectX::XMVectorGetY(caliper_local_translate), -DirectX::XMVectorGetZ(caliper_local_translate), DirectX::XMVectorGetW(caliper_local_translate));

                        caliper_local_transform.r[3] = caliper_local_translate;
                    }
                    else
                    {
                        caliper_local_transform = caliper_bone;

                        caliper_local_transform.r[3] = caliper_local_translate;
                    }

                    DirectX::XMMATRIX caliper_transform{}; // assume that hub_transform == spindle_transform (rotate around Y-axis)
                    {
                        caliper_transform += (caliper_local_transform * brake_transform);

                        DCCManager::setNodeTransformation(obj, caliper_transform);
                    }

                    //MGlobal::displayInfo("rotor_center_offset found!");
                }
                else
                {
                    //MGlobal::displayWarning("rotor_center_offset not found!");
                }
            }

        }

        if (DCCManager::objExists(lod_group_name))
        {
            //lodGroupObj = DCCManager::GetNodeByName(lod_group_name);
        }
    }
    else
    {
        for (auto& data : list_items)
        {
            if (data.item->isSelected())
            {
                int amount = static_cast<int>((index + 1) * 100 / count);

                ui->progressBar->setValue(amount);

                INode* locatorObj = nullptr;

                fmnext::MeshResolver resolver{};

                switch (data.type)
                {
                case 8: // Tires
                {
                    fmnext::PartLocation direction = DCCManager::GetPartDirection(data.model->bone_name);

                    resolver = fmnext::MeshResolver(data.bundle, current_lod->currentIndex(), static_cast<fmnext::GeometryType>(geometry_type->currentIndex()), p_records, 8, direction.end);
                    break;
                }
                case 44: // Wheels
                {
                    fmnext::PartLocation direction = DCCManager::GetPartDirection(data.model->bone_name);

                    resolver = fmnext::MeshResolver(data.bundle, current_lod->currentIndex(), static_cast<fmnext::GeometryType>(geometry_type->currentIndex()), p_records, 44, direction.end);
                    break;
                }
                default: // Other
                    resolver = fmnext::MeshResolver(data.bundle, current_lod->currentIndex(), static_cast<fmnext::GeometryType>(geometry_type->currentIndex()));
                    break;
                }

                if (!resolver.GetMeshes().empty())
                {
                    locatorObj = DCCManager::createLocator(data.model->transform);

                    std::string bundle_name = std::filesystem::path(data.model->path).stem().string();

                    locatorObj->SetName(fmnext::convertStdStringToWide(bundle_name).c_str());
                }

                for (const auto& mesh : resolver.GetMeshes())
                {
                    INode* mesh_obj = nullptr;
                    Mtl* material_obj = nullptr;

                    std::string mesh_name(mesh.name);

                    auto material = std::find_if(data.bundle->MaterialInstanceBundles.begin(), data.bundle->MaterialInstanceBundles.end(), [&](auto& mtl) {
                        return std::any_cast<int32_t>(mtl.metadata["Id"]) == mesh.material_index;
                        });

                    if (material != std::end(data.bundle->MaterialInstanceBundles))
                    {
                        mesh_name += "_";
                        mesh_name += std::any_cast<std::string>(material->metadata["Name"]);
                    }

                    if (auto material_it = data.materials.find(mesh.material_index); material_it != std::end(data.materials))
                    {
                        const auto& [key, material_data] = *material_it;

                        material_obj = DCCManager::createMaterialfromMemory(std::any_cast<std::string>(material->metadata["Name"]), material_data.instace);
                    }

                    mesh_obj = DCCManager::createMesh(&mesh, mesh_name, material_obj, geometry_type->currentIndex());
                    DCCManager::setNodeTransformation(mesh_obj, mesh.matrix);

                    locatorObj->AttachChild(mesh_obj);
                }

                index++;

            }
        }
    }

    //GetCOREInterface()->ForceCompleteRedraw(TRUE);
    GetCOREInterface()->RedrawViews(TRUE);
}