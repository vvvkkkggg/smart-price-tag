#ifndef SERVER_SMART_TAG_DB_H
#define SERVER_SMART_TAG_DB_H

#include <vector>
#include "entity.h"

namespace DB {
    int createEntitiesTableIfNotExist();

    std::vector<Entity> getAllEntities();

    Entity *getEntityByScreen(int screenId);

    int insertEntity(const Entity &entity);

    int updateEntityByScreeId(const Entity &entity);
}

#endif
