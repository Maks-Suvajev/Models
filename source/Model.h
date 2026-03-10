#ifndef MODEL_H
#define MODEL_H

#include "ResourceManager.h"
#include <QAbstractListModel>

template<typename T, typename TManager = ResourceManager<T>>
class Model : public QAbstractListModel
{
    public:
        Model(TManager* manager, QObject* parent);

        // For displaying current active directory to user
        std::string getActiveDirectory();

        // Button functions
        void setActiveDirectory(std::string path);

        virtual void refreshModelView() = 0;
        void refreshElements();

            
    protected:
        virtual QString formatToolTip(std::string key, T* element) const = 0;
        virtual QBrush colourBackground(T* element) const = 0;


        TManager*                   m_manager;
        std::vector<std::string>    m_activeKeys; 
};

template<typename T, typename TManager>
Model<T, TManager>::Model(TManager* manager, QObject* parent)
    : m_manager(manager),
      QAbstractListModel(parent)
{

}

template<typename T, typename TManager>
void Model<T, TManager>::setActiveDirectory(std::string path)
{
    m_manager->setCurrentActiveDirectory(path);
    m_manager->refreshElements();
}

template<typename T, typename TManager>
std::string Model<T, TManager>::getActiveDirectory()
{
    return m_manager->getCurrentActiveDirectory();
}



template<typename T, typename TManager>
void Model<T, TManager>::refreshElements()
{

    std::vector<std::string> currActiveKeys;
    m_manager->refreshElements(); 

    for (const auto& [key, element] : m_manager->getMap())
    {
        currActiveKeys.push_back(key);
    }

    m_activeKeys = currActiveKeys;
}


#endif