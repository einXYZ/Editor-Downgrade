#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <alphalaneous.level-storage-api/include/LevelStorageAPI.hpp>
#include <cvolton.level-id-api/include/EditorIDs.hpp>

using namespace geode::prelude;

class $modify(MyLevelCell, LevelCell) {
    void loadFromLevel(GJGameLevel* level) {
        LevelCell::loadFromLevel(level);

        auto id = EditorIDs::getID(level, false);
        auto version = Mod::get()->getSavedValue<std::string>(
            fmt::format("target-version-{}", id),
            ""
        );

        if (version.empty()) return;

        auto label = CCLabelBMFont::create(version.c_str(), "bigFont.fnt");
        label->setScale(0.3f);
        label->setAnchorPoint({1.f, 1.f});
        label->setPosition(346.0f, 65.0f);
        label->setColor(ccc3(255, 220, 100));
        label->setID("version-label"_spr);
        m_mainLayer->addChild(label, 100);
    }
};