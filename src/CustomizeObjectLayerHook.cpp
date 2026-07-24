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
            if (auto bg = this->getChildByIDRecursive("alert-bg")) {
                bg->setContentHeight(170.0f);
            }

            if (auto title = typeinfo_cast<CCLabelBMFont*>(this->getChildByIDRecursive("title-label"))) {
                title->setString("Use Color");
                title->setFntFile("bigFont.fnt");
                title->setPositionY(225.0f);
            }

            moveAll(this, { "special-channels-menu", "channels-menu" }, { 0.0f, -25.0f });
            moveAll(this, { "ok-button", "selected-channel-menu" }, { 0.0f, 34.0f });

            updateChannelButtons();
        }

        return true;
    }

    void updateChannelButtons() {
        for (int i = 1; i <= 4; i++) {
            if (auto channel = this->getChildByIDRecursive(fmt::format("channel-{}-button", i))) {
                if (auto sprite = channel->getChildByType<ButtonSprite>(0)) {
                    if (auto label = sprite->getChildByType<CCLabelBMFont>(0)) {
                        label->setString(fmt::format("Col {}", i).c_str());
                    }
                    if (auto bg = sprite->getChildByType<CCScale9Sprite>(0)) {
                        bg->setContentWidth(54.0f);
                    }
                }
                channel->setPositionX(39.0f + 84.0f * (i - 1));
            }
        }
    }

    void onSelectColor(CCObject* sender) {
        CustomizeObjectLayer::onSelectColor(sender);
        updateChannelButtons();
    }
};