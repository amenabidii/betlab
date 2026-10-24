#include "Entity.h"

// basic functions: making an entity, copying it, changing its information, and cleaning it up.
Entity::Entity() {
    id = 0;
    name = "Unnamed";
    active = true;
}
Entity::Entity(int id, string name) {
    this->id = id;
    this->name = name.empty() ? "Unnamed" : name;
    active = true;
}
Entity::Entity(const Entity& other) {
    id = other.id;
    name = other.name;
    active = other.active;
}
Entity& Entity::operator=(const Entity& other) {
    if (this != &other) {
        id = other.id;
        name = other.name;
        active = other.active;
    }
    return *this;
}
// Virtual destructor: deleting a child object through an Entity pointer still runs the child destructor.
Entity::~Entity() {
}
int Entity::getId() const {
    return id;
}
string Entity::getName() const {
    return name;
}
bool Entity::isActive() const {
    return active;
}
void Entity::setId(int newId) {
    if (newId > 0) id = newId;
}
bool Entity::setName(string newName) {
    if (newName.empty()) return false;
    name = newName;
    return true;
}
void Entity::setActive(bool value) {
    active = value;
}
