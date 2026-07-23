#pragma once
#include <vector>
#include <algorithm>

inline std::vector<int> colorType5Objects = {
    146, 147, 206, 204, 673, 674,
    144, 205, 145, 459,
    503, 504, 505,
    498, 499, 500, 501,
    719, 721,
    740, 741, 742
};

inline bool usesColorType5(int id) {
    return std::find(colorType5Objects.begin(), colorType5Objects.end(), id) != colorType5Objects.end();
}