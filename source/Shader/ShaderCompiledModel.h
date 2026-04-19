#ifndef SHADER_COMPILED_MODEL_H
#define SHADER_COMPILED_MODEL_H

#include <QBrush>
#include <QColor>
#include <QString>
#include <QList>

#include <QObject>

#include "Model.h"
#include "ShaderTypes.h"
#include "ShaderManager.h"

#include <algorithm>

#include "UIColours.h"


namespace gui
{

class ShaderCompiledModel : public Model<gfx::Shader, gfx::ShaderManager>
{
    Q_OBJECT
    
    public:
        explicit ShaderCompiledModel(gfx::ShaderManager* manager, QObject* parent);

        //QT interface
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        std::string decodeShaderProgramStatus(const gfx::ShaderProgramStatus status) const;

        // Button functions
        void unloadShaderProgram(const std::string& name);

        void refreshCompiledView();

        enum ShaderCompiledRole
        {
            NameRole            = Qt::DisplayRole,
            ShaderIDRole        = Qt::UserRole + 1,
            ProgramStatusRole   = Qt::UserRole + 2,
            LinkLogRole         = Qt::UserRole + 3
        };

    private:
        QString formatToolTip(gfx::Shader* element) const override;
        QBrush colourBackground(gfx::Shader* element) const override;

        std::string displaySourcePaths(std::vector<std::filesystem::path>& sourcePaths) const;


        
};

}

#endif