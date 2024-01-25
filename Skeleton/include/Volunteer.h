#pragma once

#include <string>
#include <vector>
#include "Order.h"
using std::string;
using std::vector;

#define NO_ORDER -1

class Volunteer {
    public:
        Volunteer(int id, const string &name);
        virtual ~Volunteer() {};
        int getId() const; //d
        const string &getName() const; //d
        int getActiveOrderId() const; //d
        int getCompletedOrderId() const; //d
        bool isBusy() const; //d Signal whether the volunteer is currently processing an order    
        virtual bool hasOrdersLeft() const = 0; //d Signal whether the volunteer didn't reach orders limit,Always true for CollectorVolunteer and DriverVolunteer
        bool hasFinishedOrder() const;
        virtual bool canTakeOrder(const Order &order) const = 0; //d Signal if the volunteer can take the order.      
        virtual void acceptOrder(const Order &order) = 0; //d Prepare for new order(Reset activeOrderId,TimeLeft,DistanceLeft,OrdersLeft depends on the volunteer type)
                
        virtual void step() = 0; //d Simulate volunteer step,if the volunteer finished the order, transfer activeOrderId to completedOrderId

        virtual string toString() const = 0; //d
        virtual Volunteer* clone() const = 0; //d Return a copy of the volunteer

    protected:
        int completedOrderId; //Initialized to NO_ORDER if no order has been completed yet
        int activeOrderId; //Initialized to NO_ORDER if no order is being processed
    
    private:
        const int id;
        const string name;

};


class CollectorVolunteer: public Volunteer {

    public:
        CollectorVolunteer(int id, string name, int coolDown); //d
        CollectorVolunteer *clone() const override; //d
        void step() override; //d
        int getCoolDown() const; //d
        int getTimeLeft() const; //d
        bool decreaseCoolDown();//d Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
        bool hasOrdersLeft() const override; //d
        bool canTakeOrder(const Order &order) const override;
        void acceptOrder(const Order &order) override;
        string toString() const override;
    
    private:
        const int coolDown; // The time it takes the volunteer to process an order
        int timeLeft; // Time left until the volunteer finishes his current order
};

class LimitedCollectorVolunteer: public CollectorVolunteer {

    public:
        LimitedCollectorVolunteer(int id, string name, int coolDown ,int maxOrders); //d
        LimitedCollectorVolunteer *clone() const override; //d
        bool hasOrdersLeft() const override; //TODO
        bool canTakeOrder(const Order &order) const override; //d
        void acceptOrder(const Order &order) override;

        int getMaxOrders() const; //d
        int getNumOrdersLeft() const; //d
        string toString() const override; //d
    
    private:
        const int maxOrders; // The number of orders the volunteer can process in the whole simulation
        int ordersLeft; // The number of orders the volunteer can still take
};

class DriverVolunteer: public Volunteer {

    public:
        DriverVolunteer(int id, string name, int maxDistance, int distancePerStep); //d
        DriverVolunteer *clone() const override; //d

        int getDistanceLeft() const; //d
        int getMaxDistance() const; //d
        int getDistancePerStep() const;   //d
        bool decreaseDistanceLeft(); //d Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
        bool hasOrdersLeft() const override; //d
        bool canTakeOrder(const Order &order) const override; //d Signal if the volunteer is not busy and the order is within the maxDistance
        void acceptOrder(const Order &order) override; //d Assign distanceLeft to order's distance
        void step() override; //d Decrease distanceLeft by distancePerStep
        string toString() const override; //d

    private:
        const int maxDistance; // The maximum distance of ANY order the volunteer can take
        const int distancePerStep; // The distance the volunteer does in one step
        int distanceLeft; // Distance left until the volunteer finishes his current order
};

class LimitedDriverVolunteer: public DriverVolunteer {

    public:
        LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders);
        LimitedDriverVolunteer *clone() const override;
        int getMaxOrders() const;
        int getNumOrdersLeft() const;
        bool hasOrdersLeft() const override;
        bool canTakeOrder(const Order &order) const override; // Signal if the volunteer is not busy, the order is within the maxDistance.
        void acceptOrder(const Order &order) override; // Assign distanceLeft to order's distance and decrease ordersLeft
        string toString() const override;

    private:
        const int maxOrders; // The number of orders the volunteer can process in the whole simulation
        int ordersLeft; // The number of orders the volunteer can still take
};