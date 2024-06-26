#include "../include/Order.h"

Order::Order() : id(-1), customerId(-1), distance(-1), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER){
    
}

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

void Order::setStatus(OrderStatus status) {
    this->status = status;
}

void Order::setCollectorId(int collectorId) {
    this->collectorId = collectorId;
}

void Order::setDriverId(int driverId) {
    this->driverId = driverId;
}

int Order::getCollectorId() const {
    return collectorId;
}

int Order::getDriverId() const {
    return driverId;
}

OrderStatus Order::getStatus() const {
    return status;
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
        break;
    case OrderStatus::COMPLETED:
        text = "COMPLETED";
        break;
    }

    return text;
}

const string Order::toString() const {
    string s = "OrderId: " + std::to_string(id) + "\r\n"
    "OrderStatus: " + enumToString(status) + "\r\n"
    "CustomerID: " + std::to_string(customerId) + "\r\n"
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
}