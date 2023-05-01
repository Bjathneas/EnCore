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
  Entity(unsigned int uid) : uid(uid) {}

  ~Entity() {
    for (auto pair : components) {
      auto component = std::get<1>(pair);
      component.reset();
    }
  }

  unsigned int get_uid() { return uid; }

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

  ~Manager() {
    for (auto pair : entities) {
      auto entity = std::get<1>(pair);
      entity.reset();
    }
  }

  unsigned int create_entity() {
    entities[next_entity_id] = std::make_shared<Entity>(next_entity_id);
    next_entity_id++;
    return next_entity_id - 1;
  }
  void destroy_entity(unsigned int uid) {
    auto it = entities.find(uid);
    if (it != entities.end()) {
      entities[uid].reset();
      entities.erase(it);
    }
  }

  std::shared_ptr<Entity> get_entity(unsigned int uid) {
    auto it = entities.find(uid);
    if (it != entities.end()) return entities[uid];
    return nullptr;
  }

  template <typename Comp>
  std::vector<std::shared_ptr<Entity>> get_entities_with_components() {
    std::vector<std::shared_ptr<Entity>> entities_with_components;

    for (auto pair : entities) {
      auto entity = std::get<1>(pair);
      if (entity->contains<Comp>()) entities_with_components.push_back(entity);
    }

    return entities_with_components;
  }

 private:
  std::unordered_map<unsigned int, std::shared_ptr<Entity>> entities;
  unsigned int next_entity_id{0};
};

class System {
 public:
  System() {}

  virtual void update(std::shared_ptr<Manager> manager) = 0;
};
}  // namespace EnCore
