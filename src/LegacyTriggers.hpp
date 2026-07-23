#pragma once
#include <vector>
#include <utility>
#include <string>

inline std::vector<std::pair<int, int>> legacyColorTriggers = {
    { 221, 1 },
    { 717, 2 },
    { 718, 3 },
    { 743, 4 }
};

inline bool isLegacyColorTrigger(int id) {
    for (auto& t : legacyColorTriggers)
        if (t.first == id) return true;
    return false;
}

inline int getLegacyColorChannel(int fakeId) {
    for (auto& t : legacyColorTriggers)
        if (t.first == fakeId) return t.second;
    return 0;
}

inline std::string getLegacyColorTexture(int channel) {
    switch (channel) {
        case 1: return "edit_eTintCol01Btn_001.png"_spr;
        case 2: return "edit_eTintCol02Btn_001.png"_spr;
        case 3: return "edit_eTintCol03Btn_001.png"_spr;
        case 4: return "edit_eTintCol04Btn_001.png"_spr;
    }
    return "";
}

inline int pendingLegacyChannel = -1;