#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/TextInput.hpp>
#include <functional>

using namespace geode::prelude;

class VersionSelectPopup : public geode::Popup {
public:
    std::function<void(std::string)> m_callback;

    static VersionSelectPopup* create(std::function<void(std::string)> callback);
    bool init() override;

private:
    std::vector<std::string> m_versions = {"2.2", "1.9"};
    int m_selectedIndex = 0;
    TextInput* m_versionInput = nullptr;

    void onLeft(CCObject*);
    void onRight(CCObject*);
    void onOK(CCObject*);
    void onCancel(CCObject*);
    void keyBackClicked() override;
};