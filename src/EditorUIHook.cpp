#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/binding/EditButtonBar.hpp>
#include <Geode/binding/GameObject.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <alphalaneous.level-storage-api/include/LevelStorageAPI.hpp>
#include <algorithm>
#include <cctype>
#include "data.hpp"
#include "ColorType5Objects.hpp"
#include "LegacyTriggers.hpp"
#include "utils.hpp"

using namespace geode::prelude;
using namespace VersionUtils;
using namespace UIUtils;

struct TrackedButton {
    CCMenuItemSpriteExtra* button;
    int id;
    bool legacy;
};

class $modify(MyEditorUI, EditorUI) {
    struct Fields {
        GDVersion version;
        std::vector<EditButtonBar*> bars;
        std::vector<TrackedButton> allButtons;
        CCMenu* tabsMenu = nullptr;
        std::vector<CCMenuItemToggler*> tabs;
        int selectedTab = 0;
        int selectedLegacyId = -1;
    };

    bool init(LevelEditorLayer* lel) {
        if (!EditorUI::init(lel)) return false;

        auto versionStr = alpha::level_storage::getSavedValue<std::string>(this, "target-version");
        m_fields->version = versionStr.empty() ? GDVersion::v2_2 : stringToVersion(versionStr);

        auto config = getVersionConfig(m_fields->version);
        if (!config || config->categories.empty()) return true;

        buildTabs();
        buildBars(config);
        selectTab(0);
        enforceHidden(10);
        applyButtonVisibility(m_fields->version);
        applyButtonSprites(m_fields->version);

        return true;
    }

    void setupCreateMenu() {
        EditorUI::setupCreateMenu();

        auto config = getVersionConfig(m_fields->version);
        if (!config || config->categories.empty()) return;

        enforceHidden(10);
    }

    void enforceHidden(int retries) {
        if (m_tabsMenu) m_tabsMenu->setScale(0);
        for (auto bar : CCArrayExt<CCNode*>(m_createButtonBars))
            bar->setScale(0);

        if (retries <= 0) return;
        Loader::get()->queueInMainThread([this, retries] {
            enforceHidden(retries - 1);
        });
    }

    void buildTabs() {
        auto tabsMenu = CCMenu::create();
        tabsMenu->setLayout(RowLayout::create()->setGap(2.f)->setCrossAxisLineAlignment(AxisAlignment::Start));
        tabsMenu->setPosition(m_tabsMenu->getPosition());
        tabsMenu->setZOrder(100);
        tabsMenu->setAnchorPoint({ 0.5f, 0.0f });
        m_fields->tabsMenu = tabsMenu;

        static const std::vector<int> icons = { 1, 40, 467, 681, 8, 506, 36, 18, 41, 50, 88, 29 };

        for (size_t i = 0; i < icons.size(); i++) {
            auto off = CCSprite::createWithSpriteFrameName("GJ_tabOff_001.png");
            auto on = CCSprite::createWithSpriteFrameName("GJ_tabOn_001.png");
            auto toggle = CCMenuItemToggler::create(off, on, nullptr, nullptr);
            toggle->setTarget(this, menu_selector(MyEditorUI::onChangeTab));

            CCSprite* iconOff;
            CCSprite* iconOn;

            if (icons[i] == -1 || !GameObject::createWithKey(icons[i])) {
                iconOff = CCSprite::createWithSpriteFrameName("sawblade_02_001.png");
                iconOn = CCSprite::createWithSpriteFrameName("sawblade_02_001.png");
            } else {
                iconOff = GameObject::createWithKey(icons[i]);
                iconOn = GameObject::createWithKey(icons[i]);
            }

            iconOff->setPosition(off->getContentSize() / 2);
            iconOff->setPositionY(7.5f);
            limitNodeSize(iconOff, ccp(25, 14), 0.55f, 0);
            off->addChild(iconOff);

            iconOn->setPosition(on->getContentSize() / 2);
            iconOn->setPositionY(7.5f);
            iconOn->setColor(ccc3(127, 127, 127));
            limitNodeSize(iconOn, ccp(25, 14), 0.55f, 0);
            on->addChild(iconOn);

            toggle->setTag((int)i);
            tabsMenu->addChild(toggle);
            m_fields->tabs.push_back(toggle);
        }

        tabsMenu->updateLayout();
        this->addChild(tabsMenu);
    }

    void buildBars(VersionConfig* config) {
        auto winSize = CCDirector::get()->getWinSize();

        for (size_t i = 0; i < config->categories.size(); i++) {
            auto objs = CCArray::create();

            for (int id : config->categories[i]) {
                if (id == 0) {
                    objs->addObject(CCNode::create());
                    continue;
                }

                if (isLegacyColorTrigger(id)) {
                    auto btn = this->getCreateBtn(899, 4);
                    btn->setTag(899);
                    btn->setID(std::to_string(id));
                    btn->setTarget(this, menu_selector(EditorUI::onCreateButton));

                    auto btnSpr = static_cast<ButtonSprite*>(btn->getNormalImage());
                    if (auto go = btnSpr->getChildByType<GameObject>(0)) {
                        if (CCSprite* newSpr = CCSprite::create(getLegacyColorTexture(getLegacyColorChannel(id)).c_str())) {
                            go->m_addToNodeContainer = true;
                            go->setTexture(newSpr->getTexture());
                            go->setTextureRect(newSpr->getTextureRect());
                        }
                    }

                    objs->addObject(btn);
                    m_fields->allButtons.push_back({ btn, id, true });
                    continue;
                }

                if (!GameObject::createWithKey(id)) {
                    objs->addObject(CCNode::create());
                    continue;
                }

                auto btn = this->getCreateBtn(id, usesColorType5(id) ? 5 : 4);
                btn->setTarget(this, menu_selector(EditorUI::onCreateButton));
                btn->setTag(id);
                objs->addObject(btn);
                m_fields->allButtons.push_back({ btn, id, false });
            }

            auto bar = EditButtonBar::create(objs, ccp(winSize.width / 2 - 5, 86), 69, false, 6, 2);
            this->addChild(bar, 5 + (int)i);
            m_fields->bars.push_back(bar);
        }
    }

    void onCreateButton(CCObject* sender) {
        auto node = static_cast<CCNode*>(sender);
        auto idStr = node->getID();

        bool numeric = !idStr.empty() && std::all_of(idStr.begin(), idStr.end(), ::isdigit);

        if (numeric) {
            int fakeId = utils::numFromString<int>(idStr).unwrapOr(0);
            if (isLegacyColorTrigger(fakeId)) {
                bool switchingFromDifferentLegacy = m_selectedObjectIndex == 899 && m_fields->selectedLegacyId != fakeId;

                pendingLegacyChannel = getLegacyColorChannel(fakeId);
                m_fields->selectedLegacyId = fakeId;

                if (switchingFromDifferentLegacy)
                    EditorUI::onCreateButton(sender);

                EditorUI::onCreateButton(sender);
                return;
            }
        }

        pendingLegacyChannel = -1;
        m_fields->selectedLegacyId = -1;
        EditorUI::onCreateButton(sender);
    }

    void updateCreateMenu(bool p0) {
        EditorUI::updateCreateMenu(p0);

        for (auto& tracked : m_fields->allButtons) {
            bool selected = tracked.legacy
                ? (m_selectedObjectIndex == 899 && tracked.id == m_fields->selectedLegacyId)
                : (tracked.id == m_selectedObjectIndex);

            auto color = selected ? ccc3(127, 127, 127) : ccWHITE;
            auto btnSpr = static_cast<ButtonSprite*>(tracked.button->getNormalImage());
            btnSpr->m_subBGSprite->setColor(color);

            if (auto go = btnSpr->getChildByType<GameObject>(0)) 
                go->setColor(color);
        }
    }

    void onChangeTab(CCObject* sender) {
        selectTab(sender->getTag());
    }

    void selectTab(int index) {
        if (m_fields->tabs.size() <= 1) return;

        m_fields->selectedTab = index;

        for (size_t i = 0; i < m_fields->tabs.size(); i++) {
            m_fields->tabs[i]->toggle(i == index);
            m_fields->tabs[i]->setEnabled(i != index);
            m_fields->tabs[i]->m_offButton->setScale(1);
            m_fields->tabs[i]->m_onButton->setScale(1);
            m_fields->tabs[i]->m_offButton->stopAllActions();
            m_fields->tabs[i]->m_onButton->stopAllActions();
        }

        updateCustomTabs();
    }

    void applyButtonVisibility(GDVersion version) {
        if (version <= GDVersion::v1_9) {
            removeIds(this, {
                "edit-special-button",
                "copy-values-button",
                "paste-state-button",
                "go-to-layer-button",
                "paste-color-button",
                "hsv-button",
                "unlink-button",
                "link-button",
                "enable-link-button"
            });
        }
    }

    void applyButtonSprites(GDVersion version) {
        if (version <= GDVersion::v1_9) {
            newSprite(this, "copy-button", "GJ_copyBtn_001.png", 0.8f, {31.f, 0.f});
            newSprite(this, "paste-button", "GJ_pasteBtn_001.png", 0.8f, {-4.5f, -40.f});
            newSprite(this, "deselect-button", "GJ_deSelBtn_001.png", 0.8f, {0.f, 26.5f});
            newSprite(this, "copy-paste-button", "GJ_duplicateObjectBtn_001.png", 0.8f);
            newSprite(this, "edit-group-button", "GJ_groupIDBtn_001.png", 0.8f, {-4.5f, -44.5f});
            newSprite(this, "edit-object-button", "GJ_editObjBtn_001.png"_spr, 0.8f, {0.f, -4.5f});
        }
    }

    void updateCustomTabs() {
        if (!m_fields->tabsMenu) return;

        for (size_t i = 0; i < m_fields->bars.size(); i++)
            m_fields->bars[i]->setVisible(m_tabsMenu->isVisible() && (int)i == m_fields->selectedTab);

        m_fields->tabsMenu->setVisible(m_tabsMenu->isVisible());
    }

    void resetUI() {
        EditorUI::resetUI();

        auto config = getVersionConfig(m_fields->version);
        if (config && !config->categories.empty())
            enforceHidden(10);

        updateCustomTabs();
    }

    void onPause(CCObject* sender) {
        EditorUI::onPause(sender);
        applyButtonVisibility(m_fields->version);
    }

    void updateButtons() {
        EditorUI::updateButtons();
        applyButtonVisibility(m_fields->version);
    }
};