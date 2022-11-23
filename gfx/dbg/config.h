#pragma once
#include <cstdint>
namespace dbg {
inline constexpr uint16_t FPS = 1 << 0;
inline constexpr uint16_t Stats = 1 << 1;
inline constexpr uint16_t Draw = 1 << 2;
inline constexpr uint16_t ALL = FPS | Stats | Draw;
inline constexpr uint16_t None = 0;

extern uint16_t DEBUG;
} // namespace dbg