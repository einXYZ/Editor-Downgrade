#include <Geode/Geode.hpp>
#include <Geode/modify/SetGroupIDLayer.hpp>
#include <alphalaneous.level-storage-api/include/LevelStorageAPI.hpp>
#include "utils.hpp"

using namespace geode::prelude;
using namespace VersionUtils;
using namespace UIUtils;


class $modify(MySetGroupIDLayer, SetGroupIDLayer) {
    bool init(GameObject* obj, CCArray* objs) {
        if (!SetGroupIDLayer::init(obj, objs)) return false;

        auto lel = LevelEditorLayer::get();
        auto versionStr = alpha::level_storage::getSavedValue<std::string>(lel, "target-version");
        auto version = versionStr.empty() ? GDVersion::v2_2 : stringToVersion(versionStr);

        if (version <= GDVersion::v1_9) {
            removeIds(this, { "groups-bg", "z-layer-label", "draw-order-label", "editor-layer-next-free-button",
                        "editor-layer-2-menu", "z-order-menu", "add-group-id-menu", "channel-order-menu",
                        "channel-menu", "groups-list-menu", "next-free-menu", "add-group-id-buttons-menu",
                        "z-layer-menu", "actions-menu" });
            this->setOpacity(63);
            auto bg = this->getChildByIDRecursive("background");
            bg->setContentSize({ 160.0f, 120.0f });
            auto prev = this->getChildByIDRecursive("editor-layer-prev-button");
            prev->setRotation(-90.0f);
            auto next = this->getChildByIDRecursive("editor-layer-next-button");
            next->setRotation(90.0f);
            next->setScale(-1.0f, 1.0f);
            auto menu = this->getChildByIDRecursive("editor-layer-menu");
            menu->setAnchorPoint({ 1.0f, 1.0f });
            menu->setPosition(bg->getPosition() + CCPoint(bg->getContentSize().width / 2, bg->getContentSize().height / 2));
        }

        return true;
    }
};