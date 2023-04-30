/*
 * @file EnCore.h
 * @brief Defines Component, Entity, Manager, and System
 *
 * Defines the ECS and template functions
 *
 * @author Bjathnes (https://github.com/Bjathneas)
 */

#pragma once

#include <algorithm>
#include <memory>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace EnCore {

struct Component {};

class Entity {
 public:
  inline Entity(unsigned int uid) : uid(uid) {}

  inline unsigned int get_uid() { return uid; }

  template <typename Comp>
  void add_component(std::shared_ptr<Comp> component) {
    components[typeid(Comp).name()] = component;
  }

  template <typename Comp>
  void remove_component() {
    auto it = components.find(typeid(Comp).name());

    if (it != components.end()) {
      it->second.reset();
      components.erase(it);
    }
  }

  template <typename Comp>
  bool contains() {
    return components.count(typeid(Comp).name());
  }

  template <typename Comp>
  std::shared_ptr<Comp> get_component() {
    auto it = components.find(typeid(Comp).name());

    if (it != components.end())
      return std::static_pointer_cast<Comp>(it->second);
    else
      return nullptr;
  }

 private:
  unsigned int uid;
  std::unordered_map<const char*, std::shared_ptr<Component>> components;
};

class Manager {
 public:
  Manager() {}

  inline unsigned int create_entity() {
    entities[next_entity_id] = new Entity(next_entity_id);
    next_entity_id++;
    return next_entity_id - 1;
  }
  inline void destroy_entity(unsigned int uid) {
    auto it = entities.find(uid);
    if (it != entities.end()) {
      delete entities[uid];
      entities.erase(it);
    }
  }

  Entity* get_entity(unsigned int uid) {
    auto it = entities.find(uid);
    if (it != entities.end()) return entities[uid];
    return nullptr;
  }

  template <typename Comp>
  std::vector<Entity*> get_entities_with_components() {
    std::vector<Entity*> entities_with_components;

    for (std::pair<unsigned int, Entity*> pair : entities) {
      Entity* entity = std::get<1>(pair);
      if (entity->contains<Comp>()) entities_with_components.push_back(entity);
    }

    return entities_with_components;
  }

 private:
  std::unordered_map<unsigned int, Entity*> entities;
  unsigned int next_entity_id{0};
};

class System {
 public:
  System() {}

  virtual void update(Manager manager) = 0;
};
}  // namespace EnCore
