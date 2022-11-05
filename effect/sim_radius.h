#include <effect/pool.h>
#include <effect/rate_controller.h>
#include <effect/life_controller.h>
#include <effect/visual_controller.h>
#include <effect/config.h>

namespace ant2d {

// RadiusConfig used to configure the RadiusSimulator.
struct RadiusConfig : public CommonConfig {
    Range radius;
    Var angle;
    Var angle_delta;
};

// RadiusSimulator works as the radius mode of the Cocos2D's particle-system.
class RadiusSimulator : public Simulator {

private:
    Pool pool_;
    RateController rate_controller_;
    LifeController life_controller_;
    VisualController visual_controller_;

    Channel_v2 pose_start_;
    Channel_v4 color_delta_;
    Channel_f32 size_delta_;
    Channel_f32 rot_;
    Channel_f32 rot_delta_;

	Channel_f32 angle_;
	Channel_f32 angle_delta_;
	Channel_f32 radius_; 
	Channel_f32 radius_delta_;

    RadiusConfig config_;

    void NewParticle(int new_particle);

public:
    RadiusSimulator(RadiusConfig cfg);

    void Initialize();

    void Simulate(float dt);

    std::tuple<int, int> Size();
    void Visualize(std::vector<PosTexColorVertex>& buf, ITexture2D* tex);
};

} // namespace ant2d
