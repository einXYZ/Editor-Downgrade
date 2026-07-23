#pragma once
#include <Geode/Geode.hpp>
#include "data.hpp"

using namespace geode::prelude;

namespace UIUtils {
    void removeId(CCNode* root, const std::string& id);
    void removeIds(CCNode* root, const std::vector<std::string>& ids);
    void move(CCNode* root, const std::string& id, CCPoint offset);
    void moveAll(CCNode* root, const std::vector<std::string>& ids, CCPoint offset);
    void newSprite(CCNode* root, const std::string& id, const std::string& frameName,
                   float scale = 1.0f, CCPoint offset = CCPointZero, bool enabled = false);
}

namespace VersionUtils {
    extern std::string pendingVersion;

    GDVersion stringToVersion(const std::string& str);
    std::string versionToString(GDVersion version);
    bool isAllowedInVersion(int objectID, GDVersion version);
}