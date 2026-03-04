#include "TextureModel.h"

TextureModel::TextureModel(gfx::TextureManager* manager, QObject* parent)
    : QAbstractListModel(parent),
      m_manager(manager)
{
    refreshTextures();
}

std::filesystem::path TextureModel::getCurrentTextureDirectory()
{
    return m_manager->getCurrentWorkingDirectory();
}


void TextureModel::loadTexture(std::string key)
{
    m_manager->loadTexture(key);
}

void TextureModel::unloadTexture(std::string key)
{
    m_manager->unloadTexture(key);
}

void TextureModel::refreshTextures()
{
    beginResetModel();

    std::vector<std::string> currActiveTextures;
    m_manager->refreshTextures(); 

    for (const auto& [key, texture] : m_manager->getMap())
    {
        currActiveTextures.push_back(key);
    }

    m_activeTextureKeys = currActiveTextures;

    endResetModel();
}

int TextureModel::rowCount(const QModelIndex &parent) const
{
    return m_activeTextureKeys.size();
}

void TextureModel::updateTexturePath(std::string path)
{
    m_manager->updateTexturePath(path);
    refreshTextures();
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

QString TextureModel::formatToolTip(std::string key, gfx::Texture* texture) const
{
    if ( texture->systemSourcePath.parent_path() != m_manager->getCurrentWorkingDirectory())
    {
        return QString(
            "<center><b>%1</b></center><br>"
            "<b>Texture ID:</b> %2<br>"
            "<b>Texture Format:</b> %3<br>"
            "<b>Resolution:</b> %4x%5<br>"
            "<b>Number of channels:</b> %6<br>"
            "<b>Filepath:</b> %7<br>"
            "<b style='color: #ffb2b2' > Not in current active directory.<br>"
        ).arg(key)
        .arg(texture->textureID)
        .arg(decodeTextureFormat(texture->textureFormat))
        .arg(texture->width)
        .arg(texture->height)
        .arg(texture->nrChannels)
        .arg(texture->systemSourcePath.string());

    };

    return QString(
        "<center><b>%1</b></center><br>"
        "<b>Texture ID:</b> %2<br>"
        "<b>Texture Format:</b> %3<br>"
        "<b>Resolution:</b> %4x%5<br>"
        "<b>Number of channels:</b> %6<br>"
        "<b>Filepath:</b> %7<br>"
    ).arg(key)
    .arg(texture->textureID)
    .arg(decodeTextureFormat(texture->textureFormat))
    .arg(texture->width)
    .arg(texture->height)
    .arg(texture->nrChannels)
    .arg(texture->systemSourcePath.string());
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

    auto key = m_activeTextureKeys[index.row()];

    const auto& texture = m_manager->getMap().at(key);
    
    switch (role)
    {
        case Qt::BackgroundRole:
            return colourBackground(texture.get());

        case Qt::ToolTipRole:
            return formatToolTip(key, texture.get());

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
