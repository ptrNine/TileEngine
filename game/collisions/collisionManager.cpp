#include "collisionManager.hpp"

#include <numeric>
#include <cstdio>

#include "collisionPoint.hpp"
#include "collisionLine.hpp"
#include "collisionRectangle.hpp"

#include "../objects/bullet.hpp"
#include "../objects/prop.hpp"


void CCollisionManager::add(CCollisionPoint* cPoint) {
	_collisionBoxes.push_back(cPoint);
}

void CCollisionManager::erase(CCollisionPoint* cPoint) {
	auto findRes = std::find(_collisionBoxes.begin(), _collisionBoxes.end(), cPoint);
	//R_ASSERTF(findRes != _collisionBoxes.end(), "Can't find collision point. Memory corrypted.");
	if (findRes != _collisionBoxes.end())_collisionBoxes.erase(findRes);
}

void CCollisionManager::alreadyTestedReset() {
	for (auto element : _collisionBoxes) {
		element->_alreadyTested = false;
		element->_preCollisionOn = false;
	}
}

CCollisionManager::SCollisionRes CCollisionManager::lineCollision(const vector2d& start1, const vector2d& end1, const vector2d& start2, const vector2d& end2) const {
	//printf("line1: (%f, %f) : (%f, %f)\n", start1.getX(), start1.getY(), end1.getX(), end1.getY());
	//printf("line2: (%f, %f) : (%f, %f)\n", start2.getX(), start2.getY(), end2.getX(), end2.getY());

	// находим длину отрезков
	double line1Magn = (end1 - start1).magnitude();
	double line2Magn = (end2 - start2).magnitude();

	// находим уравнения прямых через векторное произведение в однородных координатах
	vector3d line1Eq = end1.homogeousCross(start1);
	vector3d line2Eq = end2.homogeousCross(start2);

	// находим точку пересечения прямых
	vector2d point = line1Eq.cross(line2Eq).getHomogeneous();

	// результат пересечения
	bool res = false;
	//printf("intersect: (%f, %f)\n", point.getX(), point.getY());

	// проверка для отрезков
	if (((point - start1).magnitude() <= line1Magn) &&
		((point - end1).magnitude() <= line1Magn) &&
		((point - start2).magnitude() <= line2Magn) &&
		((point - end2).magnitude() <= line2Magn))
		res = true;

	return SCollisionRes(res, false, point);
}


// Todo: добавить отбрасывание ограничение зоны коллизий. добавить вычисление нормали и точки столкновения для других видов столкновений

///////////////////////////////////////////////////////////////////////
//
// главный метод колижн теста.
// алгоритм вычисляет пересечения двух отрезков в разных кадрах
//
///////////////////////////////////////////////////////////////////////
void CCollisionManager::test(CCollisionPoint* cPoint) {
	for (auto element : _collisionBoxes) {
		// пропускаем, если это тот же элемент
		if (cPoint == element)
			continue;

		// пропускаем протестированные
		if (element->_alreadyTested)
			continue;

		// игнорируем следующий кадр после столкновения
		if (element->_nextFrameIgnore == cPoint || cPoint->_nextFrameIgnore == element) {
			printf("[%p]:[%p] ignored!\n", cPoint, element);
			continue;
		}

		// сработал пре коллижн, находим точки пересечения
		if (cPoint->_preCollisionRect.intersects(element->_preCollisionRect)) {
			cPoint->_preCollisionOn = true;
			element->_preCollisionOn = true;
		}
		else {
			continue;
		}

		// чекнем соответствие групп
		auto find1 = std::find(cPoint->_allowedGroups.begin(), cPoint->_allowedGroups.end(), element->_group);
		auto find2 = std::find(element->_allowedGroups.begin(), element->_allowedGroups.end(), cPoint->_group);
		if (((find1 == cPoint->_allowedGroups.end()) && (!cPoint->_allowedGroups.empty())) || ((find2 == element->_allowedGroups.end()) && (!element->_allowedGroups.empty())))
			continue;

		// была ли коллизия
		bool onCollision = false;


		
// Todo: доделать вычисление точек и нормали для всех объектов
		std::vector<std::pair<vector2d, vector2d>> collisionPoints; // точка столкновения, нормаль к поверхности ВТОРОГО элемента
		//////////////////////////////////////////////////////////////////
		//
		//		БОЛЬШОЙ СУКА СВИТЧ
		//
		//////////////////////////////////////////////////////////////////
		switch (cPoint->COLLISION_TYPE | element->COLLISION_TYPE) {

		// столкновение точки с точкой, не обрабатываем
		case ECOLLISION_POINT | ECOLLISION_POINT:
			break;

		//////////////////// точка и отрезок //////////////////////////////
		case ECOLLISION_POINT | ECOLLISION_LINE: {
			CCollisionPoint* point	= nullptr;
			CCollisionLine* line	= nullptr;

			// определяем точку и отрезок
			if (cPoint->COLLISION_TYPE == ECOLLISION_POINT) {
				point = cPoint;
				line = static_cast<CCollisionLine*>(element);
			}
			else {
				point = element;
				line = static_cast<CCollisionLine*>(cPoint);
			}
			
			// точки всех примитивов
			vector2d pS			= point->_point;				
			vector2d pE			= pS + point->_displacement;	
			vector2d lp1S		= line->_point;					
			vector2d lp2S		= lp1S + line->_pointTwo;
			vector2d lp1E		= lp1S + line->_displacement;	
			vector2d lp2E		= lp2S + line->_displacement;

			auto res1 = lineCollision(pS, pE, lp1S, lp2S);							// с линией
			auto res2 = lineCollision(pS, pE, lp1E, lp2E);							// с перемещенной линией

			onCollision = res1.result || res2.result;

			// вычисляем точку и нормаль к прямой
			vector2d collisionPoint;
			vector2d lineVector;
			if ((res1._where - pS).magnitude() > (res2._where - pS).magnitude()) {
				collisionPoint = res2._where;
				lineVector = lp1E - lp2E;
			} else {
				collisionPoint = res1._where;
				lineVector = lp1S - lp2S;
			}
			collisionPoints.emplace_back(std::make_pair(collisionPoint, lineVector.normal()));


			if (onCollision)
				_frameCollisions.emplace(point, line, collisionPoints);
		}
			break;

		// точка и прямоугольник
		case ECOLLISION_POINT | ECOLLISION_RECTANGLE: {
			CCollisionPoint * point = nullptr;
			CCollisionRectangle* rect = nullptr;

			// определяем точку и отрезокугол
			if (cPoint->COLLISION_TYPE == ECOLLISION_POINT) {
				point = cPoint;
				rect = static_cast<CCollisionRectangle*>(element);
			}
			else {
				point = element;
				rect = static_cast<CCollisionRectangle*>(cPoint);
			}

			// точки всех примитивов
			vector2d pS = point->_point;
			vector2d pE = pS + point->_displacement;
			vector2d lp1S = rect->_point;
			vector2d lp2S = lp1S + rect->_point2;
			vector2d lp3S = lp1S + rect->_point3;
			vector2d lp4S = lp1S + rect->_point4;


			auto res1 = lineCollision(pS, pE, lp1S, lp2S);		// с линией
			auto res2 = lineCollision(pS, pE, lp2S, lp3S);		// с линией
			auto res3 = lineCollision(pS, pE, lp3S, lp4S);		// с линией
			auto res4 = lineCollision(pS, pE, lp1S, lp4S);		// с линией
			onCollision = res1.result || res2.result || res3.result || res4.result;

			// мутим контейнер векторов сторон и точек пересечения на них
			typedef std::pair<vector2d, vector2d> res_pair;
			
			std::vector<res_pair> resArr;

			if (res1.result) resArr.emplace_back(std::make_pair(res1._where, lp1S - lp2S));
			if (res2.result) resArr.emplace_back(std::make_pair(res2._where, lp2S - lp3S));
			if (res3.result) resArr.emplace_back(std::make_pair(res3._where, lp3S - lp4S));
			if (res4.result) resArr.emplace_back(std::make_pair(res4._where, lp1S - lp4S));


			if (!onCollision && rect->_displacement.getX() != 0.0 && rect->_displacement.getY() != 0.0) {

				vector2d lp1E = lp1S + rect->_displacement;
				vector2d lp2E = lp2S + rect->_displacement;
				vector2d lp3E = lp3S + rect->_displacement;
				vector2d lp4E = lp4S + rect->_displacement;

				auto res5 = lineCollision(pS, pE, lp1E, lp2E);		// с перемещенной линией
				auto res6 = lineCollision(pS, pE, lp2E, lp3E);		// с перемещенной линией
				auto res7 = lineCollision(pS, pE, lp3E, lp4E);		// с перемещенной линией
				auto res8 = lineCollision(pS, pE, lp1E, lp4E);		// с перемещенной линией

				onCollision = onCollision || res5.result || res6.result || res7.result || res8.result;


				// добавляем перемещенные результаты в контейнер
				if (res5.result) resArr.emplace_back(std::make_pair(res5._where, lp1E - lp2E));
				if (res6.result) resArr.emplace_back(std::make_pair(res6._where, lp2E - lp3E));
				if (res7.result) resArr.emplace_back(std::make_pair(res7._where, lp3E - lp4E));
				if (res8.result) resArr.emplace_back(std::make_pair(res8._where, lp1E - lp4E));
			}

			for (size_t i = 0; i < resArr.size(); i++)
				printf("%li: magn = %f\n", i, (pS - resArr[i].first).magnitude());


			// находим ближайшую точку
			if (!resArr.empty()) {
				res_pair nearest = std::accumulate(resArr.begin() + 1, resArr.end(), resArr[0], [&](const res_pair& a, const res_pair& cur) {
					return (a.first - pS).magnitude() > (cur.first - pS).magnitude() ? cur : a;
				});
				printf("nearest: (%f, %f)\n", nearest.second.getX(), nearest.second.getY());
				// завершаем вычисление нормали и точки
				collisionPoints.emplace_back(std::make_pair(nearest.first, nearest.second.normal()));
			}
			
			if (onCollision)
				_frameCollisions.emplace(point, rect, collisionPoints);
		}
			break;
		case ECOLLISION_LINE | ECOLLISION_LINE: {			
			auto* line1 = static_cast<CCollisionLine*>(cPoint);
            auto* line2 = static_cast<CCollisionLine*>(element);
			
			// точки всех примитивов	
			vector2d l1p1S		= line1->_point;					
			vector2d l1p2S		= l1p1S + line1->_pointTwo;
			vector2d l1p1E		= l1p1S + line1->_displacement;	
			vector2d l1p2E		= l1p2S + line1->_displacement;
			vector2d l2p1S		= line2->_point;					
			vector2d l2p2S		= l2p1S + line2->_pointTwo;
			vector2d l2p1E		= l2p1S + line2->_displacement;	
			vector2d l2p2E		= l2p2S + line2->_displacement;

			// пересечение второй линии первой
			auto res1 = lineCollision(l1p1S, l1p1E, l2p1S, l2p2S);				
			auto res2 = lineCollision(l1p2S, l1p2E, l2p1S, l2p2S);

			// пересечение первой линии второй
			auto res3 = lineCollision(l2p1S, l2p1E, l1p1S, l1p2S);
			auto res4 = lineCollision(l2p2S, l2p2E, l1p1S, l1p2S);

			onCollision = res1.result || res2.result || res3.result || res4.result;
		}
			break;
// Todo: может не срабатывать, если линия много больше прямоугольника и перемещается очень быстро
		case ECOLLISION_LINE | ECOLLISION_RECTANGLE: {
			CCollisionLine * line = nullptr;
			CCollisionRectangle* rect = nullptr;

			// определяем точку и отрезок
			if (cPoint->COLLISION_TYPE == ECOLLISION_LINE) {
				line = static_cast<CCollisionLine*>(cPoint);
				rect = static_cast<CCollisionRectangle*>(element);
			}
			else {
				line = static_cast<CCollisionLine*>(line);
				rect = static_cast<CCollisionRectangle*>(cPoint);
			}

			// точки всех примитивов
			vector2d l1p1S = line->_point;
			vector2d l1p2S = l1p1S + line->_pointTwo;
			vector2d l1p1E = l1p1S + line->_displacement;
			vector2d l1p2E = l1p2S + line->_displacement;

			vector2d l2p1S = rect->_point;
			vector2d l2p2S = l2p1S + rect->_point2;
			vector2d l2p3S = l2p1S + rect->_point3;
			vector2d l2p4S = l2p1S + rect->_point4;

			// одна точка и грани прямоугольника
			auto res1 = lineCollision(l1p1S, l1p1E, l2p1S, l2p2S);
			auto res2 = lineCollision(l1p1S, l1p1E, l2p2S, l2p3S);
			auto res3 = lineCollision(l1p1S, l1p1E, l2p3S, l2p4S);
			auto res4 = lineCollision(l1p1S, l1p1E, l2p1S, l2p4S);

			// другая точка и грани прямоугольника
			auto res5 = lineCollision(l1p2S, l1p2E, l2p1S, l2p2S);
			auto res6 = lineCollision(l1p2S, l1p2E, l2p2S, l2p3S);	
			auto res7 = lineCollision(l1p2S, l1p2E, l2p3S, l2p4S);	
			auto res8 = lineCollision(l1p2S, l1p2E, l2p1S, l2p4S);	

			onCollision = res1.result || res2.result || res3.result || res4.result || res5.result || res6.result || res7.result || res8.result;

			// если прямоугольник двигается
			if (!onCollision && rect->_displacement.getX() != 0.0 && rect->_displacement.getY() != 0.0) {

				vector2d l2p1E = l2p1S + rect->_displacement;
				vector2d l2p2E = l2p2S + rect->_displacement;
				vector2d l2p3E = l2p3S + rect->_displacement;
				vector2d l2p4E = l2p4S + rect->_displacement;

				// перемещение каждой точки прямоугольника и линия
				auto res9 = lineCollision(l2p1S, l2p1E, l1p1S, l1p2S);
				auto res10 = lineCollision(l2p1S, l2p1E, l1p1E, l1p2E);

				auto res11 = lineCollision(l2p2S, l2p2E, l1p1S, l1p2S);
				auto res12 = lineCollision(l2p2S, l2p2E, l1p1E, l1p2E);

				auto res13 = lineCollision(l2p3S, l2p3E, l1p1S, l1p2S);
				auto res14 = lineCollision(l2p3S, l2p3E, l1p1E, l1p2E);

				auto res15 = lineCollision(l2p4S, l2p4E, l1p1S, l1p2S);
				auto res16 = lineCollision(l2p4S, l2p4E, l1p1E, l1p2E);

				// перемещение линии и перемещенный прямоугольник
				auto res17 = lineCollision(l1p2S, l1p2E, l2p1E, l2p2E);
				auto res18 = lineCollision(l1p2S, l1p2E, l2p2E, l2p3E);
				auto res19 = lineCollision(l1p2S, l1p2E, l2p3E, l2p4E);
				auto res20 = lineCollision(l1p2S, l1p2E, l2p1E, l2p4E);
				auto res21 = lineCollision(l1p1S, l1p1E, l2p1E, l2p2E);
				auto res22 = lineCollision(l1p1S, l1p1E, l2p2E, l2p3E);
				auto res23 = lineCollision(l1p1S, l1p1E, l2p3E, l2p4E);
				auto res24 = lineCollision(l1p1S, l1p1E, l2p1E, l2p4E);

				onCollision = onCollision || res9.result || res10.result || res11.result || res12.result || res13.result || res14.result || res15.result || res16.result ||
					res17.result || res18.result || res19.result || res20.result || res21.result || res22.result || res23.result || res24.result;
			}
		}

			break;
		case ECOLLISION_RECTANGLE | ECOLLISION_RECTANGLE: {
			auto* rect1 = static_cast<CCollisionRectangle*>(cPoint);
			auto* rect2 = static_cast<CCollisionRectangle*>(element);
			bool is_static = false;
			if (rect1->_displacement.getX() == 0.0 && rect1->_displacement.getY() == 0.0) {
				is_static = true;
				std::swap(rect1, rect2);
			}
			else if (rect2->_displacement.getX() == 0.0 && rect2->_displacement.getY() == 0.0) {
				is_static = true;
			}


			vector2d rect1P[8] = {
				rect1->_point,
				rect1->_point + rect1->_point2,
				rect1->_point + rect1->_point3,
				rect1->_point + rect1->_point4,
				rect1->_displacement,
				rect1->_displacement,
				rect1->_displacement,
				rect1->_displacement
			}; 
			rect1P[4] += rect1P[0];
			rect1P[5] += rect1P[1];
			rect1P[6] += rect1P[2];
			rect1P[7] += rect1P[3];

			vector2d rect2P[8] = {
				rect2->_point,
				rect2->_point + rect2->_point2,
				rect2->_point + rect2->_point3,
				rect2->_point + rect2->_point4,
				rect2->_displacement,
				rect2->_displacement,
				rect2->_displacement,
				rect2->_displacement
			};
			rect2P[4] += rect2P[0];
			rect2P[5] += rect2P[1];
			rect2P[6] += rect2P[2];
			rect2P[7] += rect2P[3];

			// коллизия перемещенного прямоугольника 1 и прямоугольника 2
			for (size_t j = 0; j < 4; ++j) {
				for (size_t i = 0; i < 4; ++i) {
					onCollision = onCollision || lineCollision(rect1P[i + 4], rect1P[4 + (i+1) % 4], rect2P[j], rect2P[(j+1) % 4]).result;
					if (onCollision) break;
				}
				if (onCollision) break;
			}

			// вектора перемещения прямоугольника 1 и прямоугольник 2
			if (!onCollision) {
				for (size_t j = 0; j < 4; ++j) {
					for (size_t i = 0; i < 4; ++i) {
						onCollision = onCollision || lineCollision(rect1P[i], rect1P[4 + i], rect2P[j], rect2P[(j + 1) % 4]).result;
						if (onCollision) break;
					}
					if (onCollision) break;
				}
			}

			if (!is_static && !onCollision) {
				// коллизия перемещенного прямоугольника 1 и перемещенного прямоугольника 2
				for (size_t j = 0; j < 4; ++j) {
					for (size_t i = 0; i < 4; ++i) {
						onCollision = onCollision || lineCollision(rect1P[i + 4], rect1P[4 + (i + 1) % 4], rect2P[4 + j], rect2P[4 + (j + 1) % 4]).result;
						if (onCollision) break;
					}
					if (onCollision) break;
				}

				// вектора перемещения прямоугольника 1 и перемещенный прямоугольник 2
				if (!onCollision) {
					for (size_t j = 0; j < 4; ++j) {
						for (size_t i = 0; i < 4; ++i) {
							onCollision = onCollision || lineCollision(rect1P[i], rect1P[4 + i], rect2P[4 + j], rect2P[4 + (j + 1) % 4]).result;
							if (onCollision) break;
						}
						if (onCollision) break;
					}
				}
			}
		}
			break;
		default:
			break;
		}
	}

	// уже протестировали для всех вариантов
	cPoint->_alreadyTested = true;
}

void CCollisionManager::solveCollisions() {
	// обработка подтвержденных коллизий
	while (!_frameCollisions.empty()) {
		auto collision = _frameCollisions.front();
		_frameCollisions.pop();


		if ((collision.one->_owner->testType(EBullet) && collision.two->_owner->testType(EProp)) || (collision.one->_owner->testType(EProp) && collision.two->_owner->testType(EBullet))) {
			CBullet* bullet = nullptr;
			CProp* prop = nullptr;
			if (collision.one->_owner->testType(EBullet)) {
				bullet = static_cast<CBullet*>(collision.one->_owner);
				prop = static_cast<CProp*>(collision.two->_owner);
			} else {
				bullet = static_cast<CBullet*>(collision.two->_owner);
				prop = static_cast<CProp*>(collision.one->_owner);
			}
			auto bulletDir = bullet->getDir();
			auto normal = collision.points[0].second;
			printf("size = %li normal: (%.3f, %.3f)\n", collision.points.size(), normal.getX(), normal.getY());
			bulletDir = bulletDir - normal * (bulletDir.dot(normal) / normal.dot(normal)) * 2;
			printf("new dir: (%.3f, %.3f)\n", bulletDir.getX(), bulletDir.getY());
			bulletDir.normalize();
			bullet->setDir(bulletDir);
			bullet->setPos(collision.points[0].first);
			printf("bullet with prop!\n");


		}

		// включаем игнор в следующем фрэйме
		collision.one->_nextFrameIgnore = collision.two;
		collision.two->_nextFrameIgnore = collision.one;
		//printf("%p with %p\n", collision.one, collision.two);
	}
}

void CCollisionManager::update() {
	// сбрасываем флаги "уже протестирован" с коллижнов
	alreadyTestedReset();

	// находим коллизии для каждого объекта (n!)
	for (auto& i : _collisionBoxes)
		i->test();

	// сбрасываем игнор (игнор работает только на один фрэйм)
	for (auto& i : _collisionBoxes)
		i->_nextFrameIgnore = nullptr;

	// разрешаем столкновения  и устанавливаем если нужно игнор
	solveCollisions();

}