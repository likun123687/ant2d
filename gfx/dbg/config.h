#pragma once
#include <cstdint>
namespace ant2d {
namespace dbg {
inline constexpr uint16_t FPS = 1 << 0;
inline constexpr uint16_t Stats = 1 << 1;
inline constexpr uint16_t Draw = 1 << 2;
inline constexpr uint16_t ALL = FPS | Stats | Draw;
inline constexpr uint16_t None = 0;

inline constexpr int32_t MAX_ZORDER = int32_t(0xFFFF >> 1);

extern uint16_t DEBUG;
} // namespace dbg
} // namespace ant2d