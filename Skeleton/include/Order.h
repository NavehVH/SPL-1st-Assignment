#pragma once

#include <string>
#include <vector>
using std::string;
using std::vector;

class Volunteer;
#define NO_VOLUNTEER -1

enum class OrderStatus {
    PENDING,
    COLLECTING,
    DELIVERING,
    COMPLETED,
};

class Order {

public:
    Order();
    Order(int id, int customerId, int distance);
    virtual ~Order() {};
    int getId() const;                   
    int getCustomerId() const;
    int getDistance() const;            
    void setStatus(OrderStatus status);   
    void setCollectorId(int collectorId); 
    void setDriverId(int driverId);       
    int getCollectorId() const;
    int getDriverId() const;
    OrderStatus getStatus() const; 
    string enumToString(OrderStatus e) const;
    const string toString() const;

private:
    const int id;         
    const int customerId; 
    const int distance;   
    OrderStatus status;   
    int collectorId;      //Initialized to NO_VOLUNTEER if no collector has been assigned yet
    int driverId;         //Initialized to NO_VOLUNTEER if no driver has been assigned yet
};