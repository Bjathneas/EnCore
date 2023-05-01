
# EnCore

EnCore is an Entity Component System that was developed for a game engine that I am still currently developing


## Installation

Add ```EnCore.h``` to your project
    
## Usage/Examples
```cpp
//Include Encore
//Include Encore
#include "EnCore.h"

#include <iostream>

//Create components
struct PosComp : EnCore::Component {
    int x, y;
    PosComp(int x, int y) : x(x), y(y) {}
};

struct VelComp : EnCore::Component {
    double x, y;
    VelComp(double x, double y) : x(x), y(y) {}
};

//Create system
class UpdateSystem : public EnCore::System {
public:
    UpdateSystem() {}
    void update(std::shared_ptr<EnCore::Manager> manager) override {
        //Get entites with VelComp
        auto entities = manager->get_entities_with_components<VelComp>();
        for(auto entity : entities){
            //Check that entity has a PosComp
            if(entity->contains<PosComp>()){
                //Get components
                auto pos_comp = entity->get_component<PosComp>();
                auto vel_comp = entity->get_component<VelComp>();
                //Update values
                pos_comp->x += vel_comp->x;
                pos_comp->y += vel_comp->y;
                
                //print out the new position
                std::cout << "x: " << pos_comp->x << " y: " << pos_comp->y << "\n";
            }
        }
    }
};

int main() {
    //Create the ECS manager
    auto manager = std::make_shared<EnCore::Manager>();

    //Create and store entity
    unsigned int entity_uid = manager->create_entity();
    //Make components
    auto pos_comp = std::make_shared<PosComp>(10, 10);// x=10 y=10
    auto vel_comp = std::make_shared<VelComp>(3.14, 3.14);// vel_x = 3.14, vel_y = 3.13
    //Get entity and add compnents
    auto entity = manager->get_entity(entity_uid);
    entity->add_component(pos_comp);
    entity->add_component(vel_comp);

    //Create System
    auto system = UpdateSystem();

    // Update Loop
    while(true) {
        system.update(manager);
    }

    return 0;
}
```


## Authors

- [@Bjathneas](https://github.com/Bjathneas)


