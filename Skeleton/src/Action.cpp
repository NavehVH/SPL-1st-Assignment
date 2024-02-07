#include <string>
#include <iostream>
#include "../include/Action.h"

ActionStatus Action::getStatus() const
{
    return status;
}

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

SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps)
{
}
//STIMULATESTEP
void SimulateStep::act(WareHouse &wareHouse)
{
    for (int i = 0; i < numOfSteps; i++)
    {
        for (Order *o : wareHouse.getPendingOrders()) // step (1)
        {
            for (Volunteer *v : wareHouse.getVolunteers())
            {
                if (v->canTakeOrder(*o))
                {
                    v->acceptOrder(*o);
                    wareHouse.addOrder(o);
                    if (o->getStatus() == OrderStatus::COLLECTING)
                        o->setCollectorId(v->getId());
                    if (o->getStatus() == OrderStatus::DELIVERING)
                        o->setDriverId(v->getId());
                    break;
                }
            }
        }

        for (Order *o : wareHouse.getInProcessOrders())
            if (wareHouse.isInPending(o))
                wareHouse.eraseFromPending(o);

        for (Volunteer *v : wareHouse.getVolunteers()) // step (2)
            v->step();

        for (Volunteer *v : wareHouse.getVolunteers()) // step (3)
        {
            if (v->hasFinishedOrder())
            {
                Order *o = &wareHouse.getOrder(v->getActiveOrderId());
                if (v->getActiveOrderId() == -1 || o->getId() == -1)
                    continue;
                wareHouse.setOrder(o);
                v->setActiveOrderId(NO_ORDER);
            }
        }

        for (Volunteer *v : wareHouse.getVolunteers())
            if (!v->hasOrdersLeft() && !v->isBusy()) // step (4)
                wareHouse.DeleteLimitedVolunteer(v);
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
    return "simulateStep " + std::to_string(numOfSteps);
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
    if (!c.canMakeOrder())
    {
        error("Cannot place this order");
        wareHouse.addAction(this);
        return;
    }
    Order *o = new Order(wareHouse.getOrderCounter(), customerId, c.getCustomerDistance());
    c.PushToOrdersIds(o->getId());
    wareHouse.getPendingOrders().push_back(o);
    wareHouse.setOrderCounter(wareHouse.getOrderCounter() + 1);
    complete();
    wareHouse.addAction(this);
}

AddOrder *AddOrder::clone() const
{
    return new AddOrder(*this);
}

string AddOrder::toString() const
{
    return "order " + std::to_string(customerId);
}

AddCustomer::AddCustomer(string customerName, CustomerType customerType, int distance, int maxOrders)
    : customerName(customerName), customerType(customerType), distance(distance), maxOrders(maxOrders)
{
}

void AddCustomer::act(WareHouse &wareHouse)
{
    Customer *c = nullptr;
    if (customerType == CustomerType::Soldier)
        c = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
    else if (customerType == CustomerType::Civilian) {
        c = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
    }
    wareHouse.getCustomers().push_back(c);
    wareHouse.setCustomerCounter(wareHouse.getCustomerCounter() + 1);
    complete();
    wareHouse.addAction(this);
}

AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
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


PrintOrderStatus::PrintOrderStatus(int id) : orderId(id)
{
}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
    if (orderId >= wareHouse.getOrderCounter() || orderId < 0)
    {
        error("Order doesn't exist");
        wareHouse.addAction(this);
        return;
    }
    Order order = wareHouse.getOrder(orderId);
    std::cout << order.toString() << std::endl;
    complete();
    wareHouse.addAction(this);
}

PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const
{
    return "orderStatus " + std::to_string(orderId);
}

PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId)
{
}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    if (customerId >= wareHouse.getCustomerCounter() || customerId < 0)
    {
        error("Customer doesn't exist");
        wareHouse.addAction(this);
        return;
    }
    std::cout << "CustomerID: " + std::to_string(customerId) << std::endl;

    for (int orderId : wareHouse.getCustomer(customerId).getOrdersIds())
    {
        Order order = wareHouse.getOrder(orderId);
        std::cout << "OrderId: " + std::to_string(order.getId()) << std::endl;
        std::cout << "OrderStatus: " + order.enumToString(order.getStatus()) << std::endl;
    }

    int numOrdersLeft = wareHouse.getCustomer(customerId).getMaxOrders() - wareHouse.getCustomer(customerId).getOrdersIds().size();
    std::cout << "numOrdersLeft: " + std::to_string(numOrdersLeft) << std::endl;
    complete();
    wareHouse.addAction(this);
}

PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const
{
    return "customerStatus " + std::to_string(customerId);
}

PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id)
{
}

void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    if (volunteerId >= wareHouse.getVolunteerCounter() || volunteerId < 0)
    {
        error("Volunteer doesn't exist");
        wareHouse.addAction(this);
        return;
    }
    Volunteer &v = wareHouse.getVolunteer(volunteerId);
    if (v.getId() == -1) 
    {
        error("Volunteer doesn't exist");
        wareHouse.addAction(this);
        return;
    }
    std::cout << v.toString() << std::endl;
    complete();
    wareHouse.addAction(this);
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const
{
    return "volunteerStatus " + std::to_string(volunteerId);
}

void PrintActionsLog::act(WareHouse &wareHouse)
{
    string s;
    for (Action *a : wareHouse.getActions())
    {
        if (a->getStatus() == ActionStatus::COMPLETED)
            s = " COMPLETED";
        else
            s = " ERROR" + getErrorMsg();
        std::cout << a->toString() + s + '\n';
    }
    complete();
    wareHouse.addAction(this);
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const
{
    return "log";
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

BackupWareHouse *BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const
{
    return "backupWareHouse";
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

RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const
{
    return "restoreWareHouse";
}

void Close::act(WareHouse &wareHouse)
{
    for (Customer *c : wareHouse.getCustomers())
    {
        for (int orderId : c->getOrdersIds())
        {
            Order order = wareHouse.getOrder(orderId);
            std::cout << "orderID: " << std::to_string(order.getId()) << ", CustomerID: " << std::to_string(order.getCustomerId()) << ", Status: " <<  order.enumToString(order.getStatus()) << std::endl;
        }
    }
    wareHouse.close();
}

Close *Close::clone() const
{
    return new Close(*this);
}

string Close::toString() const
{
    return "close";
}