//////////////////////////////////////////////////////////////////////////////////////
// Module:		fps_counter.hpp
// Created:		15.04.2016
// Modify:		17.08.2018
// Author:		PdrNine
// Required:	timer.hpp
// Description:	Calculate FPS in game cycles
/////////////////////////////////////////////////////////////////////////////////////

#ifndef __FPS_COUNTER__
    #define __FPS_COUNTER__

#include "timer.hpp"

class CFpsCounter{
public:
    double get();

protected:
    zog::CTimer::Timestamp	_framesTime;
    double	_frameMinCont;
    size_t	_frames;
    double	_lastFPS;
    double  _lastFPStwo;
    double	_fps;

private:
    CFpsCounter();
    ~CFpsCounter() = default;
public:
    CFpsCounter(const CFpsCounter&) = delete;
    CFpsCounter& operator= (const CFpsCounter&) = delete;

public:
    static CFpsCounter& _instance() { static CFpsCounter instance; return instance; }

};

namespace zog {
    inline double FPS() { return CFpsCounter::_instance().get(); }
}

#endif //__FPS_COUNTER__