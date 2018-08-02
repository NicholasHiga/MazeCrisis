#pragma once
#include <map>
#include <string>
#include <memory>

template <class T>
class ResourceManager
{
protected:
    std::map<std::string, std::shared_ptr<T>> resources;

public:
    typedef typename std::map<std::string, 
        std::shared_ptr<T>>::iterator ResourceIterator;
    typedef typename std::map<std::string,
        std::shared_ptr<T>>::const_iterator ResourceIteratorConst;

    // Does not overwrite if copy already found, returns true on success,
    // false on fail.
    bool store(std::string name, std::shared_ptr<T> resource)
    {
        ResourceIterator it = resources.find(name);
        if (it == resources.end())
        {
            resources[name] = move(resource);
            return true;
        }
        return false;
    }

    // Retreive resource using the given name, if not found return null.
    T* get(std::string name)
    {
        ResourceIterator it = resources.find(name);
        if (it != resources.end())
            return it->second.get();
        return nullptr;
    }

    bool remove(std::string &name)
    {
        if (resources[name] != nullptr)
        {
            resources.erase(name);
            return true;
        }
        return false;
    }

    ResourceIterator begin()
    {
        return resources.begin();
    }

    ResourceIterator end()
    {
        return resources.end();
    }

protected:
    virtual ~ResourceManager() {};
};