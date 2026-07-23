#include <Geode/Geode.hpp>
#include <Geode/modify/EffectGameObject.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <alphalaneous.level-storage-api/include/LevelStorageAPI.hpp>
#include "LegacyTriggers.hpp"
#include "utils.hpp"

using namespace geode::prelude;
using namespace VersionUtils;

class $modify(MyEffectGameObject, EffectGameObject) {
    void customSetup() {
        EffectGameObject::customSetup();

        switch (m_objectID) {
            case 29:
                setIcon("edit_eTintBGBtn_001.png"_spr);
                return;
            case 30:
                setIcon("edit_eTintGBtn_001.png"_spr);
                return;
            case 105:
                setIcon("edit_eTintObjBtn_001.png"_spr);
                return;
            case 744:
                setIcon("edit_eTint3DLBtn_001.png"_spr);
                return;
            case 915: 
                setIcon("edit_eTintLBtn_001.png"_spr);
                return;
        }

        if (m_objectID != 899) return;

        auto lel = LevelEditorLayer::get();
        if (!lel) return;

        auto versionStr = alpha::level_storage::getSavedValue<std::string>(lel, "target-version");
        if (versionStr.empty() || stringToVersion(versionStr) != GDVersion::v1_9) return;

        auto texture = getLegacyColorTexture(m_targetColor);
        if (!texture.empty()) setIcon(texture);
        removeLegacyLabels();
    }

    void setIcon(std::string texture) {
        if (CCSprite* newSpr = CCSprite::create(texture.c_str())) {
            m_addToNodeContainer = true;
            setTexture(newSpr->getTexture());
            setTextureRect(newSpr->getTextureRect());
        }
    }

    void removeLegacyLabels() {
        if (!m_pChildren) return;
        for (auto node : getChildrenExt<CCLabelBMFont*>()) {
            node->setVisible(false);
        }
    }
};