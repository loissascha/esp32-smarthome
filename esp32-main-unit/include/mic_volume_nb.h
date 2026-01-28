#pragma once
#include <Arduino.h>

class MicVolumeNB {
public:
  struct Config {
    uint8_t pin;
    uint16_t sampleRateHz;
    uint16_t windowMs;
    float smoothing;          // 0..1
    uint16_t minUsefulP2P;

    uint16_t noiseFollowUp;
    uint16_t noiseFollowDown;

    Config()
        : pin(35),
          sampleRateHz(8000),
          windowMs(50),
          smoothing(0.2f),
          minUsefulP2P(20),
          noiseFollowUp(2),
          noiseFollowDown(1) {}
  };

  explicit MicVolumeNB(const Config& cfg) : _cfg(cfg) {}

  void begin() {
    pinMode(_cfg.pin, INPUT);

    analogReadResolution(12);
    // If your mic output range seems "compressed", try:
    // analogSetAttenuation(ADC_11db);

    _sampleIntervalUs = 1000000UL / (uint32_t)_cfg.sampleRateHz;
    _windowUs = (uint32_t)_cfg.windowMs * 1000UL;

    uint32_t now = micros();
    _windowStartUs = now;
    _lastSampleUs = now;

    _minV = 4095;
    _maxV = 0;

    _lastP2P = 0;
    _noiseFloor = 0;
    _maxSignal = 200;
    _smoothed = 0.0f;
    _level = 0;
  }

  // Call very often from loop()
  void update() {
    const uint32_t now = micros();

    // Take at most one sample when it's time
    if ((uint32_t)(now - _lastSampleUs) >= _sampleIntervalUs) {
      _lastSampleUs += _sampleIntervalUs; // steady pacing

      uint16_t v = analogRead(_cfg.pin);
      if (v < _minV) _minV = v;
      if (v > _maxV) _maxV = v;
    }

    // Finalize a window when it's over
    if ((uint32_t)(now - _windowStartUs) >= _windowUs) {
      finalizeWindow();
      _windowStartUs += _windowUs;

      _minV = 4095;
      _maxV = 0;
    }
  }

  int level() const { return _level; }
  uint16_t lastPeakToPeak() const { return _lastP2P; }
  uint16_t noiseFloor() const { return _noiseFloor; }

private:
  void finalizeWindow() {
    uint16_t p2p = (_maxV >= _minV) ? (uint16_t)(_maxV - _minV) : 0;
    _lastP2P = p2p;

    if (p2p < _cfg.minUsefulP2P) p2p = 0;

    // Noise floor tracking
    if (_noiseFloor == 0) _noiseFloor = p2p;

    if (p2p > _noiseFloor) {
      uint16_t nf = (uint16_t)(_noiseFloor + _cfg.noiseFollowUp);
      _noiseFloor = (nf > p2p) ? p2p : nf;
    } else {
      if (_noiseFloor > _cfg.noiseFollowDown) _noiseFloor = (uint16_t)(_noiseFloor - _cfg.noiseFollowDown);
      else _noiseFloor = 0;
    }

    int signal = (int)p2p - (int)_noiseFloor;
    if (signal < 0) signal = 0;

    // Adaptive max (fast up, slow down)
    if (signal > _maxSignal) _maxSignal = signal;
    else _maxSignal = (int)(_maxSignal * 0.995f);
    if (_maxSignal < 50) _maxSignal = 50;

    float raw = (float)signal / (float)_maxSignal;
    if (raw > 1.0f) raw = 1.0f;

    _smoothed = _smoothed + (raw - _smoothed) * _cfg.smoothing;

    int lvl = (int)lroundf(_smoothed * 100.0f);
    if (lvl < 0) lvl = 0;
    if (lvl > 100) lvl = 100;
    _level = lvl;
  }

private:
  Config _cfg;

  uint32_t _sampleIntervalUs;
  uint32_t _windowUs;

  uint32_t _windowStartUs;
  uint32_t _lastSampleUs;

  uint16_t _minV;
  uint16_t _maxV;

  uint16_t _lastP2P;
  uint16_t _noiseFloor;
  int _maxSignal;

  float _smoothed;
  int _level;
};

