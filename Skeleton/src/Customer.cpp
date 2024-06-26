#include <string>
#include <vector>
#include <algorithm>
#include "../include/Customer.h"
#include <iostream>

using std::string;
using std::vector;

Customer::Customer() : id(-1), name(""), locationDistance(-1), maxOrders(-1),  ordersId() {  
}

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders) 
                : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId() {   
}

const string &Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

int Customer::getCustomerDistance() const {
    return locationDistance;
}

int Customer::getMaxOrders() const { // Returns maxOrders
    return maxOrders;
}

int Customer::getNumOrders() const { // Returns num of orders the customer has made so far
    return ordersId.size();
}

bool Customer::canMakeOrder() const { // Returns true if the customer didn't reach max orders
    if (static_cast<int>(ordersId.size()) >= maxOrders) {
        return false;
    }
    return true;
}

const vector<int> &Customer::getOrdersIds() const { // Returns verctor of orders IDs
    return ordersId;
}

void Customer::PushToOrdersIds(int id) {
    ordersId.push_back(id);
}


int Customer::addOrder(int orderId) {
    if (std::find(getOrdersIds().begin(), getOrdersIds().end(), orderId) != getOrdersIds().end()) { //check if orderId is in verctor orderIds
        return orderId;
    }
    return -1;
}

SoldierCustomer::SoldierCustomer() : Customer() {    
}

SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders)
                   : Customer(id, name, locationDistance, maxOrders) {
}

SoldierCustomer *SoldierCustomer::clone() const {
    return new SoldierCustomer(*this);
}

CivilianCustomer::CivilianCustomer() : Customer() { 
}

CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders)
                   : Customer(id, name, locationDistance, maxOrders) { 
}

CivilianCustomer *CivilianCustomer::clone() const {
    return new CivilianCustomer(*this);
}
