#include "stdafx.h"
#include "entity.h"
#include "player.h"

class Spring : public Entity {
public:
    Spring();
    void Draw(sf::RenderWindow& window);
    bool Update(std::map<std::size_t, std::shared_ptr<Player>>::const_iterator &iter, sf::Time elapsed_time);

private:
    float spring_propulsion = 100000.f;
};
