#include "Resource.h"

#include <iomanip>
#include <iostream>

Resource::Resource()
    : m_resourceId(0), m_name(""), m_type(""), m_capacity(0) {}

Resource::Resource(int resourceId,
                   const std::string& name,
                   const std::string& type,
                   int capacity)
    : m_resourceId(resourceId), m_name(name), m_type(type), m_capacity(capacity) {}

int Resource::getResourceId() const { return m_resourceId; }

const std::string& Resource::getName() const { return m_name; }

const std::string& Resource::getType() const { return m_type; }

int Resource::getCapacity() const { return m_capacity; }

void Resource::setResourceId(int resourceId) { m_resourceId = resourceId; }

void Resource::setName(const std::string& name) { m_name = name; }

void Resource::setType(const std::string& type) { m_type = type; }

void Resource::setCapacity(int capacity) { m_capacity = capacity; }

void Resource::displayHeader() {
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(28) << "Name"
              << std::setw(16) << "Type"
              << std::right << std::setw(9) << "Capacity" << "\n";
    std::cout << std::string(59, '-') << "\n";
}

void Resource::display() const {
    std::cout << std::left
              << std::setw(6)  << m_resourceId
              << std::setw(28) << m_name
              << std::setw(16) << m_type
              << std::right << std::setw(9) << m_capacity << "\n";
}
