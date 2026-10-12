#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <cstddef>
#include <string>
#include <vector>

#include "Resource.h"

class ResourceManager {
public:
    ResourceManager();

    bool loadFromFile(const std::string& filePath);

    void displayAllResources() const;

    std::vector<Resource> resourcesSortedByName() const;
    void displayResourcesSortedByName() const;

    const Resource* findResourceById(int resourceId) const;

    bool resourceExists(int resourceId) const;

    std::size_t getResourceCount() const;
    int         getSkippedRecordCount() const;

    const std::vector<Resource>& getResources() const;

private:
    std::vector<Resource> m_resources;
    int                   m_skippedRecords;
};

#endif
