#ifndef MATERIAL_MODEL_H
#define MATERIAL_MODEL_H

#include <QObject>
#include <QBrush>

#include "Model.h"
#include "MaterialManager.h"
#include "MaterialTypes.h"
#include "UIColours.h"

class MaterialModel : public Model<gfx::MaterialProperties, gfx::MaterialManager>
{
    Q_OBJECT
    
    public:
        explicit MaterialModel(gfx::MaterialManager* manager, QObject* parent);

        //QT interface
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        // Button functions
        void createMaterial(std::string name, gfx::MaterialProperties&& materialInitProperties);
        void deleteMaterial(std::string key);

        enum TextureRole
        {
            NameRole            = Qt::DisplayRole,
            ShaderIDRole        = Qt::UserRole + 1,
            DiffuseTextureRole  = Qt::UserRole + 2,
            SpecularTextureRole = Qt::UserRole + 3,
            ShininessRole       = Qt::UserRole + 4,
        };
        
    private:
        QString formatToolTip(gfx::MaterialProperties* material) const override;
        QBrush colourBackground(gfx::MaterialProperties* material) const override;

};


#endif