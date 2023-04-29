
# EnCore

EnCore is an Entity Component System that was developed for a game engine that I am still currently developing


## Installation

Add ```EnCore.h``` and ```EnCore.cpp``` to your project
    
## Usage/Examples
```cpp
//Include Encore
#include "Encore.h"

//Create components
struct PosComp : Encore::Component {
    int x, y;
    PosComp(x, y) : x(x), y(y) {}
};

struct VelComp : Encore::Component {
    double x, y;
    VelComp(x, y) : x(x), y(y) {}
};

//Create system
class UpdateSystem : public Encore::System {
public:
    UpdateSystem();
    void update(Encore::Manager manager) override {
        //Get entites with VelComp
        std::vector<Encore::Entity*> entities = manager.get_entities_with_components<VelComp>();
        for(Entity* entity : entities){
            //Check that entity has a PosComp
            if(entity->contains<PosComp>()){
                //Get components
                auto pos_comp = entity->get_component<PosComp>();
                auto vel_comp = entity->get_component<VelComp>();
                //Update values
                pos_comp->x += vel_comp->x;
                pos_comp->y += vel_comp->y;
            }
        }
    }
};

int main() {
    //Create the ECS manager
    auto manager = Encore::Manager();

    //Create and store entity
    unsigned int entity_uid = manager.create_entity();
    //Make components
    auto pos_comp = std::make_shared<PosComp>(10, 10);// x=10 y=10
    auto vel_comp = std::make_shared<VelComp>(3.14, 3.14)// vel_x = 3.14, vel_y = 3.13
    //Get entity and add compnents
    auto entity = manager.get_entity(uid);
    entity->add_component(pos_comp);
    entity->add_component(vel_comp);

    //Create System
    auto system = UpdateSystem();

    // Update Loop
    while(true) {
        system.update(manager);
    }
}
```


## Authors

- [@Bjathneas](EnCore.h)


