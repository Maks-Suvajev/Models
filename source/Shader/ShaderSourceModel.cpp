#include "ShaderSourceModel.h"


ShaderSourceModel::ShaderSourceModel(gfx::ShaderManager* manager, QObject* parent)
    : Model<gfx::ShaderSource, gfx::ShaderManager>(manager, parent)
{
    refreshModelView();
}

std::string ShaderSourceModel::decodeShaderType(GLenum type) const
{
    switch(type)
    {
        case GL_VERTEX_SHADER:
            return std::string("Vertex");
            break;

        case GL_FRAGMENT_SHADER:
            return std::string("Fragment");
            break;

        case GL_INVALID_ENUM:
            return std::string("Undetected");

            break;

        default:
            return std::string("ERROR");
    }
}

std::string ShaderSourceModel::decodeShaderStatus(gfx::SourceStatus status) const
{
    switch(status)
    {
        case gfx::SourceStatus::Untouched:
            return std::string("Untouched");
            break;

        case gfx::SourceStatus::Compiled:
            return std::string("Compiled");
            break;

        case gfx::SourceStatus::EmptyFileError:
            return std::string("Empty file");
            break;

        case gfx::SourceStatus::Loaded:
            return std::string("Loaded");

        case gfx::SourceStatus::CompilationError:
            return std::string("Compilation Error");

        default:
            return std::string("ERROR");
    }
}

QVariant ShaderSourceModel::data(const QModelIndex &index, int role) const
{ 
    if (!index.isValid() || m_manager == nullptr)
    {
        return QVariant();
    }

    auto key = m_activeKeys[index.row()];

    const auto& shaderSource = m_manager->getMap().at(key).get();
    
    switch (role)
    {
        case Qt::BackgroundRole:
            return colourBackground(shaderSource);

        case Qt::ToolTipRole:
            return formatToolTip(shaderSource);

        case NameRole:    
            return QString::fromStdString(shaderSource->name);      

        case TypeRole:    
            return QString::fromStdString(decodeShaderType(shaderSource->type));

        case FilePathRole:    
            return QString::fromStdString(shaderSource->systemSourcePath.string());

        case SourceCodeRole:    
            return QString::fromStdString(shaderSource->sourceCode);

        case StatusRole:    
            return QString::fromStdString(decodeShaderStatus(shaderSource->status));

        case CompiledIDRole:    
            return shaderSource->compiledID;

        case InfoLogRole:    
            return QString::fromStdString(shaderSource->infoLog);

        case LinkedShaderKeysRole:   
        {
            QStringList keys;

            std::transform(shaderSource->linkedShaderKeys.begin(), shaderSource->linkedShaderKeys.end(), std::back_inserter(keys), &QString::fromStdString);

            return QVariant::fromValue(keys);
        }

        default:
            return QVariant();
    }

    return QVariant();
}

int ShaderSourceModel::rowCount(const QModelIndex &parent) const
{
    return m_activeKeys.size();
}

void ShaderSourceModel::compileSource(const std::string& key)
{
    m_manager->compileShaderWithKey(key);
}

void ShaderSourceModel::linkIntoProgram(const std::string& sourceA, const std::string& sourceB, const std::string& name)
{
    m_manager->compileShaderProgram(sourceA, sourceB, name);
}

void ShaderSourceModel::unloadShader(const std::string& key)
{
    m_manager->unloadShader(key);
}

QBrush ShaderSourceModel::colourBackground(gfx::ShaderSource* shaderSource) const
{
    if (shaderSource->status != gfx::SourceStatus::Compiled)
    {
        return QBrush(negativeRed); // Red    
    }

    return QBrush(positiveGreen); // Green    
}

std::string ShaderSourceModel::activeDirectoryWarning(gfx::ShaderSource* shaderSource) const
{
    if (shaderSource->systemSourcePath.parent_path() != std::filesystem::path(m_manager->getCurrentActiveDirectory()))
    {
        return std::string("<b style='color: #ffb2b2' > Not in current active directory.<br>");
    }

    return std::string();
}

QString ShaderSourceModel::formatToolTip(gfx::ShaderSource* shaderSource) const
{
    return QString(
        "<center><b>%1</b></center><br>"
        "<b>Source Type:</b> %2<br>"
        "<b>Compiled ID:</b> %3<br>"
        "<b>Filepath:</b> %4<br>"
        "%6"
    ).arg(shaderSource->name)
    .arg(decodeShaderType(shaderSource->type))
    .arg(shaderSource->compiledID)
    .arg(shaderSource->systemSourcePath.string())
    .arg(activeDirectoryWarning(shaderSource));
}
