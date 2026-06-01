#include "fmoptionsdialog.h"

FMOptionsDialog::FMOptionsDialog(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* base_layout = new QVBoxLayout(this);
    QFrame* frame_base = new QFrame(this);
    frame_base->setMaximumHeight(FMQtWindow::dpiScale(160));

    QVBoxLayout* frame_layout = new QVBoxLayout(frame_base);

    levelOfDetail = new QComboBox(this);
    geometryType = new QComboBox(this);
    materialOption = new QCheckBox(this);
    skeletonOption = new QCheckBox(this);
    importButton = new QPushButton("Import");

    materialOption->setText("Materials");
    skeletonOption->setText("Skeleton");

    levelOfDetail->addItem("LODS");
    levelOfDetail->addItem("LOD0");
    levelOfDetail->addItem("LOD1");
    levelOfDetail->addItem("LOD2");
    levelOfDetail->addItem("LOD3");
    levelOfDetail->addItem("LOD4");
    levelOfDetail->addItem("LOD5");

    geometryType->addItem("Triangles");
    geometryType->addItem("Quads");

    frame_layout->addWidget(levelOfDetail);
    frame_layout->addWidget(geometryType);
    frame_layout->addWidget(materialOption);
    frame_layout->addWidget(skeletonOption);
    frame_layout->addWidget(importButton);

    frame_layout->setContentsMargins(QMargins(FMQtWindow::dpiScale(10), FMQtWindow::dpiScale(8), FMQtWindow::dpiScale(10), FMQtWindow::dpiScale(8)));
    base_layout->setContentsMargins(QMargins(FMQtWindow::dpiScale(10), FMQtWindow::dpiScale(8), FMQtWindow::dpiScale(10), FMQtWindow::dpiScale(8)));
    base_layout->addWidget(frame_base);

    this->setLayout(base_layout);
    this->setWindowTitle("ForzaTech Bundle Importer");
    this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);


    QObject::connect(importButton, &QAbstractButton::clicked, this, &FMOptionsDialog::importData);

    getPreferences();
}

void FMOptionsDialog::setModelData(std::string& path)
{
    m_path = path;
}

void FMOptionsDialog::importData()
{
    auto data = fmnext::BundleReader(m_path);

    if (data.Init())
    {
        auto bundle = std::make_shared<fmnext::BundleReader::BundleData>(data.bundle);
        auto resolver = fmnext::MeshResolver(bundle, levelOfDetail->currentIndex(), static_cast<fmnext::GeometryType>(geometryType->currentIndex()));

        if (skeletonOption->isChecked())
        {
            DCCManager::LoadSkeleton(bundle->Skeleton);
        }

        for (auto& mesh : resolver.GetMeshes())
        {
            INode* mesh_obj = nullptr;
            Mtl* material_obj = nullptr;

            std::string mesh_name;

            auto material = std::find_if(data.bundle.MaterialInstanceBundles.begin(), data.bundle.MaterialInstanceBundles.end(), [&](auto& mtl) {
                return std::any_cast<int32_t>(mtl.metadata["Id"]) == mesh.material_index;
                });

            if (material != std::end(data.bundle.MaterialInstanceBundles))
            {
                mesh_name += mesh.name;
                mesh_name += "_";
                mesh_name += std::any_cast<std::string>(material->metadata["Name"]);

                if (materialOption->isChecked())
                {
                    material_obj = DCCManager::createMaterialShader(std::any_cast<std::string>(material->metadata["Name"]));
                }
            }

            mesh_obj = DCCManager::createMesh(mesh.vertices, mesh.indices, mesh.normals, mesh.uvs, mesh_name, material_obj, geometryType->currentIndex());
            DCCManager::setNodeTransformation(mesh_obj, mesh.matrix);
        }
    }

    //GetCOREInterface()->ForceCompleteRedraw(TRUE);
    GetCOREInterface()->RedrawViews(TRUE);

    savePreferences();

    delete this;
}

void FMOptionsDialog::closeEvent(QCloseEvent* event)
{
    savePreferences();

    if (event->isAccepted())
    {
        delete this;
    }
}

void FMOptionsDialog::savePreferences()
{
    QSettings settings("Apex Software", "ForzaTech");

    settings.setValue("current_lod", levelOfDetail->currentIndex());
    settings.setValue("current_geo", geometryType->currentIndex());
    settings.setValue("import_materials", materialOption->isChecked());
    settings.setValue("import_skeleton", skeletonOption->isChecked());
}

void FMOptionsDialog::getPreferences()
{
    QSettings settings("Apex Software", "ForzaTech");

    if (settings.contains("current_lod"))
    {
        levelOfDetail->setCurrentIndex(settings.value("current_lod", QVariant()).toUInt());
    }

    if (settings.contains("current_geo"))
    {
        geometryType->setCurrentIndex(settings.value("current_geo", QVariant()).toUInt());
    }

    if (settings.contains("import_materials"))
    {
        materialOption->setChecked(settings.value("import_materials", QVariant()).toBool());
    }

    if (settings.contains("import_skeleton"))
    {
        skeletonOption->setChecked(settings.value("import_skeleton", QVariant()).toBool());
    }
}