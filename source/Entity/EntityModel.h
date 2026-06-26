#ifndef ENTITY_MODEL_H
#define ENTITY_MODEL_H

#include <QObject>
#include <QBrush>

#include "Model.h"
#include "EntityManager.h"
#include "Entity.h"

#include "UIColours.h"

#include "PhysicsTypes.h"


namespace gui
{

struct EntityComponentNode
{
    Entity::Entity entityID;
    std::type_index type;
};

class EntityModel : public QAbstractItemModel
{
    Q_OBJECT
    
    public:
        explicit EntityModel(EntityManager* entityManager, QObject* parent);

        //QT interface
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QModelIndex index(int row, int column, const QModelIndex& parent) const override;
        QModelIndex parent(const QModelIndex &child) const override;

        void refreshModelView();
        void refreshElements();

    private:
        void updateActiveIDs()
        {
            activeIDs = m_manager->getActiveEntityIDs();
        }


        QString buildDisplayString(const QModelIndex &index) const
        {
            EntityComponentNode* node = static_cast<EntityComponentNode*>(index.internalPointer());

            if (node->type == std::type_index(typeid(Entity::Entity)))
            {
                return QString(QString::fromStdString("Entity " + std::to_string(node->entityID)));
            }

            if (node->type == std::type_index(typeid(gfx::Material)))
            {
                return QString("Component: gfx::Material");
            }
            else if (node->type == std::type_index(typeid(gfx::RenderNode)))
            {
                return QString("Component: gfx::RenderNode");
            }
            else if (node->type == std::type_index(typeid(gfx::RenderHierarchy)))
            {
                return QString("Component: gfx::RenderHierarchy");
            }
            else if (node->type == std::type_index(typeid(gfx::Transform)))
            {
                return QString("Component: gfx::Transform");
            }

            return QString("Can't detect type.");
        }

        std::vector<EntityComponentNode> m_nodes;
        std::unordered_map<Entity::Entity, int> m_entityNodeIndex;
        std::vector<Entity::Entity> activeIDs;
        
        EntityManager* m_manager;

};

}

#endif