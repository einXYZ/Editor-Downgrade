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
        auto version = versionStr.empty() ? GDVersion::v2_2 : stringToVersion(versionStr);

        if (version == GDVersion::v1_9) {
            for (auto obj : CCArrayExt<GameObject*>(m_objects)) {
                if (obj->m_objectID != 899) continue;
                if (auto effectObj = typeinfo_cast<EffectGameObject*>(obj))
                    effectObj->customSetup();
            }
        }

        applyLevelSettings(version);

        return true;
    }

    void applyLevelSettings(GDVersion version) {
        if (alpha::level_storage::getSavedValue<bool>(this, "legacy-settings-applied"))
            return;

        auto settings = m_levelSettings;
        if (version < GDVersion::v2_2) { 
            settings->m_allowMultiRotation = false;
            settings->m_allowStaticRotate = false;
            settings->m_fixGravityBug = false;  // I especially hate this setting
            settings->m_fixRobotJump = false;
            settings->m_sortGroups = false;
            settings->m_enable22Changes = false;
            settings->m_enablePlayerSqueeze = false;
            settings->m_fixNegativeScale = false;
            settings->m_dynamicLevelHeight = false;
            settings->m_fixRadiusCollision = false;
            settings->m_reverseSync = false;
            settings->m_decreaseBoostSlide = false;
            settings->m_enableImpulseFix = false;
        }

        alpha::level_storage::setSavedValue(this, "legacy-settings-applied", true);
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