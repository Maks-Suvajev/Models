#ifndef TEXTURE_MODEL_H
#define TEXTURE_MODEL_H

#include <QBrush>
#include <QColor>

#include <QObject>
#include <QAbstractListModel>

#include "TextureManager.h"
#include "UIColours.h"

class TextureModel : public QAbstractListModel
{
    Q_OBJECT
    
    public:
        explicit TextureModel(gfx::TextureManager* manager, QObject* parent);

        //QT interface
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        std::filesystem::path getCurrentTextureDirectory();
        // Button functions
        void refreshTextures();
        void loadTexture(std::string key);
        void unloadTexture(std::string key);
        void updateTexturePath(std::string path);

        
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
        QString formatToolTip(std::string key, gfx::Texture* texture) const;
        QBrush colourBackground(gfx::Texture* texture) const;


        gfx::TextureManager*        m_manager;
        std::vector<std::string>    m_activeTextureKeys; 
};


#endif