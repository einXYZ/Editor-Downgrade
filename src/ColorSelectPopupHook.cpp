#include <Geode/Geode.hpp>
#include <Geode/modify/ColorSelectPopup.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <alphalaneous.level-storage-api/include/LevelStorageAPI.hpp>

using namespace geode::prelude;

class $modify(MyColorSelectPopup, ColorSelectPopup) {
    bool init(EffectGameObject* p0, CCArray* p1, ColorAction* p2) {
        if (!ColorSelectPopup::init(p0, p1, p2)) return false;

        auto lel = LevelEditorLayer::get();
        if (!lel) return true;

        auto version = alpha::level_storage::getSavedValue<std::string>(lel, "target-version");
        if (version.empty()) return true;

        removeFeatures();

        return true;
    }

    void removeFeatures() {
    }
};