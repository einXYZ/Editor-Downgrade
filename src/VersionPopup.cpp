#include "VersionPopup.hpp"

using namespace geode::prelude;

VersionSelectPopup* VersionSelectPopup::create(std::function<void(std::string)> callback) {
    auto ret = new VersionSelectPopup();
    if (ret && ret->init()) {
        ret->m_callback = callback;
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool VersionSelectPopup::init() {
    if (!Popup::init(280.f, 160.f)) return false;
    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);

    auto panel = CCLayerColor::create({ 0, 0, 0, 0 });
    panel->setContentSize({ 280.f, 160.f });
    this->m_mainLayer->addChildAtPosition(panel, Anchor::BottomLeft);

    auto menu = CCMenu::create();
    menu->setPosition({ 0, 0 });
    panel->addChild(menu);

    auto title = CCLabelBMFont::create("Select Version", "bigFont.fnt");
    title->setPosition({ 140.f, 140.f });
    title->setScale(0.6f);
    panel->addChild(title);

    m_versionInput = TextInput::create(120.f, "Version", "bigFont.fnt");
    m_versionInput->setPosition({ 140.f, 88.f });
    m_versionInput->setString(m_versions[m_selectedIndex].c_str());
    m_versionInput->setEnabled(false);
    panel->addChild(m_versionInput);

    auto leftSpr = CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png");
    auto leftBtn = CCMenuItemSpriteExtra::create(leftSpr, this, menu_selector(VersionSelectPopup::onLeft));
    leftBtn->setPosition({ 55.f, 88.f });
    menu->addChild(leftBtn);

    auto rightSpr = CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");
    auto rightBtn = CCMenuItemSpriteExtra::create(rightSpr, this, menu_selector(VersionSelectPopup::onRight));
    rightBtn->setPosition({ 225.f, 88.f });
    menu->addChild(rightBtn);

    auto cancelBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Cancel"),
        this,
        menu_selector(VersionSelectPopup::onCancel)
    );
    cancelBtn->setPosition({ 85.f, 28.f });
    menu->addChild(cancelBtn);

    auto okBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("New"),
        this,
        menu_selector(VersionSelectPopup::onOK)
    );
    okBtn->setPosition({ 195.f, 28.f });
    menu->addChild(okBtn);

    return true;
}

void VersionSelectPopup::onLeft(CCObject*) {
    m_selectedIndex = (m_selectedIndex - 1 + (int)m_versions.size()) % (int)m_versions.size();
    m_versionInput->setString(m_versions[m_selectedIndex].c_str());
}

void VersionSelectPopup::onRight(CCObject*) {
    m_selectedIndex = (m_selectedIndex + 1) % (int)m_versions.size();
    m_versionInput->setString(m_versions[m_selectedIndex].c_str());
}

void VersionSelectPopup::onOK(CCObject*) {
    if (m_callback) m_callback(m_versions[m_selectedIndex]);
    this->removeFromParentAndCleanup(true);
}

void VersionSelectPopup::onCancel(CCObject*) {
    this->removeFromParentAndCleanup(true);
}

void VersionSelectPopup::keyBackClicked() {
    onCancel(nullptr);
}