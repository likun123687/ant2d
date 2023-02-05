#pragma once
#include <asset/particle_config_manager/particle_config_manager.h>
#include <utils/content.h>
#include <utils/debug.h>

namespace ant2d {

ParticleConfigManager::ParticleConfigManager()
    : repo_ {}
{
}

void ParticleConfigManager::Load(const std::string& file)
{
    auto it = repo_.find(file);
    if (it != repo_.end()) {
        it->second.cnt += 1;
    } else {
        auto full_path = SharedContent.GetFullPath(file);
        auto [config, type] = LoadFile(full_path);
        if (!config) {
            Error("load confg error {}", full_path);
        }

        std::unique_ptr<CommonConfig> cfg_p { config };
        repo_[file] = PsConfigCount { std::move(cfg_p), 1, type };
    }
}

void ParticleConfigManager::Unload(const std::string& file)
{
    auto it = repo_.find(file);
    if (it != repo_.end()) {
        if (it->second.cnt > 1) {
            it->second.cnt -= 1;
        } else {
            repo_.erase(file);
        }
    }
}

std::tuple<CommonConfig*, uint8_t> ParticleConfigManager::Get(const std::string& file)
{
    auto it = repo_.find(file);
    if (it != repo_.end()) {
        return { it->second.ref.get(), it->second.config_type };
    }
    return { nullptr, 0 };
}

std::tuple<CommonConfig*, uint8_t> ParticleConfigManager::LoadFile(const std::string& file)
{
    auto [config_file_data, config_file_len] = SharedContent.LoadFile(file);
    nlohmann::json config_json = nlohmann::json::parse(config_file_data.get(), config_file_data.get() + config_file_len);

    if (config_json.is_discarded()) {
        Error("parse ps config file {} error", file);
    }

    PsConfig cfg {};
    config_json.get_to<PsConfig>(cfg);

    CommonConfig* config = nullptr;
    uint8_t config_type = 0;
    if (cfg.emitterType == 0) {
        GravityConfig* g = new GravityConfig {};
        g->gravity = math::Vec2 { cfg.gravityx, cfg.gravityy };
        g->speed = Var { cfg.speed, cfg.speedVariance };

        auto ab = math::Radian(cfg.angle);
        auto av = math::Radian(cfg.angleVariance);

        g->angel = Var { ab, av };

        g->radial_acc = Var { cfg.radialAcceleration, cfg.radialAccelVariance };
        g->tangential_acc = Var { cfg.tangentialAcceleration, cfg.tangentialAccelVariance };
        g->rotation_is_dir = cfg.rotationIsDir;

        config = g;
        config_type = kGravityConfigType;
    } else {
        RadiusConfig* r = new RadiusConfig {};
        r->radius = Range {
            Var { cfg.maxRadius, cfg.maxRadiusVariance },
            Var { cfg.minRadius, cfg.minRadiusVariance }
        };

        r->angle = Var { cfg.angle, cfg.angleVariance };
        config = r;
        config_type = kRadiusConfigType;
    }

    // shared properties
    config->max = cfg.maxParticles;
    config->duration = cfg.duration;
    config->life = Var { cfg.particleLifespan, cfg.particleLifespanVariance };
    config->x = Var { cfg.sourcePositionx, cfg.sourcePositionVariancex };
    config->y = Var { cfg.sourcePositiony, cfg.sourcePositionVariancey };

    // size and spin
    config->size = Range {
        Var { cfg.startParticleSize, cfg.startParticleSizeVariance },
        Var { cfg.finishParticleSize, cfg.finishParticleSizeVariance }
    };

    config->rot = Range {
        Var { cfg.rotationStart, cfg.rotationStartVariance },
        Var { cfg.rotationEnd, cfg.rotationEndVariance }
    };
    // color
    config->r = Range {
        Var { cfg.startColorRed, cfg.startColorVarianceRed },
        Var { cfg.finishColorRed, cfg.finishColorVarianceRed }
    };

    config->g = Range {
        Var { cfg.startColorGreen, cfg.startColorVarianceGreen },
        Var { cfg.finishColorGreen, cfg.finishColorVarianceGreen }
    };

    config->b = Range {
        Var { cfg.startColorBlue, cfg.startColorVarianceBlue },
        Var { cfg.finishColorBlue, cfg.finishColorVarianceBlue }
    };
    config->a = Range {
        Var { cfg.startColorAlpha, cfg.startColorVarianceAlpha },
        Var { cfg.finishColorAlpha, cfg.finishColorVarianceAlpha }
    };

    return { config, config_type };
}
} // namespace ant2d
