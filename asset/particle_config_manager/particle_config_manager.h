#pragma once
#include <unordered_map>
#include <asset/particle_config_manager/config.h>
#include <effect/config.h>
#include <effect/sim_gravity.h>
#include <effect/sim_radius.h>

namespace ant2d {
class ParticleConfigManager {
    struct PsConfigCount {
        std::unique_ptr<CommonConfig> ref;
        uint32_t cnt;
        uint8_t config_type;
    };

private:
    std::unordered_map<std::string, PsConfigCount> repo_;
    std::tuple<CommonConfig*, uint8_t> LoadFile(const std::string& file);

public:
    static constexpr uint8_t kGravityConfigType = 0;
    static constexpr uint8_t kRadiusConfigType = 1;

    ParticleConfigManager();

    void Load(const std::string& file);
    void Unload(const std::string& file);
    std::tuple<CommonConfig*, uint8_t> Get(const std::string& file);
};
} // namespace ant2d
