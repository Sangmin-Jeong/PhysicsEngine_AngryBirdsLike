#include "CollisionManager.h"
#include "Util.h"
#include <algorithm>



int CollisionManager::SquaredDistance(const glm::vec2 p1, const glm::vec2 p2)
{
	const auto diff_of_xs = static_cast<int>(p2.x - p1.x);
	const auto diff_of_ys = static_cast<int>(p2.y - p1.y);

	return diff_of_xs * diff_of_xs + diff_of_ys * diff_of_ys;
}

bool CollisionManager::SquaredRadiusCheck(GameObject* object1, GameObject* object2)
{
	const glm::vec2 p1 = object1->GetTransform()->position;
	const glm::vec2 p2 = object2->GetTransform()->position;

	if (const int half_heights = static_cast<int>((object1->GetHeight() + object2->GetHeight()) * 0.5); 
		SquaredDistance(p1, p2) < (half_heights * half_heights)) 
	{
		if (!object2->GetRigidBody()->isColliding) 
		{
			object2->GetRigidBody()->isColliding = true;

			switch (object2->GetType()) {
			case GameObjectType::TARGET:
				std::cout << "Collision with Target!" << std::endl;
				SoundManager::Instance().PlaySound("yay", 0);

				break;
			default:

				break;
			}

			return true;
		}
		return false;
	}
	object2->GetRigidBody()->isColliding = false;
	return false;

}

bool CollisionManager::AABBCheck(PhysicsEngine* object1, PhysicsEngine* object2)
{
	// prepare relevant variables
	const glm::vec2 p1 = object1->GetTransform()->position;
	const glm::vec2 p2 = object2->GetTransform()->position;
	const float p1_width = static_cast<float>(object1->GetWidth());
	const float p1_height = static_cast<float>(object1->GetHeight());
	const float p2_width = static_cast<float>(object2->GetWidth());
	const float p2_height = static_cast<float>(object2->GetHeight());

	glm::vec2 p1_center;
	glm::vec2 p2_center;
	if (object1->GetType() == GameObjectType::BLOCK || object1->GetType() == GameObjectType::BLOCK_FIX)
	{
		p1_center = glm::vec2(p1.x + p1_width * 0.5f, p1.y + p1_height * 0.5f);
	}
	else
	{
		p1_center = p1;
	}

	if (object2->GetType() == GameObjectType::BLOCK || object2->GetType() == GameObjectType::BLOCK_FIX)
	{
		p2_center = glm::vec2(p2.x + p2_width * 0.5f, p2.y + p2_height * 0.5f);
	}
	else
	{
		p2_center = p2;
	}


	float displacementX = p2_center.x - p1_center.x;
	float displacementY = p2_center.y - p1_center.y;
	glm::vec2 displacement = p2_center - p1_center;
	glm::vec2 direction = Util::Normalize(displacement);

	glm::vec2 p1Point = glm::vec2(p1_center.x, p1_center.y + p1_height * 0.5f);
	glm::vec2 p1Direction = p1_center - p1Point;
	glm::vec2 p1Normal = Util::Normalize(p1Direction);

	float distanceX = abs(displacementX);
	float distanceY = abs(displacementY);

	// If the value of the overlaps are greater than 0, it means that objects are collided
	float overlapX = (p1_width / 2 + p2_width / 2) - distanceX;
	float overlapY = (p1_height / 2 + p2_height / 2) - distanceY;

	// p = m * v (Momentum = Mass * Velocity)
	glm::vec2 totalMomentum = object2->GetMomentum() + object2->GetMomentum();
	float COR1 = object1->GetMaterialCOR();
	float COR2 = object2->GetMaterialCOR();

	if (overlapY > 0 && overlapX > 0)
	{
		//Move along X and than bounce the object off
		if (overlapX < overlapY)
		{
			object2->GetTransform()->position.x = object2->GetTransform()->position.x + direction.x * overlapX;
			// v = p / m (Velocity = (totalMomentum / COR) / mass) 
			object2->SetVelocity(direction * (totalMomentum * COR2) / object2->GetMass());

			// Bounce for another object, but except Fixed_Block
			if (object1->GetType() != GameObjectType::BLOCK_FIX)
			{
				object1->SetVelocity(-direction * (totalMomentum * COR1) / object1->GetMass());
			}

		}
		//Move along Y and than bounce the object off
		else
		{
			object2->GetTransform()->position.y = object2->GetTransform()->position.y + p1Normal.y * overlapY;
			object2->SetVelocity(direction * (totalMomentum * COR2) / object2->GetMass());

			if (object1->GetType() != GameObjectType::BLOCK_FIX)
			{
				object1->SetVelocity(-direction * (totalMomentum * COR1) / object1->GetMass());
			}
		}

		// Check if object is applied bigger energy than their toughness, if then, object will be destroyed
		if (object2->GetToughness().x <= abs(totalMomentum.x * COR2) || object2->GetToughness().y <= abs(totalMomentum.y * COR2))
		{
			object2->SetOverTough(true);
		}
		if (object1->GetType() != GameObjectType::BLOCK_FIX)
		{
			if (object1->GetToughness().x <= abs(totalMomentum.x * COR1) || object1->GetToughness().y <= abs(totalMomentum.y * COR1))
			{
				object1->SetOverTough(true);
			}
		}
		return true;
	}
	return false;
}

bool CollisionManager::LineLineCheck(const glm::vec2 line1_start, const glm::vec2 line1_end, const glm::vec2 line2_start, const glm::vec2 line2_end)
{
	const auto x1 = line1_start.x;
	const auto x2 = line1_end.x;
	const auto x3 = line2_start.x;
	const auto x4 = line2_end.x;
	const auto y1 = line1_start.y;
	const auto y2 = line1_end.y;
	const auto y3 = line2_start.y;
	const auto y4 = line2_end.y;

	// calculate the distance to intersection point
	const auto u_a = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	const auto u_b = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (u_a >= 0 && u_a <= 1 && u_b >= 0 && u_b <= 1)
	{
		return true;
	}
	return false;
}

bool CollisionManager::LineRectEdgeCheck(const glm::vec2 line_start, const glm::vec2 rect_start, const float rect_width, const float rect_height)
{
	bool state = false;

	const auto x1 = line_start.x;
	const auto y1 = line_start.y;

	const auto rx = rect_start.x;
	const auto ry = rect_start.y;
	const auto rw = rect_width;
	const auto rh = rect_height;

	// configure the left edge
	const auto left_edge_start = glm::vec2(rx, ry);
	const auto left_edge_end = glm::vec2(rx, ry + rh);
	const auto left_edge_mid_point = Util::Lerp(left_edge_start, left_edge_end, 0.5f);

	// configure the right edge
	const auto right_edge_start = glm::vec2(rx + rw, ry);
	const auto right_edge_end = glm::vec2(rx + rw, ry + rh);
	const auto right_edge_mid_point = Util::Lerp(right_edge_start, right_edge_end, 0.5f);

	// configure the top edge
	const auto top_edge_start = glm::vec2(rx, ry);
	const auto top_edge_end = glm::vec2(rx + rw, ry);
	const auto top_edge_mid_point = Util::Lerp(top_edge_start, top_edge_end, 0.5f);

	// configure the bottom edge
	const auto bottom_edge_start = glm::vec2(rx, ry + rh);
	const auto bottom_edge_end = glm::vec2(rx + rw, ry + rh);
	const auto bottom_edge_mid_point = Util::Lerp(bottom_edge_start, bottom_edge_end, 0.5f);

	// line / line comparisons
	const auto left = LineLineCheck(glm::vec2(x1, y1), left_edge_mid_point, left_edge_start, left_edge_end);
	const auto right = LineLineCheck(glm::vec2(x1, y1), right_edge_mid_point, right_edge_start, right_edge_end);
	const auto top = LineLineCheck(glm::vec2(x1, y1), top_edge_mid_point, top_edge_start, top_edge_end);
	const auto bottom = LineLineCheck(glm::vec2(x1, y1), bottom_edge_mid_point, bottom_edge_start, bottom_edge_end);

	// return true if any line collides with the edge
	if (left || right || top || bottom)
	{
		state = true;
	}
	return state;
}

int CollisionManager::MinSquaredDistanceLineLine(glm::vec2 line1_start, glm::vec2 line1_end, glm::vec2 line2_start, glm::vec2 line2_end)
{
	auto u = line1_end - line1_start;
	auto v = line2_end - line2_start;
	auto w = line1_start - line2_start;
	auto a = Util::Dot(u, u);         // always >= 0
	auto b = Util::Dot(u, v);
	auto c = Util::Dot(v, v);         // always >= 0
	auto d = Util::Dot(u, w);
	auto e = Util::Dot(v, w);
	auto D = a * c - b * b;					// always >= 0
	float sc, tc;

	// compute the line parameters of the two closest points
	if (D < Util::EPSILON) {					// the lines are almost parallel
		sc = 0.0;
		tc = (b > c ? d / b : e / c);			// use the largest denominator
	}
	else {
		sc = (b * e - c * d) / D;
		tc = (a * e - b * d) / D;
	}

	// get the difference of the two closest points
	auto d_p = w + (sc * u) - (tc * v);  // =  L1(sc) - L2(tc)

	// return the normal
	return static_cast<int>(Util::Dot(d_p, d_p));
}


int CollisionManager::CircleAABBSquaredDistance(const glm::vec2 circle_centre, int circle_radius, const glm::vec2 box_start, const int box_width, const int box_height)
{
	auto dx = std::max(box_start.x - circle_centre.x, 0.0f);
	dx = std::max(dx, circle_centre.x - (box_start.x + static_cast<float>(box_width)));
	auto dy = std::max(box_start.y - circle_centre.y, 0.0f);
	dy = std::max(dy, circle_centre.y - (box_start.y + static_cast<float>(box_height)));

	return static_cast<int>((dx * dx) + (dy * dy));
}

bool CollisionManager::CircleAABBCheck(PhysicsEngine* aabb, PhysicsEngine* circle)
{
	// prepare relevant variables
	const glm::vec2 p1 = aabb->GetTransform()->position;
	const glm::vec2 p2 = circle->GetTransform()->position;
	const float p1_width = static_cast<float>(aabb->GetWidth());
	const float p1_height = static_cast<float>(aabb->GetHeight());
	const float p2_width = static_cast<float>(circle->GetWidth());
	const float p2_height = static_cast<float>(circle->GetHeight());

	glm::vec2 p1_center;
	glm::vec2 p2_center;
	if (aabb->GetType() == GameObjectType::BLOCK || aabb->GetType() == GameObjectType::BLOCK_FIX)
	{
		p1_center = glm::vec2(p1.x + p1_width * 0.5f, p1.y + p1_height * 0.5f);
	}
	else
	{
		p1_center = p1;
	}

	if (circle->GetType() == GameObjectType::BLOCK || circle->GetType() == GameObjectType::BLOCK_FIX)
	{
		p2_center = glm::vec2(p2.x + p2_width * 0.5f, p2.y + p2_height * 0.5f);
	}
	else
	{
		p2_center = p2;
	}


	glm::vec2 displacement = p2_center - p1_center;
	glm::vec2 direction = Util::Normalize(displacement);

	// Specific point in the direction between Circle and AABB 
	glm::vec2 circleRadius = -direction * (p2_width * 0.5f);
	glm::vec2 circlePoint = p2_center + circleRadius;
	glm::vec2 circleBottom = p2_center + p2_height * 0.5f;

	float displacementX = circlePoint.x - p1_center.x;
	float displacementY;

	// To calculate with correct direction
	if (aabb->GetType() != GameObjectType::BLOCK_FIX)
	{
		displacementY = circlePoint.y - p1_center.y;
	}
	else
	{
		displacementY = circleBottom.y - p1_center.y;
	}

	float distanceX = abs(displacementX);
	float distanceY = abs(displacementY);


	// If the value of the overlaps are greater than 0, it means that objects are collided
	float overlapX = (p1_width * 0.5f) - distanceX;
	float overlapY = (p1_height * 0.5f) - distanceY;

	// p = m * v (Momentum = Mass * Velocity)
	glm::vec2 totalMomentum = aabb->GetMomentum() + circle->GetMomentum();
	float COR1 = aabb->GetMaterialCOR();
	float COR2 = circle->GetMaterialCOR();


	if (overlapY > 0 && overlapX > 0)
	{
		//Move along X and than bounce the object off
		if (overlapX < overlapY)
		{
			circle->GetTransform()->position = circle->GetTransform()->position + direction * overlapX;
			// v = p / m (Velocity = (totalMomentum / COR) / mass) 
			circle->SetVelocity(direction * (totalMomentum * COR2) / circle->GetMass());

			// Bounce for another object, but except Fixed_Block
			if (aabb->GetType() != GameObjectType::BLOCK_FIX)
			{
				aabb->SetVelocity(-direction * (totalMomentum * COR1) / aabb->GetMass());
			}

		}
		//Move along Y and than bounce the object off
		else
		{
			circle->GetTransform()->position = circle->GetTransform()->position + direction * overlapY;
			circle->SetVelocity(direction * (totalMomentum * COR2) / circle->GetMass());

			if (aabb->GetType() != GameObjectType::BLOCK_FIX)
			{
				aabb->SetVelocity(-direction * (totalMomentum * COR1) / aabb->GetMass());
			}
		}

		// Check if object is applied bigger energy than their toughness, if then, object will be destroyed
		if (circle->GetToughness().x <= abs(totalMomentum.x * COR2) || circle->GetToughness().y <= abs(totalMomentum.y * COR2))
		{
			circle->SetOverTough(true);
		}
		if (aabb->GetType() != GameObjectType::BLOCK_FIX)
		{
			if (aabb->GetToughness().x <= abs(totalMomentum.x * COR1) || aabb->GetToughness().y <= abs(totalMomentum.y * COR1))
			{
				aabb->SetOverTough(true);
			}
		}

		return true;
	}
	return false;

}

int CollisionManager::CircleCircleDistance(glm::vec2 circle_centre1, int circle_radius1, glm::vec2 circle_centre2, int circle_radius2)
{
	auto dx = std::max(circle_centre2.x - circle_centre1.x, 0.0f);
	dx = std::max(dx, circle_centre1.x - circle_centre2.x);
	auto dy = std::max(circle_centre2.y - circle_centre1.y, 0.0f);
	dy = std::max(dy, circle_centre1.y - circle_centre2.x);
	return static_cast<int>((dx * dx) + (dy * dy));
}

bool CollisionManager::CircleCircleCheck(PhysicsEngine* object1, PhysicsEngine* object2)
{

	const auto circle_centre1 = object1->GetTransform()->position;
	const auto circle_radius1 = static_cast<int>(std::max(object1->GetWidth() * 0.5f, object1->GetHeight() * 0.5f));
	const auto circle_centre2 = object2->GetTransform()->position;
	const auto circle_radius2 = static_cast<int>(std::max(object2->GetWidth() * 0.5f, object2->GetHeight() * 0.5f));

	glm::vec2 displacement = circle_centre2 - circle_centre1;
	glm::vec2 direction = Util::Normalize(displacement);

	// p = m * v (Momentum = Mass * Velocity)
	glm::vec2 totalMomentum = object1->GetMomentum() + object2->GetMomentum();
	float COR1 = object1->GetMaterialCOR();
	float COR2 = object2->GetMaterialCOR();

	float distnace = Util::Distance(circle_centre2, circle_centre1);
	float overlap = (circle_radius1 + circle_radius2) - distnace;


	if (distnace <= (circle_radius1 + circle_radius2))
	{
		object1->GetTransform()->position = object1->GetTransform()->position + -direction * overlap;
		object2->GetTransform()->position = object2->GetTransform()->position + direction * overlap;

		if (object2->GetToughness().x <= abs(totalMomentum.x * COR2) || object2->GetToughness().y <= abs(totalMomentum.y * COR2))
		{
			object2->SetOverTough(true);
		}
		if (object1->GetToughness().x <= abs(totalMomentum.x * COR1) || object1->GetToughness().y <= abs(totalMomentum.y * COR1))
		{
			object1->SetOverTough(true);
		}
		
		// v = p / m (Velocity = (totalMomentum / 2) / mass) 
		object1->SetVelocity(-direction * (totalMomentum * COR1) / object1->GetMass());
		object2->SetVelocity(direction * (totalMomentum * COR2) / object2->GetMass());

		return true;
	}

	return false;
	

}

bool CollisionManager::LineCircleCheck(Boundary* object1, PhysicsEngine* object2)
{
	const auto line_start = object1->GetStartPoint();
	//const auto line_end = object1->GetEndPoint();
	const auto line_normalPoint = object1->GetEndPoint() - line_start;
	
	const auto circle_centre1 = object2->GetTransform()->position;
	const auto circle_radius1 = static_cast<int>(std::max(object2->GetWidth() * 0.5f, object2->GetHeight() * 0.5f));

	// Relative vector
	glm::vec2 displacement = circle_centre1 - line_start;
	// Distance between projectile center and plane's normal vector
	float distance = Util::Dot(displacement, line_normalPoint);
	// How much overlaped
	float overlap = circle_radius1 - distance;

	if (overlap > 0)
	{
		object2->overlap = overlap;
		// Translate Projectile's position to where is not collided
		object2->GetTransform()->position = object2->GetTransform()->position + glm::vec2(overlap * object1->GetNormal().x, overlap * object1->GetNormal().y);
		// Stop moving
		/*object2->SetVelocity(glm::vec2(0,0));*/
		//cout << "Collided" << endl;
		object2->SetIsCollided(true);
		return true;
	}
	object2->SetIsCollided(false);
	return false;
}

bool CollisionManager::LineCircleCollision(glm::vec2 line_start, glm::vec2 line_end, glm::vec2 circle_center, float circle_radius)
{
	const auto x1 = line_start.x;
	const auto x2 = line_end.x;
	const auto y1 = line_start.y;
	const auto y2 = line_end.y;
	const auto cx = circle_center.x;
	const auto cy = circle_center.y;
	const auto cr = circle_radius;

	// check if the line has hit any of the rectangle's sides
	// uses the Line/Line function below
	const auto left = LineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(cx, cy), glm::vec2(cx, cy + cr));
	const auto right = LineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(cx + cr, cy), glm::vec2(cx + cr, cy + cr));
	const auto top = LineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(cx, cy), glm::vec2(cx + cr, cy));
	const auto bottom = LineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(cx, cy + cr), glm::vec2(cx + cr, cy + cr));

	// if ANY of the above are true, the line
	// has hit the rectangle
	if (left || right || top || bottom)
	{
		return true;
	}

	return false;
}

bool CollisionManager::LineRectCheck(const glm::vec2 line_start, const glm::vec2 line_end, const glm::vec2 rect_start, const float rect_width, const float rect_height)
{
	const auto x1 = line_start.x;
	const auto x2 = line_end.x;
	const auto y1 = line_start.y;
	const auto y2 = line_end.y;
	const auto rx = rect_start.x;
	const auto ry = rect_start.y;
	const auto rw = rect_width;
	const auto rh = rect_height;

	// check if the line has hit any of the rectangle's sides
	// uses the Line/Line function below
	const auto left = LineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry), glm::vec2(rx, ry + rh));
	const auto right = LineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx + rw, ry), glm::vec2(rx + rw, ry + rh));
	const auto top = LineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry), glm::vec2(rx + rw, ry));
	const auto bottom = LineLineCheck(glm::vec2(x1, y1), glm::vec2(x2, y2), glm::vec2(rx, ry + rh), glm::vec2(rx + rw, ry + rh));

	// if ANY of the above are true, the line
	// has hit the rectangle
	if (left || right || top || bottom)
	{
		return true;
	}

	return false;
}

bool CollisionManager::PointRectCheck(const glm::vec2 point, const glm::vec2 rect_start, const float rect_width, const float rect_height)
{
	const float top_left_x = rect_start.x - rect_width * 0.5f;
	const float top_left_y = rect_start.y - rect_height * 0.5f;
	const auto width = rect_width;
	const auto height = rect_height;

	if (point.x > top_left_x &&
		point.x < top_left_x + width &&
		point.y > top_left_y &&
		point.y < top_left_y + height)
	{
		return true;
	}
	return false;
}

// assumptions - the list of objects are stored so that they are facing the target and the target is loaded last
bool CollisionManager::LOSCheck(Agent* agent, const glm::vec2 end_point, const std::vector<DisplayObject*>& objects, DisplayObject* target)
{
	const auto start_point = agent->GetTransform()->position;

	for (const auto object : objects)
	{
		const auto width = static_cast<float>(object->GetWidth());
		const auto height = static_cast<float>(object->GetHeight());
		auto object_offset = glm::vec2(width * 0.5f, height * 0.5f);
		const auto rect_start = object->GetTransform()->position - object_offset;
		

		switch (object->GetType())
		{
		case GameObjectType::OBSTACLE:
			if (LineRectCheck(start_point, end_point, rect_start, width, height))
			{
				return false;
			}
			break;
		case GameObjectType::TARGET:
		{
			switch (agent->GetType())
			{
			case GameObjectType::AGENT:
				if (LineRectCheck(start_point, end_point, rect_start, width, height))
				{
					return true;
				}
				break;
			case GameObjectType::PATH_NODE:
				if (LineRectEdgeCheck(start_point, rect_start, width, height))
				{
					return true;
				}
				break;
			default:
				//error
				std::cout << "ERROR: " << static_cast<int>(agent->GetType()) << std::endl;
				break;
			}
		}
		break;
		default:
			//error
			std::cout << "ERROR: " << static_cast<int>(object->GetType()) << std::endl;
			break;
		}

	}

	// if the line does not collide with an object that is the target then LOS is false
	return false;
}


CollisionManager::CollisionManager()
= default;


CollisionManager::~CollisionManager()
= default;
