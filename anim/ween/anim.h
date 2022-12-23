#pragma once
#include <functional>
#include <math/ease/elastic.h>
#include <math/ease/ease.h>
#include <anim/ween/animator.h>

namespace ant2d {
namespace ween {
using UpdateCallback = std::function<void(bool, float)>;
using EndCallback = std::function<void(bool)>;
struct Callback {
    UpdateCallback update_callback;
    EndCallback end_callback;
};

struct Value {
    float f;
};

using AnimState = uint8_t;
constexpr AnimState Waiting = 0;
constexpr AnimState Running = 1;
constexpr AnimState Stopped = 2;
constexpr AnimState Dispose = 3;

using LoopType = uint8_t;
constexpr LoopType Restart = 0;
constexpr LoopType PingPong = 1;

constexpr int RepeatOnce = 1;
constexpr int RepeatInfinite = -1;

// 维护动画的状态数据
// 底层动画系统，使用float作为单位 0-1
struct Animation {
    int index;
    float clock;
    float duration;
    int iteration;
    int repeat_count;
    math::ease::Function interpolator;
    LoopType loop_type;
    struct
    {
        AnimState anim_state;
        bool dirty;
    } state;
    bool reverse;
    float delay;

    void Reset();
    // 动画核心算法
    float Animate(float dt_);
};

struct TweenEngine {
    std::vector<Animation> anims;
    std::vector<Value> values;
    std::vector<Callback> callbacks;

    float time;
    float scale;
    int active;
    int cap;
    std::unordered_map<int, int> loopup;
    int unique_id;

    TweenEngine();

    int New();
    Animator NewAnimator();

    void Delete(int index_);

    void SetTimeScale(float sk_);
    void Update(float dt_);
    void Forward(int index_);
    void Reverse(int index_);

    // Stops running this animation.
    void Stop(int index_);

    // Duration is the length of time this animation should last.
    void SetDuration(int index_, float d_);

    // Repeat the animation. If playback type is forward, restart the animation
    // from start, if the playback type is backward or ping-pong,
    void SetRepeat(int index_, int count_, LoopType loop_);
    void SetFunction(int index_, math::ease::Function fn_);
    void SetUpdateCallback(int index_, UpdateCallback cb_);
    void SetCompleteCallback(int index_, EndCallback cb_);
    float GetValue(int index_);
    float GetDuration(int index_);
};

}
} // namespace ant2d
