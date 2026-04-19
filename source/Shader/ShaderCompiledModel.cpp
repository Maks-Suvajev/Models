
#include "ShaderCompiledModel.h"


namespace gui
{

ShaderCompiledModel::ShaderCompiledModel(gfx::ShaderManager* manager, QObject* parent)
    : Model<gfx::Shader, gfx::ShaderManager>(manager, parent)
{
    refreshCompiledView();
}

// Button functions
void ShaderCompiledModel::unloadShaderProgram(const std::string& name)
{
    m_manager->unloadShaderProgram(name);
    refreshCompiledView();

}

std::string ShaderCompiledModel::decodeShaderProgramStatus(const gfx::ShaderProgramStatus status) const
{
    switch(status)
    {
        case gfx::ShaderProgramStatus::Unlinked:
            return std::string("Unlinked");

        case gfx::ShaderProgramStatus::Linked:
            return std::string("Linked");

        case gfx::ShaderProgramStatus::Error:
            return std::string("Error");

        default:
            return std::string();
    }
}

std::string ShaderCompiledModel::displaySourcePaths(std::vector<std::filesystem::path>& sourcePaths) const
{
    std::string formattedString = "<b>Source File Names:</b><br>";
    
    for (const auto& path : sourcePaths)
    {
        formattedString = formattedString + "<b>    " + path.filename().string() + "<\b>";
    }

    return formattedString;
}

QString ShaderCompiledModel::formatToolTip(gfx::Shader* element) const
{
    return QString(
        "<center><b>%1</b></center><br>"
        "<b>Compiled ID:</b> %2<br>"
        "%3"
    ).arg(element->getShaderName())
    .arg(element->getShaderID())
    .arg(displaySourcePaths(element->getSourcePaths()));
}

QBrush ShaderCompiledModel::colourBackground(gfx::Shader* element) const
{
    return QBrush(positiveGreen);  
}

void ShaderCompiledModel::refreshCompiledView()
{
    beginResetModel();

    std::vector<std::string> currActiveKeys;

    m_activeKeys.clear();

    for (const auto& [key, element] : static_cast<gfx::ShaderManager*>(m_manager)->getCompiledMap())
    {
        m_activeKeys.push_back(key);
    }

    endResetModel();
}

int ShaderCompiledModel::rowCount(const QModelIndex &parent) const
{
    return m_activeKeys.size();
}

QVariant ShaderCompiledModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || m_manager == nullptr)
    {
        return QVariant();
    }

    auto key = m_activeKeys[index.row()];

    const auto& compiledShader = m_manager->getCompiledMap().at(key).get();

    switch (role)
    {
        case Qt::BackgroundRole:
            return colourBackground(compiledShader);

        case Qt::ToolTipRole:
            return formatToolTip(compiledShader);

        case NameRole:    
            return QString::fromStdString(compiledShader->getShaderName());      

        case ShaderIDRole:    
            return compiledShader->getShaderID();

        case ProgramStatusRole:    
            return QString::fromStdString(decodeShaderProgramStatus(compiledShader->getShaderProgramStatus()));

        case LinkLogRole:    
            return QString::fromStdString(compiledShader->getLinkLog());

        default:
            return QVariant();
    }
}

}