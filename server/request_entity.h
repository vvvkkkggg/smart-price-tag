#ifndef REQUEST_ENTITY_H_
#define REQUEST_ENTITY_H_

#include <string>
#include <utility>

class Entity {
public:
    Entity(std::string  product_, std::string  cost_) : product_name(std::move(product_)), cost(std::move(cost_)) {
    }

    [[nodiscard]] const std::string& get_product_name() const {
        return product_name;
    }

    [[nodiscard]] const std::string& get_cost() const {
        return cost;
    }

private:
    std::string product_name;
    std::string cost;
};


#endif