#pragma once

#include <string>
#include <vector>
#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"
using std::string;
using std::vector;

class Action;
class Volunteer;

// Warehouse responsible for Volunteers, Customers and Actions.

class WareHouse {

public:
    WareHouse(const string &configFilePath);
    WareHouse(const WareHouse &other);
    WareHouse& operator = (const WareHouse &other);
    WareHouse (WareHouse&& other) noexcept;
    WareHouse& operator = (WareHouse&& other) noexcept;
    virtual ~WareHouse();   
    void start();
    const vector<Action *> &getActions() const; //d
    void addOrder(Order *order);
    void addAction(Action *action);
    void printActionsLogs();
    Customer &getCustomer(int customerId) const; //d
    Volunteer &getVolunteer(int volunteerId) const; //d
    Order &getOrder(int orderId) const; //d
    void close();
    void open();
    int getOrderCounter();
    void setOrderCounter(int orderC);
    int getCustomerCounter();
    void setCustomerCounter(int customerC);
    int getVolunteerCounter();
    Order &getNextOrder(Volunteer *v);
    void DeleteLimitedVolunteer(Volunteer *v);
    
    vector<Order *> &getPendingOrders(); // added by myself
    vector<Order *> &getInProcessOrders();
    vector<Order *> &getCompletedOrders();
    vector<Volunteer *> &getVolunteers();
    vector<Customer *> &getCustomers();

private:
    void processFile(std::ifstream& inFile);
    bool isOpen;                    
    vector<Action *> actionsLog; 
    vector<Volunteer *> volunteers;  
    vector<Order *> pendingOrders;   
    vector<Order *> inProcessOrders; 
    vector<Order *> completedOrders; 
    vector<Customer *> customers;    
    int customerCounter;             // For assigning unique customer IDs
    int volunteerCounter;            //For assigning unique volunteer IDs
    int orderCounter;                //For assinging unique order IDs
};