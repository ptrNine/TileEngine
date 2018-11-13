#ifndef _GAMEBASE_HPP_
#define _GAMEBASE_HPP_
#include <deque>
#include "base/math/game_vector.hpp"
#include "base/new_types/sharedString.hpp"


class CEngineState {
public:
	double	timestep;


//	singleton realization
private:
	CEngineState(): timestep(0){};
	~CEngineState() = default;

public:
	CEngineState(const CEngineState&) = delete;
	CEngineState& operator=(const CEngineState&) = delete;

public:
	static CEngineState& inst() {
		static CEngineState instance;
		return instance;
	}
};


namespace sf { class RenderWindow; }
class CPhysicObject;
class CChunkArea;

class CEngineKernel {
public:
	explicit CEngineKernel(sf::RenderWindow* window);
	~CEngineKernel();

	void load_map(const shared_str& map_name);
	void update(const vector2d& viewpoint, double timestep) const;
	void draw(const vector2d& viewpoint) const;

	void addObject		(CPhysicObject* object);		// adding object to map
	void destroyObject	(CPhysicObject* object);		// deleting object from map


	double getPixelsToMeters() const{ return _placeToTile; }

	U32 getCameraXFactor	()				{ return _cameraXFactor; }
	void setCameraXFactor	(U32 factor);

	void setDrawCollisions	(bool drawCollisions) { _drawCollisions = drawCollisions; }


protected:

	mutable sf::RenderWindow*		_window;
	CChunkArea*					_onlineChunks;
	U32								_cameraXFactor;  // CNF: how much chunk tiles can be placed in screen width
	mutable double					_placeToTile;	// screen_width/_cameraXFactor: place in pixels to one tile (1 tile = 1 meter) 

	std::deque<CPhysicObject*>		_objects;		// all game objects


	bool							_drawCollisions;


public:

	// info
protected:
	mutable U32						_renderedObjects;

public:
	U32								infoObjectsInView() { return _renderedObjects; }
};

#endif //_GAMEBASE_HPP_