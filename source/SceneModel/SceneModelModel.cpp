#include "SceneModelModel.h"

namespace gui
{
        SceneModelModel::SceneModelModel(gfx::SceneModelManager* manager, QObject* parent)
            : gui::Model<gfx::SceneModel, gfx::SceneModelManager>(manager, parent)
        {
            refreshModelView();
        }


        int SceneModelModel::rowCount(const QModelIndex &parent) const
        {
            Q_UNUSED(parent);
            return static_cast<int>(m_activeKeys.size());
        }


        QVariant SceneModelModel::data(const QModelIndex &index, int role) const
        {
            if (!index.isValid() || m_manager == nullptr)
            {
                return QVariant();
            }

            auto key = m_activeKeys[index.row()];

            const auto& sceneModel = m_manager->getMap().at(key);
            
            switch (role)
            {
                case Qt::BackgroundRole:
                    return colourBackground(sceneModel.get());

                case Qt::ToolTipRole:
                    return formatToolTip(sceneModel.get());

                case NameRole:    
                    return QString::fromStdString(sceneModel->sourceFileName);                 
                    
                case IsLoadedRole:    
                    return boolToString(sceneModel->isLoaded);           

                case FilePathRole:    
                    return QString::fromStdString(sceneModel->sourceFilePath.string());          


                default:
                    return QVariant();
            }
        }

        QString SceneModelModel::boolToString(bool flag) const
        {
            if (flag)
            {
                return QString("True");
            }
            else
            {
                return QString("False");
            }
        }


        std::string SceneModelModel::activeDirectoryWarning(gfx::SceneModel* sceneModel) const
        {
             
            if (sceneModel->sourceFilePath.parent_path() != std::filesystem::path(m_manager->getCurrentActiveDirectory()))
            {
                return std::string("<b style='color: #ffb2b2' > Not in current active directory.<br>");
            }

            return std::string();
        }


        QString SceneModelModel::formatToolTip(gfx::SceneModel* sceneModel) const
        {
            return QString(
            "<center><b>%1</b></center><br>"
            "<b>Filepath:</b> %2<br>"
            "<b>Loaded: </b> %3<br>"
            "%4"
            ).arg(sceneModel->sourceFileName)
            .arg(sceneModel->sourceFilePath.string())
            .arg(boolToString(sceneModel->isLoaded))
            .arg(activeDirectoryWarning(sceneModel));
        }


        QBrush SceneModelModel::colourBackground(gfx::SceneModel* sceneModel) const
        {
            if (sceneModel->isLoaded)
            {
                return QBrush(positiveGreen); // Green
            }

            return QBrush(negativeRed); // Red
        }
}
