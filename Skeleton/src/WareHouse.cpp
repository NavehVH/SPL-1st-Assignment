#include "../include/WareHouse.h"
#include "../include/Action.h"
#include <algorithm>

const vector<Action*> &WareHouse::getActions() const
{
    return actionsLog;
}

//Am I suppose to return null if not found? .-.
Customer &WareHouse::getCustomer(int customerId) const
{
    for (Customer *c : customers)
        if (c->getId() == customerId)
            return *c;
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const
{
    for (Volunteer *v : volunteers)
        if (v->getId() == volunteerId)
            return *v;
}

vector<Volunteer*> &WareHouse::getVolunteers() {
    return volunteers;
}

vector<Order *> &WareHouse::getPendingOrders()
{
    return pendingOrders;
}

Order &WareHouse::getOrder(int orderId) const
{
    for (Order *o : pendingOrders)
        if (o->getId() == orderId)
            return *o;
    for (Order *o : inProcessOrders)
        if (o->getId() == orderId)
            return *o;
    for (Order *o : completedOrders)
        if (o->getId() == orderId)
            return *o;
}

void WareHouse::addOrder(Order *order) {
    if (std::find(pendingOrders.begin(), pendingOrders.end(), order) != pendingOrders.end())
    {
        if (order->getStatus() == OrderStatus::PENDING) {
           pendingOrders.erase(std::remove(pendingOrders.begin(), pendingOrders.end(), order), pendingOrders.end());
           inProcessOrders.push_back(order);
        }
        else if (order->getStatus() == OrderStatus::DELIVERING) {
            inProcessOrders.erase(std::remove(inProcessOrders.begin(), inProcessOrders.end(), order), inProcessOrders.end());
           completedOrders.push_back(order);
        }
    }
}

int WareHouse::getOrderCounter()
{
    return orderCounter;
}

int WareHouse::getCustomerCounter()
{
    return orderCounter;
}

int WareHouse::getVolunteerCounter() {
    return volunteerCounter;
}