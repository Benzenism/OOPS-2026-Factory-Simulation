#include "Products.h"

Product::Product(std::string id) : id(std::move(id)) {}

std::string Product::getId() const {
    return id;
}

RawWafer::RawWafer(std::string id) : Product(std::move(id)) {}

std::string RawWafer::getType() const {
    return "RawWafer";
}

RAMModule::RAMModule(std::string id) : Product(std::move(id)) {}

std::string RAMModule::getType() const {
    return "RAMModule (Finished)";
}