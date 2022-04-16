#include <FPS.h>

namespace game {
const int32_t MaxScriptSize = 1024;
const int32_t MaxSpriteSize = 64 << 10;
const int32_t MaxTransformSize = 64 << 10;
const int32_t MaxTextSize = 64 << 10;
const int32_t MaxMeshSize = 64 << 10;
const int32_t MaxParticleSize = 1024;

struct Options {
    int32_t W;
    int32_t H;
}

class DB {
private:
    EntityManager entityM_;
    std::vector<std::unique_ptr<Table>> tables_;
};

class AppState
{
public:
    typedef std::function<void(bool)> StateChangeCallback;
    StateChangeCallback pauseCallback_;
    StateChangeCallback focusCallback_;

private:
    class State
    {
    public:
        bool paused_;
        bool lostFocus_;
    };

    State old_;
    State now_;
}

void AppState::setPaused(bool paused)
{
    old_.paused_ = now_.paused_;
    now_.paused_ = paused_;
}

void AppState::setFocused(bool focused)
{
    old_.lostFocus_ = now_.lostFocus_;
    now_.lostFocus_ = !focused_;
}

type Game struct {
	Options; FPS; DB

	// scene manager
	SceneManager

	// system
	*gfx.RenderSystem
	*input.InputSystem
	*effect.ParticleSimulateSystem
	*ScriptSystem
	*anim.AnimationSystem

	// game state
	appState
}

class Game {

private:
    Options options_;
    FPS fps_;
    DB db_;
    SceneManager scenemanager_;
}


}
