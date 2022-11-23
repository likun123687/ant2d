namespace debug
{
    enum class DebugEnum:uint32_t
    {
        FPS = 1,
        Stats = 2,
        Draw = 3,
        ALL = FPS|Stats|Draw,
        None = 0,
    };

    constexpr int32_t MAX_ZORDER = int32_t(0xFFFF>>1)
    class Debug
    {
        private:
        struct Screen
        {
            float w;
            float h;
        };
        Screen screen_;


    };
} // namespace debug
