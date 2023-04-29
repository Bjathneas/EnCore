#include "EnCore.h"

namespace EnCore {
Entity::Entity(unsigned int uid) : uid(uid) {}

unsigned int Entity::get_uid() { return uid; }

Manager::Manager() {}

unsigned int Manager::create_entity() {
  entities.push_back(new Entity(next_entity_id));
  next_entity_id++;
  return next_entity_id - 1;
}

void Manager::destroy_entity(unsigned int uid) {
  auto entity = std::remove(entities.begin(), entities.end(), get_entity(uid));
  std::for_each(entities.begin(), entity,
                [](Entity* entity) { delete entity; });
  entities.erase(entity, entities.end());
}

Entity* Manager::get_entity(unsigned int uid) {
  for (Entity* entity : entities) {
    if (entity->get_uid() == uid) return entity;
  }
  return nullptr;
}

System::System() {}
}  // namespace EnCore
