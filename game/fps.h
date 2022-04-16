#include <chrono>

namespace game{
    class FPS {
        public:

    private:
        std::chrono::system_clock::time_point startTime_;
        std::chrono::system_clock::time_point preTime_;
        float dt_;
        float scale_;
        float realdt_;
        int32_t fps_;
        bool pause_;
    };
}

