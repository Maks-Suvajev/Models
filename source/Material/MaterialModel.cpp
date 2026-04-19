#include "MaterialModel.h"


namespace gui
{

MaterialModel::MaterialModel(gfx::MaterialManager* manager, QObject* parent)
    : Model<gfx::Material, gfx::MaterialManager>(manager, parent)
{
    refreshModelView();
}

void MaterialModel::createMaterial(std::string name, gfx::Material&& material)
{
    m_manager->registerElement(name, std::move(material));
}

void MaterialModel::deleteMaterial(std::string key)
{
    m_manager->deleteElement(key);
}

int MaterialModel::rowCount(const QModelIndex &parent) const
{
    return m_activeKeys.size();
}


QString MaterialModel::formatToolTip(gfx::Material* material) const
{
    return QString(
        "<center><b>%1</b></center><br>"
        "<b>Shader ID:</b> %2<br>"
        "<b>Diffuse Texture ID:</b> %3<br>"
        "<b>Specular Texture ID:</b> %4x%5<br>"
        "<b>Shininess value:</b> %6<br>"
    ).arg(material->name)
    .arg(material->shader)
    .arg(material->lightingTextures.diffuse)
    .arg(material->lightingTextures.specular)    
    .arg(material->shininess);
}

QBrush MaterialModel::colourBackground(gfx::Material* material) const
{
    return QBrush(positiveGreen); 
}

QVariant MaterialModel::data(const QModelIndex &index, int role) const
{ 
    if (!index.isValid() || m_manager == nullptr)
    {
        return QVariant();
    }

    auto key = m_activeKeys[index.row()];

    auto material = m_manager->getMap().at(key).get();
    
    switch (role)
    {
        case Qt::BackgroundRole:
            return colourBackground(material);

        case Qt::ToolTipRole:
            return formatToolTip(material);

        case NameRole:    
            return QString::fromStdString(material->name);                                             

        case ShaderIDRole:
            return material->shader;

        case DiffuseTextureRole:
            return material->lightingTextures.diffuse;

        case SpecularTextureRole:
            return material->lightingTextures.specular;

        case ShininessRole:
            return material->shininess;

        default:
            return QVariant();
    }

    return QVariant();
}

}