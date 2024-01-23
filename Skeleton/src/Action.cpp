#include <string>
#include <iostream>
#include "../include/Action.h"

void Action::complete()
{
    status = ActionStatus::COMPLETED;
}

void Action::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    std::cout << "Error:" + errorMsg << std::endl;
    return;
}

string Action::getErrorMsg() const
{
    return errorMsg;
}

ActionStatus Action::getStatus() const
{
    return status;
}

void SimulateStep::act(WareHouse &wareHouse)
{
    for (int i = 0; i < numOfSteps; i++)
    {
        for (Volunteer *volunteer : wareHouse.getVolunteers()) // part(3)
        {
            volunteer->step(); // part (2)
            if (volunteer->hasFinishedOrder())
            {
                //volunteer->acceptOrder();     // part (2) and (1) #TODO: Do a method that gets the most needed to finish order, easy
                AddOrder(getActiveOrderId()); // part (1), Updates to the correct vector of orders
                if (!volunteer->hasOrdersLeft())
                { // part(4)
                    // DELETE THE BITCH
                }
            }
        }
    }
}

void AddOrder::act(WareHouse &wareHouse)
{
    Customer &c = wareHouse.getCustomer(customerId);
    if (c.getId() == 0 || !c.canMakeOrder())
    { // don't know how to check if didn't find customer (id = 0?\null?), #TODO: add default return
        error("Cannot place this order");
        return;
    }

    wareHouse.setOrderCounter(wareHouse.getOrderCounter() + 1);
    Order *o = new Order(wareHouse.getOrderCounter(), customerId, c.getCustomerDistance());
    wareHouse.getPendingOrders().push_back(o);
}

// AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders): Action(), customerName(customerName), customerType(customerType), distance(distance), maxOrders(maxOrders) {}

void AddCustomer::act(WareHouse &wareHouse)
{
    wareHouse.setCustomerCounter(wareHouse.getCustomerCounter() + 1);
    Customer *c = nullptr;
    if (customerType == CustomerType::Soldier)
        c = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
    else
        c = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
}

void PrintOrderStatus::act(WareHouse &wareHouse)
{

    if (wareHouse.getOrderCounter() < orderId)
    {
        std::cout << "Order doesn't exist" << std::endl;
        return;
    }

    Order order = wareHouse.getOrder(orderId);
    std::cout << order.toString() << std::endl;
}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    if (wareHouse.getCustomerCounter() < customerId)
    {
        std::cout << "Customer doesn't exist" << std::endl;
        return;
    }
    std::cout << "CustomerID: " + customerId << std::endl;

    for (int orderId : wareHouse.getCustomer(customerId).getOrdersIds())
    {
        Order order = wareHouse.getOrder(orderId);
        std::cout << order.toString() << std::endl;
    }

    int numOrdersLeft = wareHouse.getCustomer(customerId).getMaxOrders() - wareHouse.getCustomer(customerId).getOrdersIds().size();
    std::cout << "numOrdersLeft: " + numOrdersLeft << std::endl;
}

void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    if (wareHouse.getVolunteerCounter() < volunteerId)
    {
        std::cout << "Volunteer doesn't exist" << std::endl;
        return;
    }

    Volunteer &v = wareHouse.getVolunteer(volunteerId);
    std::cout << v.toString() << std::endl;
}

//im lazy af
void PrintActionsLog::act(WareHouse &wareHouse) {
    //checking if i can update this by commit
    return;
}

// BackupWareHouse
BackupWareHouse::BackupWareHouse() : Action() {}

void BackupWareHouse::act(WareHouse &wareHouse)
{
    extern WareHouse *backup;
    if (backup != nullptr)
    {
        delete (backup);
    }
    backup = new WareHouse(wareHouse);
    complete();
}

/// RestoreWareHouse
RestoreWareHouse::RestoreWareHouse() : Action() {}

void RestoreWareHouse::act(WareHouse &wareHouse)
{
    if (backup == nullptr)
        error("Backup does not exist");
    else
    {
        wareHouse = *backup;
        complete();
    }
}