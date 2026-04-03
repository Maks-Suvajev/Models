#ifndef SHADER_SOURCE_MODEL_H
#define SHADER_SOURCE_MODEL_H

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

class ShaderSourceModel : public Model<gfx::ShaderSource, gfx::ShaderManager>
{
    Q_OBJECT
    
    public:
        explicit ShaderSourceModel(gfx::ShaderManager* manager, QObject* parent);

        //QT interface
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = ShaderSourceRole::NameRole) const override;

        std::string decodeShaderType(const GLenum type) const;
        std::string decodeShaderStatus(const gfx::SourceStatus status) const;

        // Button functions
        void compileSource(const std::string& key);
        void linkIntoProgram(const std::string& sourceAKey, const std::string& sourceBKey, const std::string& name);
        void unloadShader(const std::string& key);


        enum ShaderSourceRole
        {
            NameRole             = Qt::DisplayRole,
            TypeRole             = Qt::UserRole + 1,
            FilePathRole         = Qt::UserRole + 2,
            SourceCodeRole       = Qt::UserRole + 3,
            StatusRole           = Qt::UserRole + 4,
            CompiledIDRole       = Qt::UserRole + 5,
            InfoLogRole          = Qt::UserRole + 6,
            LinkedShaderKeysRole = Qt::UserRole + 7,
        };
        
    private:
        std::string activeDirectoryWarning(gfx::ShaderSource* shaderSource) const;
        QString formatToolTip(gfx::ShaderSource* element) const override;
        QBrush colourBackground(gfx::ShaderSource* element) const override;
};


#endif