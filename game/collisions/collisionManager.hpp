#ifndef _COLLISION_MANAGER_HPP_
#define _COLLISION_MANAGER_HPP_

#include <vector>
#include <deque>
#include <queue>
#include "../../base/math/game_vector.hpp"

class CCollisionPoint;

////////////////////// Collision Struct //////////////////////////////
//	Содержит данные о столкновении двух коллижн объектов
//	points - массив пар точек столкновения и нормалей в этих точках
//	к какой-либо линии из коллижн-объекта two
//////////////////////////////////////////////////////////////////////
struct SCollision {
	CCollisionPoint* one;
	CCollisionPoint* two;
	// точка пересечения, нормаль к линии two
	std::vector<std::pair<vector2d, vector2d>> points;

	SCollision(CCollisionPoint *o, CCollisionPoint *t, std::vector<std::pair<vector2d, vector2d>> pnts): one(o), two(t) {
		points = std::move(pnts);
	}
};


///////////////////////////////////////////// COLLISION MANAGER SINGLETON //////////////////////////////////////////////////////
//
//	Менеджер столкновений. 
//	Позволяет добавлять и удалять коллижн-объекты, обновляет их состояния, определяет
//	взаимодействие в зависимости от владельцев сталкиваемых коллижн-объектов
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CCollisionManager {
public:
	///////////////// Collision Result Struct ////////////////////
	// Содержит результаты столкновения двух отрезков
	//////////////////////////////////////////////////////////////
	struct SCollisionRes {
		bool		result;
		bool		isMovement;
		vector2d	_where;
		SCollisionRes(bool r, bool isMov, const vector2d& wh) : result(r), isMovement(isMov), _where(wh) {};
	};

protected:
	friend class CEngineKernel;

	std::deque<CCollisionPoint*>	_collisionBoxes;			// все активные коллижн-объекты
	std::queue<SCollision>			_frameCollisions;			// найденные коллизии. обрабатываются в каждом кадре

public:
	void add	(CCollisionPoint* cPoint);
	void erase	(CCollisionPoint* cPoint);


	// столкновение двух отрезков
	SCollisionRes lineCollision(const vector2d& start1, const vector2d& end1, const vector2d& start2, const vector2d& end2) const;

	void alreadyTestedReset	();								// сбрасывает флаг "уже протестирован" со всех элементов	
	void test				(CCollisionPoint* cPoint);		// тест коллижн-объекта на столкновение
	void solveCollisions	();								// определение взаимодействия владельцев коллижн-объектов

	// обновляем коллижн манагер. должен обновляться после обновления физики!
	void update();


// singleton realisation
private:
	CCollisionManager	() = default;
	~CCollisionManager	() = default;

public:
	CCollisionManager(const CCollisionManager&) = delete;
	CCollisionManager& operator=(const CCollisionManager&) = delete;

public:
	static CCollisionManager& inst() {
		static CCollisionManager instance;
		return instance;
	}
};

#endif // !_COLLISION_MANAGER_HPP_