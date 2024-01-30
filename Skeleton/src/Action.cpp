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
    std::cout << "Error: " + errorMsg << std::endl;
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
            Order &o = wareHouse.getNextOrder(volunteer);
            std::cout << "DOING: orderId: " + std::to_string(o.getId()) + ", volunteerId: " + std::to_string(volunteer->getId()) + "" << std::endl;
            if (o.getId() == -1) {
                delete &o; //#TODO: Check how to delete this correctly
                continue;
            }
            if (volunteer->hasFinishedOrder() && volunteer->canTakeOrder(o)) {
                if (typeid(*volunteer) == typeid(DriverVolunteer) || typeid(*volunteer) == typeid(LimitedDriverVolunteer))
                    if (wareHouse.getVolunteer(o.getCollectorId()).getCompletedOrderId() != wareHouse.getVolunteer(o.getCollectorId()).getActiveOrderId())
                        continue;
                volunteer->acceptOrder(o); // part (2) and (1)
                wareHouse.addOrder(&o); // part (1), Updates to the correct vector of orders
                if (o.getStatus() == OrderStatus::COLLECTING)
                    o.setCollectorId(volunteer->getId());
                if (o.getStatus() == OrderStatus::DELIVERING)
                    o.setDriverId(volunteer->getId());
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

    Order order = wareHouse.getOrder(orderId);
    if (order.getId() == -1)
    {
        std::cout << "Order doesn't exist" << std::endl;
        wareHouse.addAction(this);
        delete &order;
        return;
    }
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
    if (wareHouse.getCustomer(customerId).getId() == -1)
    {
        std::cout << "Customer doesn't exist" << std::endl;
        wareHouse.addAction(this);
        delete &wareHouse.getCustomer(customerId);
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
    Volunteer &v = wareHouse.getVolunteer(volunteerId);
    if (v.getId() == -1)
    {
        std::cout << "Volunteer doesn't exist" << std::endl;
        wareHouse.addAction(this);
        delete &v;
        return;
    }
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
    exit(1);
}

string Close::toString() const
{
    return "close";
}

Close *Close::clone() const
{
    return new Close(*this);
}