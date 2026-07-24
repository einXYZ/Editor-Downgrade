#include <Geode/Geode.hpp>
#include <Geode/modify/CustomizeObjectLayer.hpp>
#include <alphalaneous.level-storage-api/include/LevelStorageAPI.hpp>
#include "utils.hpp"

using namespace geode::prelude;
using namespace VersionUtils;
using namespace UIUtils;


class $modify(MyCustomizeObjectLayer, CustomizeObjectLayer) {
    bool init(GameObject* object, CCArray* objects) {
        if (!CustomizeObjectLayer::init(object, objects)) return false;

        auto lel = LevelEditorLayer::get();
        auto versionStr = alpha::level_storage::getSavedValue<std::string>(lel, "target-version");
        auto version = versionStr.empty() ? GDVersion::v2_2 : stringToVersion(versionStr);

        if (version <= GDVersion::v1_9) {
            removeIds(this, { "live-color-select-button", "channel-5-button", "channel-6-button", "channel-7-button",
                        "channel-8-button", "channel-9-button", "channel-custom-button", "base-hsv-menu",
                        "next-free-menu", "tabs-menu", "settings-button", "info-menu", "browse-menu", "copy-paste-menu" });
        }

        return true;
    }
};