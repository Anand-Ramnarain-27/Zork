#pragma once
#include "Entity.h"
#include "GameEnums.h" 
#include <map>
#include <string>
#include <vector>

using std::map;

class Room : public Entity {
private:
    map<Direction, Entity*> exits;
    bool isDark;

public:
    Room(const string& name, const string& description, bool isDark = false)
        : Entity(EntityType::ROOM, name, description), isDark(isDark) {
    }

    void setExit(Direction direction, Entity* exit);
    Entity* getExit(Direction direction) const;
    void look() const override;

    const map<Direction, Entity*>& getExits() const { return exits; }

    void setDark(bool dark) { isDark = dark; }
    bool getIsDark() const { return isDark; }
};