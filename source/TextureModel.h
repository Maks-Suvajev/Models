#ifndef TEXTURE_MODEL_H
#define TEXTURE_MODEL_H

#include <QBrush>
#include <QColor>
#include <QString>

#include <QObject>

#include "Model.h"
#include "TextureTypes.h"
#include "TextureManager.h"
#include "UIColours.h"

class TextureModel : public Model<gfx::Texture, gfx::TextureManager>
{
    Q_OBJECT
    
    public:
        explicit TextureModel(gfx::TextureManager* manager, QObject* parent);

        //QT interface
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        // Button functions
        void refreshModelView() override;
        void loadTexture(std::string key);
        void unloadTexture(std::string key);

        enum TextureRole
        {
            NameRole          = Qt::DisplayRole,
            TextureIDRole     = Qt::UserRole + 1,
            TextureFormatRole = Qt::UserRole + 2,
            WidthRole         = Qt::UserRole + 3,
            HeightRole        = Qt::UserRole + 4,
            NrChannelsRole    = Qt::UserRole + 5,
            FilePathRole      = Qt::UserRole + 6,
            LoadedRole        = Qt::UserRole + 7
        };
        
    private:
        QString decodeTextureFormat(GLenum textureFormat) const;
        QString formatToolTip(std::string key, gfx::Texture* element) const override;
        QBrush colourBackground(gfx::Texture* texture) const override;
};


#endif