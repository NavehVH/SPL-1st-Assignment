#include <string>
#include <vector>
#include <algorithm>
#include "Customer.h"

using std::string;
using std::vector;

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders) : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders) {}

const string &Customer::getName() const
{
    return name;
}

int Customer::getId() const
{
    return id;
}

int Customer::getCustomerDistance() const
{
    return locationDistance;
}

int Customer::getMaxOrders() const
{ // Returns maxOrders
    return maxOrders;
}

const vector<int> &Customer::getOrdersIds() const
{ // Returns verctor of orders IDs
    return ordersId;
}

int Customer::getNumOrders() const
{ // Returns num of orders the customer has made so far
    return ordersId.size();
}

bool Customer::canMakeOrder() const
{ // Returns true if the customer didn't reach max orders
    if (ordersId.size() >= maxOrders)
        return false;
    return true;
}

//#TODO: Ask about this method, are u supposed to actually ADD or just check, if so, how to check\add correctly?
int Customer::addOrder(int orderId)
{
    if (std::find(getOrdersIds().begin(), getOrdersIds().end(), orderId) != getOrdersIds().end()) //check if orderId is in verctor orderIds
        return orderId;
    return -1;
}

SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders): Customer(id, name, locationDistance, maxOrders) {}

//#TODO: Check if this is how u suppose to clone, or u need to DEEP SAVE
SoldierCustomer *SoldierCustomer::clone() const {
    return new SoldierCustomer(*this);
}

CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders): Customer(id, name, locationDistance, maxOrders) {}

//#TODO: Check if this is how u suppose to clone, or u need to DEEP SAVE
CivilianCustomer *CivilianCustomer::clone() const {
    return new CivilianCustomer(*this);
}