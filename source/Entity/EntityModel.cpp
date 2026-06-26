#include "EntityModel.h"


namespace gui
{

EntityModel::EntityModel(EntityManager* entityManager, QObject* parent)
    : m_manager(entityManager)
{
    refreshModelView();
}

QVariant EntityModel::data(const QModelIndex &index, int role) const
{ 
    if (!index.isValid() || m_manager == nullptr)
    {
        return QVariant();
    }

    if (!index.isValid() || 
        role != Qt::DisplayRole || 
        m_manager == nullptr) 
    {
        return QVariant();
    }

    return buildDisplayString(index);
}

QModelIndex EntityModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!parent.isValid())
    {
        Entity::Entity id = activeIDs[row];

        int nodeIndex = m_entityNodeIndex.at(id);

        return createIndex(row, column, &m_nodes[nodeIndex]);
    }

    EntityComponentNode* parentEntityNode = static_cast<EntityComponentNode*>(parent.internalPointer());

    Entity::Entity entity = parentEntityNode->entityID;

    int entityPos = m_entityNodeIndex.at(entity);

    return createIndex(row, column, &m_nodes[entityPos + 1 + row]);
}

QModelIndex EntityModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
    {
        return QModelIndex{};
    }

    EntityComponentNode* node = static_cast<EntityComponentNode*>(child.internalPointer());

    if (node->type == std::type_index(typeid(Entity::Entity)))
    {
        return QModelIndex{};
    }

    int parentRow = m_entityNodeIndex.at(node->entityID);

    return createIndex(parentRow, 0, &m_nodes[parentRow]);
}

int EntityModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return activeIDs.size();
    }

    EntityComponentNode* entityNode = static_cast<EntityComponentNode*>(parent.internalPointer());

    return m_manager->getEntityComponentCount(entityNode->entityID);
}

int EntityModel::columnCount(const QModelIndex &parent) const
{
    if (m_manager == nullptr)
    {
        return 0;
    }
    
    return 1;
}

void EntityModel::refreshModelView()
{
    beginResetModel();

    refreshElements();

    endResetModel();
}

void EntityModel::refreshElements()
{
    updateActiveIDs();

    m_nodes.clear();

    m_nodes.reserve(activeIDs.size() + m_manager->componentsSize());
    
    for (const Entity::Entity& entity : activeIDs)
    {
        EntityComponentNode entityNode{ .entityID = entity,
                                        .type = std::type_index(typeid(Entity::Entity)) };

        m_entityNodeIndex[entity] = m_nodes.size();

        m_nodes.push_back(std::move(entityNode));

        std::vector<std::type_index> componentTypes = m_manager->getEntityComponentTypes(entity);

        for (std::type_index type : componentTypes)
        {
            EntityComponentNode componentNode{.entityID = entity,
                                              .type = type};

            m_nodes.push_back(std::move(componentNode));
        }
    }
}


}