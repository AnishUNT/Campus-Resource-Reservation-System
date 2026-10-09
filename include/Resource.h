#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

class Resource {
public:
    Resource();
    Resource(int resourceId,
             const std::string& name,
             const std::string& type,
             int capacity);

    int getResourceId() const;
    const std::string& getName() const;
    const std::string& getType() const;
    int getCapacity() const;

    void setResourceId(int resourceId);
    void setName(const std::string& name);
    void setType(const std::string& type);
    void setCapacity(int capacity);

    void display() const;

    static void displayHeader();

private:
    int         m_resourceId;
    std::string m_name;
    std::string m_type;
    int         m_capacity;
};

#endif
