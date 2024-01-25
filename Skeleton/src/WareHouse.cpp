#include "Order.h"
#include "Volunteer.h"
#include "Customer.h"
#include "Action.h"
#include "WareHouse.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string> 
#include <algorithm>
#include <utility>




WareHouse::WareHouse(const string &configFilePath){
    std::ifstream file(configFilePath);
    if (file.is_open()){
        std::cout << "Successfully opened file" << std::endl;
        processFile(file);
        std::cout << "WareHouse is open" << std::endl;
        open();
        start();

    }
    else {
        std::cout << "Failed to open file" << std::endl; 
        exit(1);
    }
}
WareHouse::WareHouse(const WareHouse &other): orderCounter(other.orderCounter), volunteerCounter(other.volunteerCounter), customerCounter(other.customerCounter), isOpen(other.isOpen) {
    for (const Action* a : other.actionsLog) {
            actionsLog.push_back(a->clone());
        }
        for (const Volunteer* v : other.volunteers) {
            volunteers.push_back(v->clone());
        }
        for (const Order* o : other.pendingOrders) {
            pendingOrders.push_back(new Order(*o));
        }
        for (const Order* o : other.inProcessOrders) {
            inProcessOrders.push_back(new Order(*o));
        }
        for (const Order* o : other.completedOrders) {
            completedOrders.push_back(new Order(*o));
        }
        for (const Customer* c : other.customers) {
            customers.push_back(c->clone());
        }
}

WareHouse& WareHouse::operator=(const WareHouse& other) {
    if (this != &other) {

        isOpen = other.isOpen;
        orderCounter = other.orderCounter;
        volunteerCounter = other.volunteerCounter;
        customerCounter = other.customerCounter;

        for (Action* a : actionsLog) {
            delete a;
        }
        actionsLog.clear();

        for (Volunteer* v : volunteers) {
            delete v;
        }
        volunteers.clear();

        for (Order* o : pendingOrders) {
            delete o;
        }
        pendingOrders.clear();

        for (Order* o : inProcessOrders) {
            delete o;
        }
        inProcessOrders.clear();

        for (Order* o : completedOrders) {
            delete o;
        }
        completedOrders.clear();

        for (Customer* c : customers) {
            delete c;
        }
        customers.clear();

        for (const Action* a : other.actionsLog) {
            actionsLog.push_back(a->clone());
        }
        for (const Volunteer* v : other.volunteers) {
            volunteers.push_back(v->clone());
        }
        for (const Order* o : other.pendingOrders) {
            pendingOrders.push_back(new Order(*o));
        }
        for (const Order* o : other.inProcessOrders) {
            inProcessOrders.push_back(new Order(*o));
        }
        for (const Order* o : other.completedOrders) {
            completedOrders.push_back(new Order(*o));
        }
        for (const Customer* c : other.customers) {
            customers.push_back(c->clone());
        }
    }
    return *this;
}

WareHouse::WareHouse(WareHouse&& other) noexcept : isOpen(other.isOpen), orderCounter(other.orderCounter), volunteerCounter(other.volunteerCounter), customerCounter(other.customerCounter), actionsLog(std::move(other.actionsLog)), volunteers(std::move(other.volunteers)), pendingOrders(std::move(other.pendingOrders)), inProcessOrders(std::move(other.inProcessOrders)), completedOrders(std::move(other.completedOrders)), customers(std::move(other.customers)) {
    other.actionsLog.clear();
    other.volunteers.clear();
    other.pendingOrders.clear();
    other.inProcessOrders.clear();
    other.completedOrders.clear();
    other.customers.clear();
}

WareHouse& WareHouse::operator=(WareHouse&& other) noexcept {
    if (this != &other) {
        for (Action* a : actionsLog) {
            delete a;
        }
        actionsLog.clear();

        for (Volunteer* v : volunteers) {
            delete v;
        }
        volunteers.clear();

        for (Order* o : pendingOrders) {
            delete o;
        }
        pendingOrders.clear();

        for (Order* o : inProcessOrders) {
            delete o;
        }
        inProcessOrders.clear();

        for (Order* o : completedOrders) {
            delete o;
        }
        completedOrders.clear();

        for (Customer* c : customers) {
            delete c;
        customers.clear();
        }
        isOpen = std::move(other.isOpen);
        orderCounter = std::move(other.orderCounter);
        volunteerCounter = std::move(other.volunteerCounter);
        customerCounter = std::move(other.customerCounter);
        actionsLog = std::move(other.actionsLog);
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);
        customers = std::move(other.customers);
    }
    return *this;
}

WareHouse::~WareHouse(){
    for(Action* a: actionsLog){
        if(a)
            delete a;
            a = nullptr;
    }
    for(Volunteer* a: volunteers){
        if(a)
            delete a;
            a = nullptr;
    }
    for(Order* a: pendingOrders){
        if(a)
            delete a;
            a = nullptr;
    }
    for(Order* a: inProcessOrders){
        if(a)
            delete a;
            a = nullptr;
    }
    for(Order* a: completedOrders){
        if(a)
            delete a;
            a = nullptr;
    }
    for(Customer* a: customers){
        if(a)
            delete a;
            a = nullptr;
    }
}

void WareHouse::processFile(std::ifstream& inFile) {
    std::string line, word, firstWord, name, type;
    int coolDown, maxOrders, distance, maxDistance, distancePerStep; // Removed duplicate declaration of maxOrders

    while (getline(inFile, line)) {
        std::istringstream iss(line);
        iss >> firstWord;
        if (firstWord == "customer") {
            iss >> name >> type >> distance >> maxOrders;
            if (type == "soldier") {
                SoldierCustomer* soldierCustomer = new SoldierCustomer(customerCounter, name, distance, maxOrders);
                customers.push_back(soldierCustomer);
            } else {
                CivilianCustomer* civilianCustomer = new CivilianCustomer(customerCounter, name, distance, maxOrders);
                customers.push_back(civilianCustomer);
            }
            customerCounter++;
        } else {
            iss >> name >> type;
            if (type == "limited_collector") {
                iss >> coolDown >> maxOrders;
                LimitedCollectorVolunteer* limitedCollectorVolunteer = new LimitedCollectorVolunteer(volunteerCounter, name, coolDown, maxOrders);
                volunteers.push_back(limitedCollectorVolunteer);
                volunteerCounter++;
            } else if (type == "collector") {
                iss >> coolDown;
                CollectorVolunteer* collectorVolunteer = new CollectorVolunteer(volunteerCounter, name, coolDown);
                volunteers.push_back(collectorVolunteer);
                volunteerCounter++;
            } else if (type == "limited_driver") {
                iss >> maxDistance >> distancePerStep >> maxOrders;
                LimitedDriverVolunteer* limitedDriverVolunteer = new LimitedDriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep, maxOrders);
                volunteers.push_back(limitedDriverVolunteer);
                volunteerCounter++;
            } else {
                iss >> maxDistance >> distancePerStep;
                DriverVolunteer* driverVolunteer = new DriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep);
                volunteers.push_back(driverVolunteer);
                volunteerCounter++;
            }
        }
    }
}

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

void WareHouse::open () {
    isOpen = true;
}

// void WareHouse::close() {
//     Close close;
//     close.act(*this);
// }

void WareHouse::start() {
    while (isOpen) {
        string userInput, firstWord;
        std::getline(std::cin, userInput);
        std::istringstream iss;
        if (firstWord == "step") {
            int numberOfSteps;
            iss >> numberOfSteps;
            SimulateStep simulateStep(numberOfSteps);
            simulateStep.act(*this);
        } else if (firstWord == "order") {
            int customerId;
            iss >> customerId;
            AddOrder addOrder(customerId);
            addOrder.act(*this);
        } else if (firstWord == "customer") {
            string name, type;
            int distance, maxOrders;
            iss >> name >> type >> distance >> maxOrders;
            if (type == "soldier") {
                SoldierCustomer* soldierCustomer = new SoldierCustomer(customerCounter, name, distance, maxOrders);
                customers.push_back(soldierCustomer);
            } else {
                CivilianCustomer* civilianCustomer = new CivilianCustomer(customerCounter, name, distance, maxOrders);
                customers.push_back(civilianCustomer);
            }
            customerCounter++;
        } else if (firstWord == "OrderStatus") {
            int orderId;
            iss >> orderId;
            PrintOrderStatus printOrderStatus(orderId);
            printOrderStatus.act(*this);
        } else if (firstWord == "customerStatus") {
            int customerId;
            iss >> customerId;
            PrintCustomerStatus printCustomerStatus(customerId);
            printCustomerStatus.act(*this);
        } else if (firstWord == "volunteerStatus") {
            int volunteerId;
            iss >> volunteerId;
            PrintVolunteerStatus printVolunteerStatus(volunteerId);
            printVolunteerStatus.act(*this);
        } else if (firstWord == "log") {
            PrintActionsLog printActionsLog;
            printActionsLog.act(*this);
        // } else if (firstWord == "close") {
        //     Close close;
        //     close.act(*this);
        } else if (firstWord == "backup") {
            BackupWareHouse backupWareHouse;
            backupWareHouse.act(*this);
        } else if (firstWord == "restore") {
            RestoreWareHouse restoreWareHouse;
            restoreWareHouse.act(*this);
        } else {
            std::cout << "Invalid input" << std::endl;
        }
    }
}