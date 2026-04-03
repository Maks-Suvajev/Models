#include "TextureModel.h"

TextureModel::TextureModel(gfx::TextureManager* manager, QObject* parent)
    : Model<gfx::Texture, gfx::TextureManager>(manager, parent)
{
    refreshModelView();
}

void TextureModel::loadTexture(std::string key)
{
    m_manager->loadTexture(key);
}

void TextureModel::unloadTexture(std::string key)
{
    m_manager->unloadTexture(key);
}

int TextureModel::rowCount(const QModelIndex &parent) const
{
    return m_activeKeys.size();
}

QString TextureModel::decodeTextureFormat(GLenum textureFormat) const
{
    switch(textureFormat)
    {
        case GL_RED:
            return QString("GL_RED");
            break;

        case GL_GREEN:
            return QString("GL_GREEN");
            break;

        case GL_BLUE:
            return QString("GL_BLUE");
            break;

        case GL_ALPHA:
            return QString("GL_ALPHA");
            break;

        case GL_RGB:
            return QString("GL_RGB");
            break;

        case GL_RGBA:
            return QString("GL_RGBA");
            break;
        
        default:
            return QString("INVALID FORMAT");
    }
}

std::string TextureModel::activeDirectoryWarning(gfx::Texture* texture) const
{
    if (texture->systemSourcePath.parent_path() != std::filesystem::path(m_manager->getCurrentActiveDirectory()))
    {
        return std::string("<b style='color: #ffb2b2' > Not in current active directory.<br>");
    }

    return std::string();
}

QString TextureModel::formatToolTip(gfx::Texture* texture) const
{
    return QString(
        "<center><b>%1</b></center><br>"
        "<b>Texture ID:</b> %2<br>"
        "<b>Texture Format:</b> %3<br>"
        "<b>Resolution:</b> %4x%5<br>"
        "<b>Number of channels:</b> %6<br>"
        "<b>Filepath:</b> %7<br>"
        "%8"
    ).arg(texture->name)
    .arg(texture->textureID)
    .arg(decodeTextureFormat(texture->textureFormat))
    .arg(texture->width)
    .arg(texture->height)
    .arg(texture->nrChannels)
    .arg(texture->systemSourcePath.string())
    .arg(activeDirectoryWarning(texture));
}

QBrush TextureModel::colourBackground(gfx::Texture* texture) const
{
    if (texture->isLoaded)
    {
        return QBrush(positiveGreen); // Green
    }
    else
    {
        return QBrush(negativeRed); // Red
    }
}

QVariant TextureModel::data(const QModelIndex &index, int role) const
{ 
    if (!index.isValid() || m_manager == nullptr)
    {
        return QVariant();
    }

    auto key = m_activeKeys[index.row()];

    const auto& texture = m_manager->getMap().at(key);
    
    switch (role)
    {
        case Qt::BackgroundRole:
            return colourBackground(texture.get());

        case Qt::ToolTipRole:
            return formatToolTip(texture.get());

        case NameRole:    
            return QString::fromStdString(texture->name);                                             

        case TextureIDRole:
            return texture->textureID;

        case TextureFormatRole:
            return texture->textureFormat;

        case WidthRole:
            return texture->width;

        case HeightRole:
            return texture->height;

        case NrChannelsRole:
            return texture->nrChannels;

        case FilePathRole:
            return QString::fromStdString(texture->systemSourcePath.generic_string());

        case LoadedRole:
            return texture->isLoaded;

        default:
            return QVariant();
    }

    return QVariant();
}
