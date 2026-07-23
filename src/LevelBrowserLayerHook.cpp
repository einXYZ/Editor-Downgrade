#include <Geode/Geode.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include "VersionPopup.hpp"
#include "utils.hpp"

using namespace geode::prelude;
using namespace VersionUtils;

class $modify(MyLevelBrowserLayer, LevelBrowserLayer) {
    struct Fields {
        CCObject* originalTarget = nullptr;
        SEL_MenuHandler originalSelector = nullptr;
    };

    bool init(GJSearchObject* obj) {
        if (!LevelBrowserLayer::init(obj)) return false;

        if (auto btn = getChildByIDRecursive("new-level-button")) {
            auto item = static_cast<CCMenuItemSpriteExtra*>(btn);
            m_fields->originalTarget = item->m_pListener;
            m_fields->originalSelector = item->m_pfnSelector;
            item->setTarget(this, menu_selector(MyLevelBrowserLayer::onNewLevelButton));
        }

        return true;
    }

    void onNewLevelButton(CCObject* sender) {
        auto originalTarget = m_fields->originalTarget;
        auto originalSelector = m_fields->originalSelector;

        VersionSelectPopup::create([sender, originalTarget, originalSelector](std::string version) {
            pendingVersion = version;
            (originalTarget->*originalSelector)(sender);
        })->show();
    }
};