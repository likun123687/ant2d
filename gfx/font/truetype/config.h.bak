#pragma once
#include <vector>
#include <memory>
#include <gfx/font/glpy.h>

namespace ant2d {
enum class DirectionType {
    LeftToRight, // E.g.: Latin
    RightToLeft, // E.g.: Arabic
    TopToBottom, // E.g.: Old-Chinese
};

// TTF config based on range
class TTFConfig {
public:
    TTFConfig(uint32_t low, uint32_t high, int size)
    {
        if (low == high && low == 0) {
            low = 32;
            high = 127;
        }

        auto runes = std::vector<Rune>(high - low - +1);
        for (auto i = low, j = high; i <= j; i++) {
            runes[i - low] = Rune(i);
        }
    }

    TTFConfig(std::vector<Rune> runes)
        : runes_(std::move(runes))
    {
    }

    int FontSize()
    {
        return size_;
    }

    std::vector<Rune>& Runes()
    {
        return runes_;
    }

    DirectionType Direction()
    {
        return DirectionType::LeftToRight;
    }

private:
    std::vector<Rune> runes_;

    int size_;
};

} // namespace ant2d
