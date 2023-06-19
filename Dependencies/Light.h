#pragma once
#include <cstdint>
#include <cstdio>
#include <BlueBlur.h>
class Light
{
public:
    
};

struct LightRGBA {
    float r;
    float g;
    float b;
    float a;
    float scalar = 1.0f;
};

class SpawnableLight {
    Hedgehog::Math::CVector _pos;
    LightRGBA _rgba = { 0.0f, 0.0f, 0.0f, 0.0f };
    float _range = 0.0f;
    bool _enabled = true;
    hh::base::CRefPtr<Sonic::CLocalLight> _light;
    float _fadeOutSpeed = 0.0f;
    float _fadeInSpeed = 0.0f;
    float _currentFadeSpeed = 0.0f;
    float _desiredAlpha = 0.0f;
    float _lifeTime = 0.0f;
    float _currentLifeTime = -1;
    float _startDelay = 0.0f;
    float _currentStartDelay = -1;

    float lerp(float a, float b, float f)
    {
        return a * (1.0 - f) + (b * f);
    }

public:

    LightRGBA CorrectedColor(LightRGBA rgba) {
        LightRGBA correct = rgba;
        correct.r *= correct.a * correct.scalar;
        correct.g *= correct.a * correct.scalar;
        correct.b *= correct.a * correct.scalar;
        return correct;
    }

    Hedgehog::Math::CVector4 ColorToVector(LightRGBA rgba) {
        return { rgba.r, rgba.g, rgba.b, rgba.a };
    }

    LightRGBA& GetColor() {
        return _rgba;
    }

    void SetColor(LightRGBA rgba) {
        _rgba = CorrectedColor(rgba);
    }

    Hedgehog::Math::CVector playerLocalPos(float x, float y, float z) {
        Sonic::Player::CPlayerSpeedContext* sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();

        Hedgehog::Math::CVector playerRight = sonic->m_spMatrixNode->m_Transform.m_Rotation * Hedgehog::Math::CVector::UnitX();
        Hedgehog::Math::CVector playerUp = sonic->m_spMatrixNode->m_Transform.m_Rotation * Hedgehog::Math::CVector::UnitY();
        Hedgehog::Math::CVector playerForward = sonic->m_spMatrixNode->m_Transform.m_Rotation * Hedgehog::Math::CVector::UnitZ();

        Hedgehog::Math::CVector offsetVector = playerRight * x +
            playerUp * y +
            playerForward * z;

        return sonic->m_spMatrixNode->m_Transform.m_Position + offsetVector;
    }

    Hedgehog::Math::CVector& GetPosition() {
        return _pos;
    }

    void SetPosition(Hedgehog::Math::CVector pos) {
        _pos = pos;
    }

    float& GetRange() {
        return _range;
    }

    void SetRange(float r) {
        _range = r;
    }

    bool& GetEnabled() {
        return _enabled;
    }

    void SetEnabled(bool e) {
        _enabled = e;
    }

    float& GetLifeTime() {
        return _lifeTime;
    }

    void SetLifeTime(float f) {
        _lifeTime = f;
    }

    void SetFadeSpeed(float in, float out) {
        _fadeInSpeed = in;
        _fadeOutSpeed = out;
    }

    float& GetStartDelay() {
        return _startDelay;
    }

    void SetStartDelay(float f) {
        _startDelay = f;
    }

    void Spawn() {
        if (_light) { _light = nullptr; }
        Hedgehog::Base::TSynchronizedPtr gameDoc = Sonic::CGameDocument::GetInstance();
        _light = gameDoc->m_pMember->m_spLightManager->AddLocalLight(_pos, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, _range });
        _currentLifeTime = _lifeTime;
        _currentFadeSpeed = _fadeInSpeed;
        _desiredAlpha = 1.0f;
        _rgba.a = 0.0f;
        _currentStartDelay = 0;
    }

    void Despawn() {
        _light = nullptr;
    }

    void Update(float deltaTime) {
        if (!_light) { return; }
        if (_enabled) {
            if (_currentStartDelay != -1) {
                _currentStartDelay += deltaTime;
                if (_currentStartDelay >= _startDelay) {
                    _currentStartDelay = -1;
                }
            }
            else {
                if (_currentLifeTime != -1) {
                    _currentLifeTime -= deltaTime;
                    if (_currentLifeTime <= 0) {
                        _currentLifeTime = -1;
                        _currentFadeSpeed = _fadeOutSpeed;
                        _desiredAlpha = 0.0f;
                    }
                }
                _rgba.a = lerp(_rgba.a, _desiredAlpha, deltaTime * _currentFadeSpeed);
                _light->m_spLight->m_Color = ColorToVector(CorrectedColor(_rgba));
                _light->SetPosition(_pos);
            }
        }
        else {
            Despawn();
        }
    }

    SpawnableLight(){

    }
};
