#include "fps_counter.hpp"

CFpsCounter::CFpsCounter() {
    _framesTime 	= zog::timer().timestamp();
    _frameMinCont 	= 0.1;
    _frames 		= 1;
    _lastFPS 		= 0.0;
    _lastFPStwo 	= 0.0;
    _fps 			= 0.0;
}

double CFpsCounter::get() {
    auto curTime = zog::timer().timestamp();
    if ((curTime - _framesTime).sec() > _frameMinCont)
    {
        _fps = (_lastFPS + _lastFPStwo + _frames/(curTime - _framesTime).sec())/3.0f;
        _lastFPStwo = _lastFPS;
        _lastFPS = _fps;
        _framesTime = curTime;
        _frames = 1;
    }
    else
    {
        _frames++;
    }
    return _fps;
}
