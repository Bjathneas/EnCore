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

/**
 * @brief A base class for all components.
 */
struct Component {};

/**
 * @brief A class representing an entity in the ECS architecture.
 */
class Entity {
 public:

  /**
   * @brief Constructs a new Entity object with the given unique identifier.
   * @param uid The unique identifier for the entity.
   */
  Entity(unsigned int uid) : uid(uid) {}
  
  /**
   * @brief Destroys the Entity object, releasing its component resources.
   */
  ~Entity() {
    for (auto pair : components) {
      auto component = std::get<1>(pair);
      component.reset();
    }
  }
  
  /**
   * @brief Gets the unique identifier of the Entity.
   * @return The unique identifier of the Entity.
   */
  unsigned int get_uid() { return uid; }
  
  /**
   * @brief Adds a new component of the given type to the Entity.
   * @tparam Comp The type of the component to add.
   * @param component A shared pointer to the component to add.
   */
  template <typename Comp>
  void add_component(std::shared_ptr<Comp> component) {
    components[typeid(Comp).name()] = component;
  }

  /**
   * @brief Removes the component of the given type from the Entity.
   * @tparam Comp The type of the component to remove.
   */
  template <typename Comp>
  void remove_component() {
    auto it = components.find(typeid(Comp).name());

    if (it != components.end()) {
      it->second.reset();
      components.erase(it);
    }
  }
  
  /**
   * @brief Checks whether the Entity has a component of the given type.
   * @tparam Comp The type of the component to check for.
   * @return `true` if the Entity has a component of the given type, `false`
   * otherwise.
   */
  template <typename Comp>
  bool contains() {
    return components.count(typeid(Comp).name());
  }
  
  /**
   * @brief Gets the component of the given type from the Entity.
   * @tparam Comp The type of the component to get.
   * @return A shared pointer to the component, or `nullptr` if the Entity
   * doesn't have a component of the given type.
   */
  template <typename Comp>
  std::shared_ptr<Comp> get_component() {
    auto it = components.find(typeid(Comp).name());

    if (it != components.end())
      return std::static_pointer_cast<Comp>(it->second);
    else
      return nullptr;
  }

 private:
  unsigned int uid; /**< The unique identifier of the Entity. */
  std::unordered_map<const char*, std::shared_ptr<Component>>
      components; /**< The components attached to the Entity. */
};

/**
 * @brief A class representing the entity manager in the ECS architecture.
 */
class Manager {
 public:
  
  /**
   * @brief Constructs a new Manager object.
   */
  Manager() {}
  
  /**
   * @brief Destroys the Manager object, releasing all entity resources.
   */
  ~Manager() {
    for (auto pair : entities) {
      auto entity = std::get<1>(pair);
      entity.reset();
    }
  }
  
  /**
   * @brief Creates a new Entity object and returns its unique identifier.
   * @return The unique identifier of the newly created Entity.
   */
  unsigned int create_entity() {
    entities[next_entity_id] = std::make_shared<Entity>(next_entity_id);
    next_entity_id++;
    return next_entity_id - 1;
  }
  
  /**
   * @brief Destroys the entity with the given unique identifier
   * @param uid - Unique identifier of the entity to destroy
   */
  void destroy_entity(unsigned int uid) {
    auto it = entities.find(uid);
    if (it != entities.end()) {
      entities[uid].reset();
      entities.erase(it);
    }
  }
  
  /**
   * @brief Returns a shared pointer to the entity with the given unique identifier
   * @param uid - Unique identifier of the entity to retrieve
   * @return std::shared_ptr<Entity> - Shared pointer to the retrieved entity or
   * nullptr if the entity does not exist
   */
  std::shared_ptr<Entity> get_entity(unsigned int uid) {
    auto it = entities.find(uid);
    if (it != entities.end()) return entities[uid];
    return nullptr;
  }
  
  /**
   * @brief Returns a vector of shared pointers to entities that have a component of the given type
   * @tparam Comp - Type of the component
   * @return std::vector<std::shared_ptr<Entity>> - Vector of shared pointers to entities with the given component type
   */
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
  std::unordered_map<unsigned int, std::shared_ptr<Entity>> entities; /**< The map of entities owned by the Manager. */
  unsigned int next_entity_id{0}; /**< The unique ID of the next Entity to be created. */
};

/**
* @brief A class representing a system in the game engine
*/
class System {
 public:
  
  /**
   * @brief Constructor for System class
   */
  System() {}
  
  /**
   * @brief Virtual function for updating the system
   * @param manager - A shared pointer to the Manager class instance
   */
  virtual void update(std::shared_ptr<Manager> manager) = 0;
};
}  // namespace EnCore
