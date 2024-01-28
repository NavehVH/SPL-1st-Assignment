#include <string>
#include <iostream>
#include "Action.h"

void Action::complete()
{
    status = ActionStatus::COMPLETED;
}

void Action::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    std::cout << "Error:" + errorMsg << std::endl;
}

string Action::getErrorMsg() const
{
    return errorMsg;
}

ActionStatus Action::getStatus() const
{
    return status;
}

SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps)
{
}

void SimulateStep::act(WareHouse &wareHouse)

{
    for (int i = 0; i < numOfSteps; i++) {
        for (Volunteer *volunteer : wareHouse.getVolunteers()) { // part(3)
            volunteer->step(); // part (2)
            if (volunteer->hasFinishedOrder()) {
                volunteer->acceptOrder(wareHouse.getNextOrder(volunteer)); // part (2) and (1)
                AddOrder(volunteer->getActiveOrderId()); // part (1), Updates to the correct vector of orders
                if (!volunteer->hasOrdersLeft()) { // part(4)
                    wareHouse.DeleteLimitedVolunteer(volunteer);
                }
            }
        }
    }
    complete();
    wareHouse.addAction(this);
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

string SimulateStep::toString() const
{
    return "simulateStep " + numOfSteps;
}

AddOrder::AddOrder(int id) : customerId(id)
{
}
void AddOrder::act(WareHouse &wareHouse)
{
    if (customerId >= wareHouse.getCustomerCounter() || customerId < 0)
    {
        error("Cannot place this order");
        wareHouse.addAction(this);
        return;
    }
    Customer &c = wareHouse.getCustomer(customerId);
    if (c.getId() < 0 || !c.canMakeOrder())
    {
        // don't know how to check if didn't find customer (id = 0?\null?), #TODO: add default return
        error("Cannot place this order");
        wareHouse.addAction(this);
        return;
    }
    Order *o = new Order(wareHouse.getOrderCounter(), customerId, c.getCustomerDistance());
    wareHouse.getPendingOrders().push_back(o);
    wareHouse.setOrderCounter(wareHouse.getOrderCounter() + 1);
    complete();
    wareHouse.addAction(this);
}

string AddOrder::toString() const
{
    return "order " + customerId;
}

AddOrder *AddOrder::clone() const
{
    return new AddOrder(*this);
}

AddCustomer::AddCustomer(string customerName, CustomerType customerType, int distance, int maxOrders)
    : customerName(customerName), customerType(customerType), distance(distance), maxOrders(maxOrders)
{
}

string AddCustomer::toString() const
{
    string s = "customer " + customerName + " ";
    if (customerType == CustomerType::Civilian)
        s += "civilian";
    else
        s += "soldier";
    s += " " + std::to_string(distance) + " " + std::to_string(maxOrders);
    return s;
}

void AddCustomer::act(WareHouse &wareHouse)
{
    Customer *c = nullptr;
    if (customerType == CustomerType::Soldier)
        c = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
    else
        c = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
    wareHouse.getCustomers().push_back(c);
    wareHouse.setCustomerCounter(wareHouse.getCustomerCounter() + 1);
    complete();
    wareHouse.addAction(this);
}

AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
}

PrintOrderStatus::PrintOrderStatus(int id) : orderId(id)
{
}

void PrintOrderStatus::act(WareHouse &wareHouse)
{

    if (wareHouse.getOrderCounter() < orderId)
    {
        std::cout << "Order doesn't exist" << std::endl;
        wareHouse.addAction(this);
        return;
    }

    Order order = wareHouse.getOrder(orderId);
    std::cout << order.toString() << std::endl;
    complete();
    wareHouse.addAction(this);
}

string PrintOrderStatus::toString() const
{
    return "printOrderStatus " + orderId;
}

PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}

PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId)
{
}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    if (wareHouse.getCustomerCounter() < customerId)
    {
        std::cout << "Customer doesn't exist" << std::endl;
        wareHouse.addAction(this);
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
    complete();
    wareHouse.addAction(this);
}

string PrintCustomerStatus::toString() const
{
    return "printCustomerStatus " + customerId;
}

PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}

PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id)
{
}

void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    if (wareHouse.getVolunteerCounter() < volunteerId)
    {
        std::cout << "Volunteer doesn't exist" << std::endl;
        wareHouse.addAction(this);
        return;
    }

    Volunteer &v = wareHouse.getVolunteer(volunteerId);
    std::cout << v.toString() << std::endl;
    complete();
    wareHouse.addAction(this);
}

string PrintVolunteerStatus::toString() const
{
    return "printVolunteerStatus " + volunteerId;
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}

void PrintActionsLog::act(WareHouse &wareHouse)
{
    string s;
    for (Action *a : wareHouse.getActions())
    {
        if (a->getStatus() == ActionStatus::COMPLETED)
            s = "Completed";
        else
            s = "Error: " + getErrorMsg();
        std::cout << a->toString() + s << std::endl;
    }
    complete();
}

string PrintActionsLog::toString() const
{
    return "printActionLog";
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

void BackupWareHouse::act(WareHouse &wareHouse)
{
    extern WareHouse *backup;
    if (backup != nullptr)
    {
        delete (backup);
    }
    backup = new WareHouse(wareHouse);
    complete();
    wareHouse.addAction(this);
}

string BackupWareHouse::toString() const
{
    return "backupWareHouse";
}

BackupWareHouse *BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}

void RestoreWareHouse::act(WareHouse &wareHouse)
{
    if (backup == nullptr)
    {
        error("Backup does not exist");
    }
    else
    {
        wareHouse = *backup;
        complete();
        wareHouse.addAction(this);
    }
}

string RestoreWareHouse::toString() const
{
    return "restoreWareHouse";
}

RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}

void Close::act(WareHouse &WareHouse)
{
    return;
}

string Close::toString() const
{
    return "close";
}

Close *Close::clone() const
{
    return new Close(*this);
}