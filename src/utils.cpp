#include "utils.hpp"
#include <algorithm>

namespace UIUtils {
    void removeId(CCNode* root, const std::string& id) {
        if (auto node = root->getChildByIDRecursive(id)) {
            // node->removeFromParent();
            node->setVisible(false);
            if (auto item = typeinfo_cast<CCMenuItem*>(node))
                item->setEnabled(false);
        }
    }

    void removeIds(CCNode* root, const std::vector<std::string>& ids) {
        for (auto& id : ids) {
            removeId(root, id);
        }
    }

    void move(CCNode* root, const std::string& id, CCPoint offset) {
        auto node = root->getChildByIDRecursive(id);
        if (!node) return;

        node->setPosition(node->getPosition() + offset);
    }

    void moveAll(CCNode* root, const std::vector<std::string>& ids, CCPoint offset) {
        for (const auto& id : ids)
            move(root, id, offset);
    }

    void newSprite(CCNode* root, const std::string& id, const std::string& frameName,
                   float scale, CCPoint offset, bool enabled) {
        if (frameName.empty()) return;

        auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(root->getChildByIDRecursive(id));
        if (!btn) return;

        bool isModAsset = frameName.starts_with(Mod::get()->getID() + "/");

        auto newSpr = isModAsset
            ? CCSprite::create(frameName.c_str())
            : CCSprite::createWithSpriteFrameName(frameName.c_str());

        if (newSpr) {
            newSpr->setScale(scale);
            btn->setNormalImage(newSpr);
            if (offset != CCPointZero) move(root, id, offset);
            if (!enabled) {
                btn->setColor({166, 166, 166});
                btn->setOpacity(175);
            }
        }
    }
}

namespace VersionUtils {
    std::string pendingVersion = "";

    GDVersion stringToVersion(const std::string& str) {
        if (str == "1.9") return GDVersion::v1_9;
        if (str == "2.2") return GDVersion::v2_2;
        return GDVersion::v2_2;
    }

    std::string versionToString(GDVersion version) {
        switch (version) {
            case GDVersion::v1_9: return "1.9";
            case GDVersion::v2_2: return "2.2";
        }
        return "2.2";
    }

    bool isAllowedInVersion(int objectID, GDVersion version) {
        auto config = getVersionConfig(version);
        if (!config || config->categories.empty()) return true;

        for (auto& category : config->categories) {
            if (std::find(category.begin(), category.end(), objectID) != category.end())
                return true;
        }
        return false;
    }
}