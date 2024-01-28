#include "Order.h"

Order::Order(int id, int customerId, int distance)
                    : id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING),
                     collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) { 
}

int Order::getId() const {
    return id;
}

int Order::getCustomerId() const {
    return customerId;
}

int Order::getDistance() const {
    return distance;
}

OrderStatus Order::getStatus() const {
    return status;
}

void Order::setStatus(OrderStatus status) {
    this->status = status;
}

int Order::getCollectorId() const {
    return collectorId;
}

void Order::setCollectorId(int collectorId) {
    this->collectorId = collectorId;
}

int Order::getDriverId() const {
    if (driverId == 0) //no id?
        return NO_VOLUNTEER;
    return driverId;
}

void Order::setDriverId(int driverId) {
    this->driverId = driverId;
}

string Order::enumToString(OrderStatus e) const {
    string text = "";

    switch (e) {
    case OrderStatus::PENDING:
        text = "PENDING";
        break;
    case OrderStatus::COLLECTING:
        text = "COLLECTING";
        break;
    case OrderStatus::DELIVERING:
        text = "DELIVERING";
    case OrderStatus::COMPLETED:
        text = "COMPLETED";
        break;
    }

    return text;
}

//#TODO: Check if this is how they want toString to be declared or in a different way
const string Order::toString() const {
    string s = "CustomerID: " + std::to_string(customerId) + "\r\n"
    "OrderId: " + std::to_string(id) + "\r\n"
    "OrderStatus: " + enumToString(status) + "\r\n"
    "Collector: ";

    if (collectorId == NO_VOLUNTEER)
        s += "None";
    else
        s += std::to_string(collectorId);
    s += "\r\nDriver: ";
    if (driverId == NO_VOLUNTEER)
        s += "None";
    else
        s += std::to_string(driverId);

    return s;
    //return "id: " + std::to_string(id) + ", customerId: " + std::to_string(customerId) + ", distance: " + std::to_string(distance) + ", status: " + enumToString(status) + ", collectorId: " + std::to_string(collectorId) + ", driverId: " + std::to_string(driverId);
}