#include "World.h"

World* World::instance = NULL;



// 整个世界的渲染
void World::Render(Shader& shader) {
    ResourceManager* manager = ResourceManager::getInstance();
    PhysicsEngine* engine = PhysicsEngine::getInstance();
    // 画出草地
    glm::vec3 grass_position[460];
    int grass_count = 460;
    int index = 0;
	glm::vec3 vine_position[400];
	int vine_count = 400;
	int index1 = 0;

    for (int i = -10; i < 10; i++) {
        for (int j = -10; j < 10; j++) {
			if ((i > 7 || i < 0|| j != -5)&&(i!=-5||j<0||j>4)) {
				int flag = 1;
				if (i % 5 == 1 || j % 3 == 1 ||(i*j%7==1)|| (i*j % 9 == 1)) {
					flag = 0;
				}
				glm::vec3 pos = glm::vec3((float)i, 0.0f, (float)j);
				if (flag == 1) {
					grass_position[index++] = pos;
				}
				else vine_position[index1++] = pos;
				
				// 添加到物理引擎中
				engine->m[round(pos.y)][round(pos.x)][round(pos.z)] = true;
			}
           
        }
    }
    for (int i = -5; i < 5; i++) {
        for (int j = 6; j < 7; j++) {
            glm::vec3 pos = glm::vec3((float)i, 1.0f, (float)j);
            grass_position[index++] = pos;
            engine->m[round(pos.y)][round(pos.x)][round(pos.z)] = true;
        }
        for (int j = 7; j < 8; j++) {
            if (i > 2) {
                glm::vec3 pos = glm::vec3((float)i, 2.0f, (float)j);
                grass_position[index++] = pos;
                engine->m[round(pos.y)][round(pos.x)][round(pos.z)] = true;
            }
                
            else {
                glm::vec3 pos = glm::vec3((float)i, 3.0f, (float)j);
                grass_position[index++] = pos;
                engine->m[round(pos.y)][round(pos.x)][round(pos.z)] = true;
            }
            
            
        }
        for (int j = 8; j < 9; j++) {
            glm::vec3 pos = glm::vec3((float)i, 3.0f, (float)j);
            grass_position[index++] = pos;
            engine->m[round(pos.y)][round(pos.x)][round(pos.z)] = true;
        }
    }
    SingleRender(shader, grass_count, grass_position, manager->GRASS);
	SingleRender(shader, vine_count, vine_position, manager->VINE);
   // 建立围墙
    glm::vec3 brick_position[400];
    int brick_count = 400;
    index = 0;
    for (int i = -10; i < 10; i++) {
        for (int j = 1; j < 5; j++) {
            glm::vec3 pos = glm::vec3((float)i, (float)j, -10.0f);
            brick_position[index++] = pos;
            engine->m[round(pos.y)][round(pos.x)][round(pos.z)] = true;
        }
    }

    for (int i = -10; i < 10; i++) {
        for (int j = 1; j < 5; j++) {
            glm::vec3 pos = glm::vec3(-10.0f, (float)j, (float)i);
            brick_position[index++] = pos;
            engine->m[round(pos.y)][round(pos.x)][round(pos.z)] = true;
        }
    }

    for (int i = -10; i < 10; i++) {
        for (int j = 1; j < 5; j++) {
            glm::vec3 pos = glm::vec3((float)i, (float)j, 9.0f);
            brick_position[index++] = pos;
            engine->m[round(pos.y)][round(pos.x)][round(pos.z)] = true;
        }
    }

    for (int i = -10; i < 10; i++) {
        for (int j = 1; j < 5; j++) {
            glm::vec3 pos = glm::vec3(9.0f, (float)j, (float)i);
            brick_position[index++] = pos;
            engine->m[round(pos.y)][round(pos.x)][round(pos.z)] = true;
        }
    }
    SingleRender(shader, brick_count, brick_position, manager->BRICK);

    // 水流
    glm::vec3 water_position[20];
    int water_count = 20;
    index = 0;
    for (int i = 0; i < 8; i++) {
			glm::vec3 pos = glm::vec3((float)i, 0.0f, -5.0f);
			water_position[index++] = pos;
			//水不做碰撞
			engine->m[round(pos.y)][round(pos.x)][round(pos.z)] = false;	
    }
	for (int i = 0; i < 5; i++) {
		glm::vec3 pos = glm::vec3(-5.0f, 0.0f, (float)i);
		water_position[index++] = pos;
		//水不做碰撞
		engine->m[round(pos.y)][round(pos.x)][round(pos.z)] = false;
	}
    SingleRender(shader, water_count, water_position, manager->WATER);
    // 树
    DrawTree(manager->Tree_oak, manager->Leave_oak, glm::vec3(-3.0f, 1.0f, 0.0f), shader);
    DrawTree(manager->Tree_jungle, manager->Leave_jungle, glm::vec3(-6.0f, 1.0f, -2.0f), shader);
    DrawTree(manager->Tree_birch, manager->Leave_birch, glm::vec3(-7.0f, 1.0f, -7.0f), shader);
    // 蘑菇
    glm::vec3 mushroom_position[3] = { glm::vec3(3.0f), glm::vec3(1.0f,2.0f,7.0f) ,glm::vec3(2.0f,6.0f,9.5f)  };
    SingleRender(shader, 3, mushroom_position, manager->MUSHROOM);
	glm::vec3 flower_position[3] = { glm::vec3(1.0f,1.0f,-8.0f) ,glm::vec3(4.0f,2.0f,-5.0f) ,glm::vec3(-9.0f,6.0f,9.5f) };
	SingleRender(shader, 3, flower_position, manager->FLOWER);
	glm::vec3 model_position[3] = { glm::vec3(0.0f,5.0f,0.0f) };
	Rendermodel(shader, 1, model_position, manager->rose);
}

void World::Rendermodel(Shader& shader, int count, glm::vec3* position, ResourceManager::ModelType model) {
	ResourceManager* manager = ResourceManager::getInstance();
	glBindBuffer(GL_ARRAY_BUFFER, manager->getInstanceVBO());
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * count, &position[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	manager->RenderModelScene(shader, model, count);

}
void World::SingleRender(Shader& shader, int count, glm::vec3* position, ResourceManager::BlockType block) {
    ResourceManager* manager = ResourceManager::getInstance();
    glBindBuffer(GL_ARRAY_BUFFER, manager->getInstanceVBO());
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * count, &position[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    manager->RenderScene(shader, block, count);
}

void World::DrawTree(ResourceManager::BlockType tree_type, ResourceManager::BlockType leave_type, glm::vec3 pos, Shader& shader) {
    ResourceManager* manager = ResourceManager::getInstance();
    PhysicsEngine* engine = PhysicsEngine::getInstance();
    // 画出树干
    glm::vec3 tree[5];
    int count = 5;
    int index = 0;
    for (int i = 0; i < count; i++) {
        glm::vec3 tmp = glm::vec3(pos.x, pos.y + (float)i, pos.z);
        tree[index++] = tmp;
        // 添加到物理引擎中
        engine->m[round(tmp.y)][round(tmp.x)][round(tmp.z)] = true;
    }
    SingleRender(shader, count, tree, tree_type);

    // 画出树叶
    // 画出树干
    glm::vec3 leaves[35];
    count = 35;
    index = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = -2 + i; j <= 2 - i; j++) {
            for (int k = -2 + i; k <= 2 - i; k++) {
                glm::vec3 tmp = glm::vec3(pos.x + (float)j, pos.y + (float)(i + 4), pos.z + (float)k);
                leaves[index++] = tmp;
                // 添加到物理引擎中
                engine->m[round(tmp.y)][round(tmp.x)][round(tmp.z)] = true;
            }
        }
    }
    SingleRender(shader, count, leaves, leave_type);
}

