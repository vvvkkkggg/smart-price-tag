#include <SQLiteCpp/SQLiteCpp.h>
#include "db.h"

SQLite::Database db("db.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

int DB::createEntitiesTableIfNotExist() {
    return db.exec(
            "CREATE TABLE IF NOT EXISTS tags ("
            "id INTEGER PRIMARY KEY,"
            "screenId TEXT,"
            "productName TEXT,"
            "cost TEXT,"
            "isSet INTEGER"
            ")");
}

Entity *buildFromQuery(SQLite::Statement &query) {
    return new Entity(
            query.getColumn(1),
            query.getColumn(2),
            query.getColumn(3)
    );
}

std::vector<Entity> DB::getAllEntities() {
    std::vector<Entity> results;
    SQLite::Statement query(db, "SELECT * FROM tags");

    while (query.executeStep()) {
        results.push_back(
                *buildFromQuery(query)
        );
    }

    return results;
}

Entity *DB::getEntityByScreen(int screenId) {
    SQLite::Statement query(db, "SELECT * FROM tags WHERE screenId=?");
    query.bind(1, screenId);

    int e = query.exec();
    if (e == 0) {
        return nullptr;
    }

    return buildFromQuery(query);
}

int DB::insertEntity(const Entity &entity) {
    SQLite::Statement query(db, "INSERT INTO tags VALUES (NULL, ?, ?, ?, 0)");
    query.bind(1, entity.screenId);
    query.bind(2, entity.productName);
    query.bind(3, entity.cost);
    return query.exec();
}

int DB::updateEntityByScreeId(const Entity &entity) {
    SQLite::Statement query(db, R"(UPDATE tags SET productName=?, cost=?, isSet=0 WHERE screenId=?)");
    query.bind(1, entity.productName);
    query.bind(2, entity.cost);
    query.bind(3, entity.screenId);
    return query.exec();
}

int DB::updateScreenSetStatus(int screenId, int isSet) {
    SQLite::Statement query(db, R"(UPDATE tags SET isSet=? WHERE screenId=?)");
    query.bind(1, isSet);
    query.bind(1, screenId);
    return query.exec();
}