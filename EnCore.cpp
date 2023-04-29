#include "EnCore.h"

namespace EnCore {
Entity::Entity(unsigned int uid) : uid(uid) {}

unsigned int Entity::get_uid() { return uid; }

Manager::Manager() {}

unsigned int Manager::create_entity() {
  entities[next_entity_id] = new Entity(next_entity_id);
  next_entity_id++;
  return next_entity_id - 1;
}

void Manager::destroy_entity(unsigned int uid) {
  auto it = entities.find(uid);
  if (it != entities.end()) {
    delete entities[uid];
    entities.erase(it);
  }
}

Entity* Manager::get_entity(unsigned int uid) {
  auto it = entities.find(uid);
  if(it != entities.end())
    return entities[uid];
  return nullptr;
}

System::System() {}
}  // namespace EnCore
