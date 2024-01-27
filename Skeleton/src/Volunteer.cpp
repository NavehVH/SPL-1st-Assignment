#include <string>
#include "Volunteer.h"

class WareHouse;

Volunteer::Volunteer(int id, const string &name): id(id), name(name) {}

int Volunteer::getId() const {
    return id;
}

const string &Volunteer::getName() const {
    return name;
}

int Volunteer::getActiveOrderId() const {
    if (activeOrderId == 0) {
        return NO_ORDER;
    }
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const {
    if (completedOrderId == 0) {
        return NO_ORDER;
    }
    return completedOrderId;
}

bool Volunteer::isBusy() const {
    if (activeOrderId == NO_ORDER)
        return false;
    return true;
}

bool Volunteer::hasFinishedOrder() const {
    if (activeOrderId == completedOrderId && activeOrderId != NO_ORDER) {
        return true;
    }
    return false;
}

//CollectorVolunteer

CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown): Volunteer(id, name), coolDown(coolDown) {
}

CollectorVolunteer *CollectorVolunteer::clone() const {
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step() {
    if (decreaseCoolDown()) {
        completedOrderId = activeOrderId;
    }
}

//Not a limit volunteer so always returns true
bool CollectorVolunteer::hasOrdersLeft() const {
    return true;
}

int CollectorVolunteer::getCoolDown() const {
    return coolDown;
}

//#TODO: Check if i need to limit this method if it gets to 0..
bool CollectorVolunteer::decreaseCoolDown() {
    timeLeft -= 1;
    if (timeLeft == 0) {
        return true;
    }
    return false; 
}

int CollectorVolunteer::getTimeLeft() const {
    return timeLeft;
}

//#TODO: Probably will change this, too many checks
bool CollectorVolunteer::canTakeOrder(const Order &order) const {
    if (!isBusy() && timeLeft == 0 && order.getStatus() == OrderStatus::PENDING)
        return true;
    return false;
}

void CollectorVolunteer::acceptOrder(const Order &order) {
    if (!CollectorVolunteer::canTakeOrder(order)) {
        return;
    }
    activeOrderId = order.getId();
    timeLeft = coolDown;
}

string CollectorVolunteer::toString() const {
    string s = "VolunteerID " + std::to_string(getId()) + "\r\n"
    "isBusy: ";
    if (isBusy()) {
        s += "True";
    } else {
        s += "False"
        "\r\nOrderID: " + activeOrderId;
    }
    s += "timeLeft: ";
    if (timeLeft == 0) {
        s += "None";
    } else {
        s += std::to_string(timeLeft);
    }
    s += "\r\nordersLeft: No Limit"; 
    return s;
    //return "id: " + std::to_string(getId()) + ", name: " + getName() + ", completedOrderId: " + std::to_string(getCompletedOrderId()) + ", activeOrderId: " + std::to_string(getActiveOrderId()) + ", coolDown: " + std::to_string(getCoolDown()) + ", timeLeft: " + std::to_string(getTimeLeft()); 
}


//LimitedCollector

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown ,int maxOrders)
                : CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders) {
}

LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const {
    return new LimitedCollectorVolunteer(*this);
}

//#TODO: Probably will change this, too many checks, check if i need to delete him here if reached max order
bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {
    if (CollectorVolunteer::canTakeOrder(order) && hasOrdersLeft())
        return true;
    return false;
}

int LimitedCollectorVolunteer::getMaxOrders() const {
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}


void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
    CollectorVolunteer::acceptOrder(order);
    ordersLeft -= 1;
}

string LimitedCollectorVolunteer::toString() const {
    string s = "VolunteerID " + std::to_string(getId()) + "\r\n"
    "isBusy: ";
    if (isBusy()) {
        s += "True";
    } else {
        s += "False"
        "\r\nOrderID: " + activeOrderId;
    }
    s += "timeLeft: ";
    if (getTimeLeft() == 0) {
     
        s += "None";
    } else {
        s += std::to_string(getTimeLeft());
    }
    s += "\r\nordersLeft: " + ordersLeft; 
    return s;
    //return "id: " + std::to_string(getId()) + ", name: " + getName() + ", completedOrderId: " + std::to_string(getCompletedOrderId()) + ", activeOrderId: " + std::to_string(getActiveOrderId()) + ", coolDown: " + std::to_string(getCoolDown()) + ", timeLeft: " + std::to_string(getTimeLeft()); 
}

//DriverVolunteer
DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep)
                : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep) {
}

DriverVolunteer *DriverVolunteer::clone() const {
    return new DriverVolunteer(*this);
}

void DriverVolunteer::step() {
    distanceLeft -= distancePerStep;

    if (distanceLeft <= 0) {
        distanceLeft = 0;
        completedOrderId = activeOrderId;
    }
}

bool DriverVolunteer::decreaseDistanceLeft() {
    distanceLeft -= distancePerStep;

    if (distanceLeft <= 0) {
        return true;
    }
    return false;
}

//Will change this in the future (also can get pending orders? probably will know after setting collectorId correctly)
bool DriverVolunteer::canTakeOrder(const Order &order) const {
    if (!isBusy() && distanceLeft == 0 && order.getDistance() <= maxDistance && order.getStatus() == OrderStatus::COLLECTING) {
        return true;
    }
    return false;
}

 void DriverVolunteer::acceptOrder(const Order &order) {
    if (!DriverVolunteer::canTakeOrder(order)) {
        return;
    }
    activeOrderId = order.getId();
    distanceLeft = order.getDistance(); //#TODO: Check about this in the forum
}

string DriverVolunteer::toString() const {
    string s = "VolunteerID " + std::to_string(getId()) + "\r\n"
    "isBusy: ";
    if (isBusy()) {
        s += "True";
    } else {
        s += "False"
        "\r\nOrderID: " + activeOrderId;
    }
    s += "distance_Left: ";
    if (distanceLeft == 0) {
        s += "None";
    } else {
        s += std::to_string(distanceLeft); 
    }
    s += "\r\nordersLeft: No Limit"; 
    return s;
}

//always true because he is not limited
bool DriverVolunteer::hasOrdersLeft() const {
    return true;
}

int DriverVolunteer::getDistanceLeft() const {
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const {
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const {
    return distancePerStep;
}

//LimitedDriverVolunteer 
//#TODO Finish here limited driver methods, didn't finish

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders)
                : DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(ordersLeft) { 
}

LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const {
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}

string LimitedDriverVolunteer::toString() const {
    string s = "VolunteerID " + std::to_string(getId()) + "\r\n"
    "isBusy: ";
    if (isBusy()) {
        s += "True";
    } else {
        s += "False"
        "\r\nOrderID: " + activeOrderId;
    }
    s += "distance_Left: ";
    if (getDistanceLeft() == 0) {
        s += "None";
    } else {
        s += std::to_string(getDistanceLeft());
    }
    s += "\r\nordersLeft: " + ordersLeft; 
    return s;
}
