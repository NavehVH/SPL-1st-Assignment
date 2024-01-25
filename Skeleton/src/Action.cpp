#include <string>
#include <iostream>
#include "Action.h"

void Action::complete() {
    status = ActionStatus::COMPLETED;
}

void Action::error(string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    std::cout << "Error:" + errorMsg << std::endl;
}

string Action::getErrorMsg() const {
    return errorMsg;
}

ActionStatus Action::getStatus() const {
    return status;
}

SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {
}

void SimulateStep::act(WareHouse &wareHouse) {
    for (int i = 0; i < numOfSteps; i++) {
        for (Volunteer *volunteer : wareHouse.getVolunteers()) { // part(3) 
            volunteer->step(); // part (2)
            if (volunteer->hasFinishedOrder()) {
                //volunteer->acceptOrder();     // part (2) and (1) #TODO: Do a method that gets the most needed to finish order, easy
                AddOrder(getActiveOrderId()); // part (1), Updates to the correct vector of orders
                if (!volunteer->hasOrdersLeft()) { // part(4)
                    // DELETE THE BITCH
                }
            }
        }
    }
}

SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
}

AddOrder::AddOrder(int id) : customerId(id) {
}
void AddOrder::act(WareHouse &wareHouse) {
    Customer &c = wareHouse.getCustomer(customerId);
    if (c.getId() == 0 || !c.canMakeOrder()) {
         // don't know how to check if didn't find customer (id = 0?\null?), #TODO: add default return
        error("Cannot place this order");
        return;
    }

    wareHouse.setOrderCounter(wareHouse.getOrderCounter() + 1);
    Order *o = new Order(wareHouse.getOrderCounter(), customerId, c.getCustomerDistance());
    wareHouse.getPendingOrders().push_back(o);
}

AddOrder* AddOrder::clone() const {
    return new AddOrder(*this);
}

AddCustomer::AddCustomer(string customerName, CustomerType customerType, int distance, int maxOrders)
    : customerName(customerName), customerType(customerType), distance(distance), maxOrders(maxOrders) {
}

void AddCustomer::act(WareHouse &wareHouse) {
    wareHouse.setCustomerCounter(wareHouse.getCustomerCounter() + 1);
    Customer *c = nullptr;
    if (customerType == CustomerType::Soldier)
        c = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
    else
        c = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
}

AddCustomer* AddCustomer::clone() const {
    return new AddCustomer(*this);
}

PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {
}

void PrintOrderStatus::act(WareHouse &wareHouse) {

    if (wareHouse.getOrderCounter() < orderId) {
        std::cout << "Order doesn't exist" << std::endl;
        return;
    }

    Order order = wareHouse.getOrder(orderId);
    std::cout << order.toString() << std::endl;
}

PrintOrderStatus* PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}

PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId) {   
}

void PrintCustomerStatus::act(WareHouse &wareHouse) {
    if (wareHouse.getCustomerCounter() < customerId) {
        std::cout << "Customer doesn't exist" << std::endl;
        return;
    }
    std::cout << "CustomerID: " + customerId << std::endl;

    for (int orderId : wareHouse.getCustomer(customerId).getOrdersIds()) {
        Order order = wareHouse.getOrder(orderId);
        std::cout << order.toString() << std::endl;
    }

    int numOrdersLeft = wareHouse.getCustomer(customerId).getMaxOrders() - wareHouse.getCustomer(customerId).getOrdersIds().size();
    std::cout << "numOrdersLeft: " + numOrdersLeft << std::endl;
}

PrintCustomerStatus* PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}

PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id) {
}

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    if (wareHouse.getVolunteerCounter() < volunteerId) {
        std::cout << "Volunteer doesn't exist" << std::endl;
        return;
    }

    Volunteer &v = wareHouse.getVolunteer(volunteerId);
    std::cout << v.toString() << std::endl;
}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}

void PrintActionsLog::act(WareHouse &wareHouse) {
    //checking if i can update this by commit
    return;
}

PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

void BackupWareHouse::act(WareHouse &wareHouse) {
    extern WareHouse *backup;
    if (backup != nullptr) {
        delete (backup);
    }
    backup = new WareHouse(wareHouse);
    complete();
}
BackupWareHouse* BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}

void RestoreWareHouse::act(WareHouse &wareHouse) {
    if (backup == nullptr) {
        error("Backup does not exist");
    } else{
        wareHouse = *backup;
        complete();
    }
}

RestoreWareHouse* RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}