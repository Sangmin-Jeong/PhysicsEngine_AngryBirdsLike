#pragma once
#ifndef __COLLISION_MANAGER__
#define __COLLISION_MANAGER__

// core libraries
#include <iostream>

#include "GameObject.h"
#include "Ship.h"
#include <glm/gtx/norm.hpp>
#include "SoundManager.h"
#include "PhysicsEngine.h"

class CollisionManager
{
public:
	static bool CircleCircleCheck(PhysicsEngine* object1, PhysicsEngine* object2);
	static int CircleCircleDistance(glm::vec2 circle_centre1, int circle_radius1, glm::vec2 circle_centre2, int circle_radius2);

	static bool LineCircleCheck(Boundary* object1, PhysicsEngine* object2);
	static bool LineCircleCollision(glm::vec2 line_start, glm::vec2 line_end, glm::vec2 circle_center, float circle_radius);

	static int SquaredDistance(glm::vec2 p1, glm::vec2 p2);
	static bool SquaredRadiusCheck(GameObject* object1, GameObject* object2);

	static bool AABBCheck(PhysicsEngine* object1, PhysicsEngine* object2);

	static bool LineLineCheck(glm::vec2 line1_start, glm::vec2 line1_end, glm::vec2 line2_start, glm::vec2 line2_end);

	static bool LineRectCheck(glm::vec2 line_start, glm::vec2 line_end, glm::vec2 rect_start, float rect_width, float rect_height);
	static bool LineRectEdgeCheck(glm::vec2 line_start, glm::vec2 rect_start, float rect_width, float rect_height);


	static int MinSquaredDistanceLineLine(glm::vec2 line1_start, glm::vec2 line1_end, glm::vec2 line2_start, glm::vec2 line2_end);
	

	static int CircleAABBSquaredDistance(glm::vec2 circle_centre, int circle_radius, glm::vec2 box_start, int box_width, int box_height);
	static bool CircleAABBCheck(PhysicsEngine* object1, PhysicsEngine* object2);

	static bool PointRectCheck(glm::vec2 point, glm::vec2 rect_start, float rect_width, float rect_height);

	static bool LOSCheck(Agent* agent, glm::vec2 end_point, const std::vector<DisplayObject*>& objects, DisplayObject* target);

private:
	CollisionManager();
	~CollisionManager();
};

#endif /* defined (__COLLISION_MANAGER__) */