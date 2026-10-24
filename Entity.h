#ifndef ENTITY_H
#define ENTITY_H
#include <string>
#include <iostream>
using namespace std;

// The common parent class for every bettor. IDs and names are stored here, and virtual functions let the child classes behave differently.
class World;

// Every object in the world has an ID and can act when a turn runs.
class Entity {
private:
    int id;
    string name;
    bool active;
public:
    Entity();
    Entity(int id, string name);
    Entity(const Entity& other);
    Entity& operator=(const Entity& other);
    virtual ~Entity();
    int getId() const;
    string getName() const;
    bool isActive() const;
    void setId(int newId);
    bool setName(string newName);
    void setActive(bool value);
    virtual string getType() const = 0;
    virtual void performTurn(World& world) = 0;
    virtual void print() const = 0;
    virtual Entity* clone() const = 0;
};
#endif
