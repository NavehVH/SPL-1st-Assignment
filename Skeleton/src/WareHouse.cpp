#include "../include/Order.h"
#include "../include/Volunteer.h"
#include "../include/Customer.h"
#include "../include/Action.h"
#include "../include/WareHouse.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

WareHouse::WareHouse(const string &configFilePath) : isOpen(false), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(0), volunteerCounter(0), orderCounter(0)
{
    std::ifstream file(configFilePath);
    if (file.is_open())
    {
        processFile(file);
        std::cout << "WareHouse is open!" << std::endl;
        open();
        start();
    }
    else
    {
        std::cout << "Failed to open file" << std::endl;
        return;
    }
}

WareHouse::WareHouse(const WareHouse &other) : isOpen(other.isOpen), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter)
{

    for (const Action *a : other.actionsLog)
    {
        actionsLog.push_back(a->clone());
    }
    for (const Volunteer *v : other.volunteers)
    {
        volunteers.push_back(v->clone());
    }
    for (const Order *o : other.pendingOrders)
    {
        pendingOrders.push_back(new Order(*o));
    }
    for (const Order *o : other.inProcessOrders)
    {
        inProcessOrders.push_back(new Order(*o));
    }
    for (const Order *o : other.completedOrders)
    {
        completedOrders.push_back(new Order(*o));
    }
    for (const Customer *c : other.customers)
    {
        customers.push_back(c->clone());
    }
}

WareHouse &WareHouse::operator=(const WareHouse &other)
{
    if (this != &other)
    {

        isOpen = other.isOpen;
        orderCounter = other.orderCounter;
        volunteerCounter = other.volunteerCounter;
        customerCounter = other.customerCounter;

        for (Action *a : actionsLog)
        {
            delete a;
            a = nullptr;
        }
        actionsLog.clear();

        for (Volunteer *v : volunteers)
        {
            delete v;
            v = nullptr;
        }
        volunteers.clear();

        for (Order *o : pendingOrders)
        {
            delete o;
            o = nullptr;
        }
        pendingOrders.clear();

        for (Order *o : inProcessOrders)
        {
            delete o;
            o = nullptr;
        }
        inProcessOrders.clear();

        for (Order *o : completedOrders)
        {
            delete o;
            o = nullptr;
        }
        completedOrders.clear();

        for (Customer *c : customers)
        {
            delete c;
            c = nullptr;
        }
        customers.clear();

        for (const Action *a : other.actionsLog)
        {
            actionsLog.push_back(a->clone());
        }
        for (const Volunteer *v : other.volunteers)
        {
            volunteers.push_back(v->clone());
        }
        for (const Order *o : other.pendingOrders)
        {
            pendingOrders.push_back(new Order(*o));
        }
        for (const Order *o : other.inProcessOrders)
        {
            inProcessOrders.push_back(new Order(*o));
        }
        for (const Order *o : other.completedOrders)
        {
            completedOrders.push_back(new Order(*o));
        }
        for (const Customer *c : other.customers)
        {
            customers.push_back(c->clone());
        }
    }
    return *this;
}

WareHouse::WareHouse(WareHouse &&other) noexcept
    : isOpen(other.isOpen), actionsLog(std::move(other.actionsLog)),
      volunteers(std::move(other.volunteers)),
      pendingOrders(std::move(other.pendingOrders)), inProcessOrders(std::move(other.inProcessOrders)),
      completedOrders(std::move(other.completedOrders)), customers(std::move(other.customers)), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter)
{

    other.actionsLog.clear();
    other.volunteers.clear();
    other.pendingOrders.clear();
    other.inProcessOrders.clear();
    other.completedOrders.clear();
    other.customers.clear();
}

WareHouse::~WareHouse()
{
    for (size_t i = 0; i < actionsLog.size(); ++i)
    {
        delete actionsLog[i];
    }
    actionsLog.clear();

    for (size_t i = 0; i < volunteers.size(); ++i)
    {
        delete volunteers[i];
    }
    volunteers.clear();

    for (size_t i = 0; i < pendingOrders.size(); ++i)
    {
        delete pendingOrders[i];
    }
    pendingOrders.clear();

    for (size_t i = 0; i < inProcessOrders.size(); ++i)
    {
        delete inProcessOrders[i];
    }
    inProcessOrders.clear();

    for (size_t i = 0; i < completedOrders.size(); ++i)
    {
        delete completedOrders[i];
    }
    completedOrders.clear();

    for (size_t i = 0; i < customers.size(); ++i)
    {
        delete customers[i];
    }
    customers.clear();
}

void WareHouse::start()
{
    while (isOpen)
    {
        string userInput, firstWord;
        std::getline(std::cin, userInput);
        std::istringstream iss(userInput);
        std::cout << "" << std::endl; // just empty line
        iss >> firstWord;
        if (firstWord == "step")
        {
            int numberOfSteps;
            iss >> numberOfSteps;
            SimulateStep* simulateStep = new SimulateStep(numberOfSteps);
            simulateStep->act(*this);
        }
        else if (firstWord == "order")
        {
            int customerId;
            iss >> customerId;
            AddOrder* addOrder = new AddOrder (customerId);
            addOrder->act(*this);
        }
        else if (firstWord == "customer")
        {
            string name, type;
            int distance, maxOrders;
            iss >> name >> type >> distance >> maxOrders;
            if (type == "soldier")
            {
                AddCustomer* addCustomer = new AddCustomer(name, CustomerType::Soldier, distance, maxOrders);
                addCustomer->act(*this);
            }
            else if (type == "civilian")
            {
                AddCustomer* addCustomer = new AddCustomer(name, CustomerType::Civilian, distance, maxOrders);
                addCustomer->act(*this);
            }
        }
        else if (firstWord == "orderStatus")
        {
            int orderId;
            iss >> orderId;
            PrintOrderStatus* printOrderStatus = new PrintOrderStatus(orderId);
            printOrderStatus->act(*this);
        }
        else if (firstWord == "customerStatus")
        {
            int customerId;
            iss >> customerId;
            PrintCustomerStatus* printCustomerStatus = new PrintCustomerStatus (customerId);
            printCustomerStatus->act(*this);
        }
        else if (firstWord == "volunteerStatus")
        {
            int volunteerId;
            iss >> volunteerId;
            PrintVolunteerStatus* printVolunteerStatus = new PrintVolunteerStatus(volunteerId);
            printVolunteerStatus->act(*this);
        }
        else if (firstWord == "log")
        {
            PrintActionsLog* printActionsLog = new PrintActionsLog;
            printActionsLog->act(*this);
        }
        else if (firstWord == "close")
        {
            Close close;
            close.act(*this);
        }
        else if (firstWord == "backup")
        {
            BackupWareHouse* backupWareHouse = new BackupWareHouse;
            backupWareHouse->act(*this);
        }
        else if (firstWord == "restore")
        {
            RestoreWareHouse* restoreWareHouse = new RestoreWareHouse;
            restoreWareHouse->act(*this);
        }
        else
        {
            std::cout << "Invalid input" << std::endl;
        }
        std::cout << "" << std::endl; // just empty line
    }
}

const vector<Action *> &WareHouse::getActions() const
{
    return actionsLog;
}

void WareHouse::addOrder(Order *order)
{
    if (std::find(pendingOrders.begin(), pendingOrders.end(), order) != pendingOrders.end())
    {
        if (order->getStatus() == OrderStatus::PENDING)
        {
            pendingOrders.erase(std::remove(pendingOrders.begin(), pendingOrders.end(), order), pendingOrders.end());
            inProcessOrders.push_back(order);
            order->setStatus(OrderStatus::COLLECTING);
        }
        else if (order->getStatus() == OrderStatus::COLLECTING)
        {
            pendingOrders.erase(std::remove(pendingOrders.begin(), pendingOrders.end(), order), pendingOrders.end());
            inProcessOrders.push_back(order);
            order->setStatus(OrderStatus::DELIVERING);
        }
    }
}

void WareHouse::setOrder(Order *order)
{
    if (std::find(inProcessOrders.begin(), inProcessOrders.end(), order) != inProcessOrders.end())
    {
        if (order->getStatus() == OrderStatus::COLLECTING)
        {
            inProcessOrders.erase(std::remove(inProcessOrders.begin(), inProcessOrders.end(), order), inProcessOrders.end());
            pendingOrders.push_back(order);
        }
        else if (order->getStatus() == OrderStatus::DELIVERING)
        {
            inProcessOrders.erase(std::remove(inProcessOrders.begin(), inProcessOrders.end(), order), inProcessOrders.end());
            completedOrders.push_back(order);
            order->setStatus(OrderStatus::COMPLETED);
        }
    }
}

void WareHouse::addAction(Action *action)
{
    actionsLog.push_back(action);
}

Customer &WareHouse::getCustomer(int customerId) const
{
    for (Customer *c : customers)
    {
        if (c->getId() == customerId)
        {
            return *c;
        }
    }
    static CivilianCustomer civilianCustomerDefault;
    return civilianCustomerDefault;
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const
{
    for (Volunteer *v : volunteers)
    {
        if (v->getId() == volunteerId)
        {
            return *v;
        }
    }
    static CollectorVolunteer CollectorVolunteerDefault;
    return CollectorVolunteerDefault;
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
    static Order orderDefault;
    return orderDefault;
}

void WareHouse::close()
{
    isOpen = false;
}

void WareHouse::open()
{
    isOpen = true;
}

int WareHouse::getOrderCounter()
{
    return orderCounter;
}

int WareHouse::getCustomerCounter()
{
    return customerCounter;
}

int WareHouse::getVolunteerCounter()
{
    return volunteerCounter;
}

void WareHouse::setOrderCounter(int counter)
{
    orderCounter = counter;
}

void WareHouse::setCustomerCounter(int counter)
{
    customerCounter = counter;
}

void WareHouse::DeleteLimitedVolunteer(Volunteer *v)
{
    if (std::find(volunteers.begin(), volunteers.end(), v) != volunteers.end())
    {
        volunteers.erase(std::remove(volunteers.begin(), volunteers.end(), v), volunteers.end());
    }
    delete v;
}

vector<Order *> &WareHouse::getPendingOrders()
{
    return pendingOrders;
}

vector<Order *> &WareHouse::getInProcessOrders()
{
    return inProcessOrders;
}

vector<Volunteer *> &WareHouse::getVolunteers()
{
    return volunteers;
}

vector<Customer *> &WareHouse::getCustomers()
{
    return customers;
}

void WareHouse::processFile(std::ifstream &inFile)
{
    std::string line, word, firstWord, name, type;
    int coolDown, maxOrders, distance, maxDistance, distancePerStep;
    while (getline(inFile, line))
    {
        std::istringstream iss(line);
        iss >> firstWord;
        if (firstWord == "customer")
        {
            iss >> name >> type >> distance >> maxOrders;
            if (type == "soldier")
            {
                SoldierCustomer *soldierCustomer = new SoldierCustomer(customerCounter, name, distance, maxOrders);
                customers.push_back(soldierCustomer);
                customerCounter++;
            }
            else if (type == "civilian")
            {
                CivilianCustomer *civilianCustomer = new CivilianCustomer(customerCounter, name, distance, maxOrders);
                customers.push_back(civilianCustomer);
                customerCounter++;
            }
        }
        else if (firstWord == "volunteer")
        {
            iss >> name >> type;
            if (type == "limited_collector")
            {
                iss >> coolDown >> maxOrders;
                LimitedCollectorVolunteer *limitedCollectorVolunteer = new LimitedCollectorVolunteer(volunteerCounter, name, coolDown, maxOrders);
                volunteers.push_back(limitedCollectorVolunteer);
                volunteerCounter++;
            }
            else if (type == "collector")
            {
                iss >> coolDown;
                CollectorVolunteer *collectorVolunteer = new CollectorVolunteer(volunteerCounter, name, coolDown);
                volunteers.push_back(collectorVolunteer);
                volunteerCounter++;
            }
            else if (type == "limited_driver")
            {
                iss >> maxDistance >> distancePerStep >> maxOrders;
                LimitedDriverVolunteer *limitedDriverVolunteer = new LimitedDriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep, maxOrders);
                volunteers.push_back(limitedDriverVolunteer);
                volunteerCounter++;
            }
            else if (type == "driver")
            {
                iss >> maxDistance >> distancePerStep;
                DriverVolunteer *driverVolunteer = new DriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep);
                volunteers.push_back(driverVolunteer);
                volunteerCounter++;
            }
        }
    }
    inFile.close();
}