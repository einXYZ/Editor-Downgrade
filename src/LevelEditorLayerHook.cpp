#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/binding/EffectGameObject.hpp>
#include <alphalaneous.level-storage-api/include/LevelStorageAPI.hpp>
#include <cvolton.level-id-api/include/EditorIDs.hpp>
#include "utils.hpp"
#include "LegacyTriggers.hpp"

using namespace geode::prelude;
using namespace VersionUtils;

class $modify(MyLevelEditorLayer, LevelEditorLayer) {
    bool init(GJGameLevel* level, bool noUI) {
        if (!LevelEditorLayer::init(level, noUI)) return false;

        if (!pendingVersion.empty()) {
            alpha::level_storage::setSavedValue(this, "target-version", pendingVersion);

            auto id = EditorIDs::getID(level, false);
            Mod::get()->setSavedValue(
                fmt::format("target-version-{}", id),
                pendingVersion
            );

            pendingVersion = "";
        }

        auto versionStr = alpha::level_storage::getSavedValue<std::string>(this, "target-version");
        if (!versionStr.empty() && stringToVersion(versionStr) == GDVersion::v1_9) {
            for (auto obj : CCArrayExt<GameObject*>(m_objects)) {
                if (obj->m_objectID != 899) continue;
                if (auto effectObj = typeinfo_cast<EffectGameObject*>(obj))
                    effectObj->customSetup();
            }
        }

        return true;
    }

    GameObject* createObject(int id, CCPoint pos, bool unk) {
        if (id == 899 && pendingLegacyChannel != -1) {
            auto obj = LevelEditorLayer::createObject(899, pos, unk);
            if (auto effectObj = typeinfo_cast<EffectGameObject*>(obj)) {
                effectObj->m_targetColor = pendingLegacyChannel;
                effectObj->customSetup();
            }
            return obj;
        }

        auto versionStr = alpha::level_storage::getSavedValue<std::string>(this, "target-version");
        if (!versionStr.empty() && !isAllowedInVersion(id, stringToVersion(versionStr)))
            return nullptr;

        return LevelEditorLayer::createObject(id, pos, unk);
    }
};
