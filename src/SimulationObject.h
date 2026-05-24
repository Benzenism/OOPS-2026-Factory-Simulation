#pragma once
#include <string>
#include <memory>

class Product;

class SimulationObject {
public:
    virtual ~SimulationObject() = default;
    
    virtual void update(int tick) = 0; 
    virtual std::string getInfo() const = 0; 
    virtual void pushItem(std::shared_ptr<Product> item) = 0;
};
