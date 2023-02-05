#pragma once
#include <string>
#include <utils/json.h>

namespace ant2d {

struct PsConfig {
    std::string configName;

    int maxParticles;
    float angle;
    float angleVariance;
    float duration;

    // blend-func - not support, now

    // color
    float startColorRed;
    float startColorGreen;
    float startColorBlue;
    float startColorAlpha;

    float startColorVarianceRed;
    float startColorVarianceGreen;
    float startColorVarianceBlue;
    float startColorVarianceAlpha;

    float finishColorRed;
    float finishColorGreen;
    float finishColorBlue;
    float finishColorAlpha;

    float finishColorVarianceRed;
    float finishColorVarianceGreen;
    float finishColorVarianceBlue;
    float finishColorVarianceAlpha;

    // size
    float startParticleSize;
    float startParticleSizeVariance;
    float finishParticleSize;
    float finishParticleSizeVariance;

    // Position
    float sourcePositionx;
    float sourcePositiony;

    float sourcePositionVariancex;
    float sourcePositionVariancey;

    // Spinning
    float rotationStart;
    float rotationStartVariance;
    float rotationEnd;
    float rotationEndVariance;

    // life and emission rate
    float particleLifespan;
    float particleLifespanVariance;

    // mode
    int emitterType;

    ///////// ModeA

    // gravity
    float gravityx;
    float gravityy;

    // speed
    float speed;
    float speedVariance;

    // radial acceleration
    float radialAcceleration;
    float radialAccelVariance;

    // tangential acceleration
    float tangentialAcceleration;
    float tangentialAccelVariance;

    // rotation is dir
    bool rotationIsDir;

    ////////// ModeB

    // radius
    float maxRadius;
    float maxRadiusVariance;

    float minRadius;
    float minRadiusVariance;

    // rotate
    float rotatePerSecond;
    float rotatePerSecondVariance;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PsConfig, configName, maxParticles, angle, angleVariance, duration, startColorRed, startColorGreen,
    startColorBlue, startColorAlpha, startColorVarianceRed, startColorVarianceGreen, startColorVarianceBlue,
    startColorVarianceAlpha, finishColorRed, finishColorGreen, finishColorBlue, finishColorAlpha,
    finishColorVarianceRed, finishColorVarianceGreen, finishColorVarianceBlue, finishColorVarianceAlpha,
    startParticleSize, startParticleSizeVariance, finishParticleSize, finishParticleSizeVariance,
    sourcePositionx, sourcePositiony, sourcePositionVariancex, sourcePositionVariancey,
    rotationStart, rotationStartVariance, rotationEnd, rotationEndVariance,
    particleLifespan, particleLifespanVariance, emitterType, gravityx, gravityy,
    speed, speedVariance, radialAcceleration, radialAccelVariance, tangentialAcceleration,
    tangentialAccelVariance, rotationIsDir, maxRadius, maxRadiusVariance, minRadius, minRadiusVariance,
    rotatePerSecond, rotatePerSecondVariance)

} // namespace ant2d
