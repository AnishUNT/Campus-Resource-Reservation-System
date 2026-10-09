#include "ResourceManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Reservation.h"

ResourceManager::ResourceManager()
    : m_resources(), m_skippedRecords(0) {}

bool ResourceManager::loadFromFile(const std::string& filePath) {
    m_resources.clear();
    m_skippedRecords = 0;

    std::ifstream input(filePath.c_str());
    if (!input.is_open()) {
        std::cout << "ERROR: could not open resource file \"" << filePath << "\".\n"
                  << "       Make sure the file exists and the program is run from the\n"
                  << "       project root directory.\n";
        return false;
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(input, line)) {
        ++lineNumber;

        const std::string trimmedLine = ReservationUtils::trim(line);
        if (trimmedLine.empty() || trimmedLine[0] == '#') {
            continue;
        }

        std::stringstream recordStream(trimmedLine);
        std::string       field;
        std::string       fields[4];
        int               fieldCount = 0;
        bool              tooManyFields = false;

        while (std::getline(recordStream, field, '|')) {
            if (fieldCount < 4) {
                fields[fieldCount] = ReservationUtils::trim(field);
            } else {
                tooManyFields = true;
            }
            ++fieldCount;
        }

        if (fieldCount != 4 || tooManyFields || trimmedLine.back() == '|') {
            std::cout << "WARNING: line " << lineNumber << " skipped - expected 4 fields "
                      << "(resourceId|name|type|capacity), found " << fieldCount << ".\n";
            ++m_skippedRecords;
            continue;
        }

        int resourceId = 0;
        int capacity   = 0;

        if (!ReservationUtils::parseInt(fields[0], resourceId) || resourceId <= 0) {
            std::cout << "WARNING: line " << lineNumber << " skipped - resource id \""
                      << fields[0] << "\" is not a positive whole number.\n";
            ++m_skippedRecords;
            continue;
        }
        if (fields[1].empty() || fields[2].empty()) {
            std::cout << "WARNING: line " << lineNumber
                      << " skipped - name and type must not be empty.\n";
            ++m_skippedRecords;
            continue;
        }
        if (!ReservationUtils::parseInt(fields[3], capacity) || capacity <= 0) {
            std::cout << "WARNING: line " << lineNumber << " skipped - capacity \""
                      << fields[3] << "\" is not a positive whole number.\n";
            ++m_skippedRecords;
            continue;
        }
        if (resourceExists(resourceId)) {
            std::cout << "WARNING: line " << lineNumber << " skipped - duplicate resource id "
                      << resourceId << ".\n";
            ++m_skippedRecords;
            continue;
        }

        m_resources.push_back(Resource(resourceId, fields[1], fields[2], capacity));
    }

    input.close();

    if (m_resources.empty()) {
        std::cout << "WARNING: \"" << filePath << "\" contained no usable resource records.\n";
    }
    return true;
}

void ResourceManager::displayAllResources() const {
    if (m_resources.empty()) {
        std::cout << "No resources are loaded.\n";
        return;
    }

    std::cout << "\nCampus resources (" << m_resources.size() << " total)\n";
    Resource::displayHeader();
    for (std::size_t i = 0; i < m_resources.size(); ++i) {
        m_resources[i].display();
    }
}

const Resource* ResourceManager::findResourceById(int resourceId) const {
    for (std::size_t i = 0; i < m_resources.size(); ++i) {
        if (m_resources[i].getResourceId() == resourceId) {
            return &m_resources[i];
        }
    }
    return nullptr;
}

bool ResourceManager::resourceExists(int resourceId) const {
    return findResourceById(resourceId) != nullptr;
}

std::size_t ResourceManager::getResourceCount() const { return m_resources.size(); }

int ResourceManager::getSkippedRecordCount() const { return m_skippedRecords; }

const std::vector<Resource>& ResourceManager::getResources() const { return m_resources; }

