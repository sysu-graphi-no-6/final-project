#include "PhysicsEngine.h"

PhysicsEngine* PhysicsEngine::instance = NULL;

bool PhysicsEngine::HorizontalCollisionDetect(glm::vec3 currentPos, glm::vec3 nextPos) {

	int axBiggerthanPx = 0;
	int azBiggerthanPz = 0;
	if (nextPos.x < currentPos.x) {
		axBiggerthanPx = -1;
	}
	else if (nextPos.x > currentPos.x) {
		axBiggerthanPx = 1;
	}
	if (nextPos.z < currentPos.z) {
		azBiggerthanPz = -1;
	}
	else if (nextPos.z > currentPos.z) {
		azBiggerthanPz = 1;
	}
	int y_round = round(currentPos.y);
	int y_min = y_round - 1;

	// 进行8邻域水平检测
	/*
		* * *
		*   *
		* * *
	*/
	if (axBiggerthanPx == -1 && azBiggerthanPz == -1) {
		if (m[y_round][round(currentPos.x) - 1][round(currentPos.z) - 1] == true) {
			return false;
		}
		if (m[y_min][round(currentPos.x) - 1][round(currentPos.z) - 1] == true) {
			return false;
		}
	}
	else if (axBiggerthanPx == -1 && azBiggerthanPz == 0) {
		if (m[y_round][round(currentPos.x) - 1][round(currentPos.z)] == true) {
			return false;
		}
		if (m[y_min][round(currentPos.x) - 1][round(currentPos.z)] == true) {
			return false;
		}
	}
	else if (axBiggerthanPx == -1 && azBiggerthanPz == 1) {
		if (m[y_round][round(currentPos.x) - 1][round(currentPos.z) + 1] == true) {
			return false;
		}
		if (m[y_min][round(currentPos.x) - 1][round(currentPos.z) + 1] == true) {
			return false;
		}
	}
	else if (axBiggerthanPx == 0 && azBiggerthanPz == -1) {
		if (m[y_round][round(currentPos.x)][round(currentPos.z) - 1] == true) {
			return false;
		}
		if (m[y_min][round(currentPos.x)][round(currentPos.z) - 1] == true) {
			return false;
		}
	}
	else if (axBiggerthanPx == 0 && azBiggerthanPz == 1) {
		if (m[y_round][round(currentPos.x)][round(currentPos.z) + 1] == true) {
			return false;
		}
		if (m[y_min][round(currentPos.x)][round(currentPos.z) + 1] == true) {
			return false;
		}
	}
	else if (axBiggerthanPx == 1 && azBiggerthanPz == -1) {
		if (m[y_round][round(currentPos.x) + 1][round(currentPos.z) - 1] == true) {
			return false;
		}
		if (m[y_min][round(currentPos.x) + 1][round(currentPos.z) - 1] == true) {
			return false;
		}
	}
	else if (axBiggerthanPx == 1 && azBiggerthanPz == 0) {
		if (m[y_round][round(currentPos.x) + 1][round(currentPos.z)] == true) {
			return false;
		}
		if (m[y_min][round(currentPos.x) + 1][round(currentPos.z)] == true) {
			return false;
		}
	}
	else if (axBiggerthanPx == 1 && azBiggerthanPz == 1) {
		if (m[y_round][round(currentPos.x) + 1][round(currentPos.z) + 1] == true) {
			return false;
		}
		if (m[y_min][round(currentPos.x) + 1][round(currentPos.z) + 1] == true) {
			return false;
		}
	}
	return true;
}
bool PhysicsEngine::UpVerticalCollisionDetect(glm::vec3 pos) {
    if (m[round(pos.y + 0.5f)][round(pos.x)][round(pos.z)] == true) {
        return false;
    }
    return true;
}
// 弹跳时的检测
bool PhysicsEngine::DownVerticalCollisionDetect(glm::vec3 pos) {
    if (m[round(pos.y - 1.5f)][round(pos.x)][round(pos.z)] == true) {
        return false;
    }
    return true;
}

// 走时的检测
bool PhysicsEngine::WalkingVerticalCollisionDetect(glm::vec3 pos) {
    if (m[round(pos.y - 2.0f)][round(pos.x)][round(pos.z)] == true) {
        return false;
    }
    return true;
}

/* 第一版本垂直碰撞检测 */
bool PhysicsEngine::VerticalCollisionDetect(glm::vec3 pos, float& downY) {
    ////cout << sizeof(m) << endl;
    //// if (isJumping) return true;
    //int y_round = round(pos.y);
    //int y_min = y_round - 1;
    //float verticalThreshold = 2.0f;
    //for (auto iter : m[y_min]) {
    //    unsigned int minus_x = abs((int)pos.x - (int)iter.x);
    //    unsigned int minus_z = abs((int)pos.z - (int)iter.z);
    //    if (minus_x > max_threshold || minus_z > max_threshold) {
    //        continue;
    //    }
    //    if (minus_x < threshold && minus_z < threshold && abs((int)pos.y - (int)iter.y) < verticalThreshold) {
    //        downY = pos.y + 2.0f;
    //        return false;
    //    }
    //}
    return true;
}
bool PhysicsEngine::CheckCollision(glm::vec3 pos, glm::vec3 next_pos) // AABB - AABB collision
{
	// x轴方向碰撞？
	bool collisionX = (abs(pos.x-next_pos.x) <=0.6 );
	// y轴方向碰撞？
	bool collisionY = (abs(pos.y - next_pos.y) <= 2);
	bool collisionZ = (abs(pos.z - next_pos.z) <= 0.6);
	// 只有两个轴向都有碰撞时才碰撞
	return collisionX && collisionY&&collisionZ;
}

PhysicsEngine::PhysicsEngine() {
    isJumping = false;
    isFreeAll = false;
    currentHeight = 0.0f;
    initialSpeed = 7.0f;
    currentSpeed = 0.0f;
    slowingdownFactor = 1.8f;
    gravityFactor = -9.8f;
    threshold = 2;
    max_threshold = 1;
	destroyed = false;
}