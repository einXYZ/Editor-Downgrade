#include <Geode/Geode.hpp>
#include <Geode/modify/ColorSelectPopup.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <alphalaneous.level-storage-api/include/LevelStorageAPI.hpp>
#include "utils.hpp"

using namespace geode::prelude;
using namespace VersionUtils;
using namespace UIUtils;

class $modify(MyColorSelectPopup, ColorSelectPopup) {
    bool init(EffectGameObject* p0, CCArray* p1, ColorAction* p2) {
        if (!ColorSelectPopup::init(p0, p1, p2)) return false;

        auto lel = LevelEditorLayer::get();
        auto versionStr = alpha::level_storage::getSavedValue<std::string>(lel, "target-version");
        auto version = versionStr.empty() ? GDVersion::v2_2 : stringToVersion(versionStr);

        if (version <= GDVersion::v1_9) {
            if (auto bg = typeinfo_cast<CCScale9Sprite*>(this->getChildByIDRecursive("background"))) {
                bg->setColor(ccc3(0, 0, 0));
                bg->setOpacity(100);
                bg->setContentWidth(300);
            }
            if (auto old = this->getChildByIDRecursive("old-color-preview-sprite")) {
                old->setScaleY(1.0f);
                old->setPositionY(255.0f);
            }

            removeIds(this, { "opacity-label", "opacity-slider", "copy-color-label", "copy-color-toggle",
                        "spawn-trigger-label", "spawn-trigger-toggle", "color-id-label", "color-id-input"
                        "color-id-prev-button", "color-id-next-button", "color-id-background",
                        "special-color-select-button", "info-button"});
            if (this->getChildByIDRecursive("tint-ground-label")) {
                removeIds(this, { "blending-label", "blending-toggle" });
            }
            moveAll(this, { "copy-button", "paste-button", "default-button" }, { 45.0f, 0.0f });
            moveAll(this, { "player-color-1-toggle", "player-color-1-label",
                    "new-color-preview-sprite", "old-color-preview-sprite" }, { -55.0f, 0.0f });
            moveAll(this, { "player-color-2-toggle", "player-color-2-label" }, { -152.0f, 38.0f });
            moveAll(this, { "tint-ground-toggle", "tint-ground-label" }, { -262.0f, -40.0f });
            moveAll(this, { "blending-toggle", "blending-label" }, { -55.0f, 45.0f });
        }

        return true;
    }
};