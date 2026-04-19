#ifndef SCENE_MODEL_MODEL_H
#define SCENE_MODEL_MODEL_H

#include <QBrush>
#include <QColor>
#include <QString>

#include <QObject>

#include "Model.h"
#include "UIColours.h"

#include "SceneModelManager.h"

namespace gui
{

class SceneModelModel : public gui::Model<gfx::SceneModel, gfx::SceneModelManager>
{
    Q_OBJECT
    
    public:
        explicit SceneModelModel(gfx::SceneModelManager* manager, QObject* parent);

        //QT interface
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        enum SceneModelRole
        {
            NameRole     = Qt::DisplayRole,
            IsLoadedRole = Qt::UserRole + 1,
            FilePathRole = Qt::UserRole + 2
        };

        void loadModel(const std::string& key)
        {
            m_manager->loadModel(std::filesystem::canonical(key).generic_string()); // Using canonical to standardise path string format
            refreshModelView();
        }
        
    private:
        std::string activeDirectoryWarning(gfx::SceneModel* sceneModel) const;

        QString formatToolTip(gfx::SceneModel* element) const override;
        QBrush colourBackground(gfx::SceneModel* sceneModel) const override;
        QString boolToString(bool flag) const;

};

}
#endif