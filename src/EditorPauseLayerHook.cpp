#include <Geode/Geode.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <alphalaneous.level-storage-api/include/LevelStorageAPI.hpp>
#include "utils.hpp"

using namespace geode::prelude;
using namespace VersionUtils;
using namespace UIUtils;


class $modify(MyEditorPauseLayer, EditorPauseLayer) {
    bool init(LevelEditorLayer* layer) {
        if (!EditorPauseLayer::init(layer)) return false;

        auto versionStr = alpha::level_storage::getSavedValue<std::string>(layer, "target-version");
        auto version = versionStr.empty() ? GDVersion::v2_2 : stringToVersion(versionStr);

        if (version <= GDVersion::v1_9) {
            removeIds(this, { "hide-invisible-toggle", "hide-invisible-label", "preview-particles-toggle", "preview-particles-label",
                        "preview-shaders-toggle", "preview-shaders-label", "channel-custom-button", "base-hsv-button",
                        "next-free-button", "detail-tab-button", "settings-button" });
            moveAll(this, { "preview-mode-toggle", "preview-mode-label",
                        "preview-animations-toggle", "preview-animations-label"}, { 0.0f, -47.95f });
            moveAll(this, { "show-hitboxes-toggle", "show-hitboxes-label" }, { 0.0f, -71.925f });
            removeIds(this, { "build-helper-button", "copy-color-button", "paste-color-button", "create-extras-button",
                        "unlock-layers-button", "reset-unused-button" });
            move(this, "keys-button", { 0.0f, -174.0f });

            removeIds(this, { "regroup-button", "create-loop-button", "new-groupx-button", "new-groupy-button" });
            moveAll(this, { "select-all-right-button", "select-all-left-button", "select-all-button",
                        "align-x-button", "align-y-button" }, { 0.0f, -58.0f });
        }

        return true;
    }

    void onResume(CCObject* sender) {
        EditorPauseLayer::onResume(sender);
        removeIds(m_editorLayer->m_editorUI, { "link-button", "unlink-button", "enable-link-button" });
    }
};