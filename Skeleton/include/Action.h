#pragma once
#include <string>
#include <vector>
#include "WareHouse.h"
using std::string;
using std::vector;

enum class ActionStatus
{
    COMPLETED,
    ERROR
};

enum class CustomerType
{
    Soldier,
    Civilian
};

class Customer;

extern WareHouse * backup;

class Action
{
public:
    Action();
    virtual ~Action();
    ActionStatus getStatus() const;
    virtual void act(WareHouse &wareHouse) = 0;
    virtual string toString() const = 0;
    virtual Action *clone() const = 0;

protected:
    void complete();
    void error(string errorMsg);
    string getErrorMsg() const;

private:
    string errorMsg;
    ActionStatus status;
};

class SimulateStep : public Action
{

public:
    SimulateStep(int numOfSteps);
    void act(WareHouse &wareHouse) override;
    std::string toString() const override;
    SimulateStep *clone() const override;

private:
    const int numOfSteps;
};

class AddOrder : public Action
{
public:
    AddOrder(int id);
    void act(WareHouse &wareHouse) override;
    AddOrder *clone() const override;
    string toString() const override;

private:
    const int customerId;
};

class AddCustomer : public Action
{
public:
    AddCustomer(string customerName, string customerType, int distance, int maxOrders);
    void act(WareHouse &wareHouse) override;
    AddCustomer *clone() const override;
    string toString() const override;

private:
    const string customerName;
    const CustomerType customerType;
    const int distance;
    const int maxOrders;
};

class PrintOrderStatus : public Action
{
public:
    PrintOrderStatus(int id);
    void act(WareHouse &wareHouse) override;
    PrintOrderStatus *clone() const override;
    string toString() const override;

private:
    const int orderId;
};

class PrintCustomerStatus : public Action
{
public:
    PrintCustomerStatus(int customerId);
    void act(WareHouse &wareHouse) override;
    PrintCustomerStatus *clone() const override;
    string toString() const override;

private:
    const int customerId;
};

class PrintVolunteerStatus : public Action
{
public:
    PrintVolunteerStatus(int id);
    void act(WareHouse &wareHouse) override;
    PrintVolunteerStatus *clone() const override;
    string toString() const override;

private:
    const int volunteerId;
};

class PrintActionsLog : public Action
{
public:
    PrintActionsLog();
    void act(WareHouse &wareHouse) override;
    PrintActionsLog *clone() const override;
    string toString() const override;

private:
};

class Close : public Action
{
public:
    Close();
    void act(WareHouse &wareHouse) override;
    string toString() const override;

private:
};

class BackupWareHouse : public Action
{
public:
    BackupWareHouse();
    void act(WareHouse &wareHouse) override;
    BackupWareHouse *clone() const override;
    string toString() const override;

private:
};

class RestoreWareHouse : public Action
{
public:
    RestoreWareHouse();
    void act(WareHouse &wareHouse) override;
    RestoreWareHouse *clone() const override;
    string toString() const override;

private:
};