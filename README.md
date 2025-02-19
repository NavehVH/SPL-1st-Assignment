# Food-Warehouse-Management-System
BGU System Programming (SPL) 1st assignment. Written in C++11 and build with Makefile.
This is a simulation game about managing a warehouse, creating orders and instructing the volunteers to make and ship the orders to clients.

# How to Run
1. Clone the repository to your local machine.
2. Navigate to the project directory in your terminal.
3. Compile the source files using the provided makefile:

> make

4. Run the compiled executable with the path to the configuration file as the first command-line argument:

```
bash
./bin/warehouse <path_to_configuration_file>
```

# Project Overview
## Project Objectives
The objective of this assignment is to design an object-oriented system and gain implementation experience in C++ while using classes, standard data structures, and unique C++ properties such as the “Rule of 5”.


## Assignment	Definition
In a faraway Country, SPLand, A war is going on for few months. As part of the efforts to contribute and take care of the country’s citizens, few citizens decided to establish a food warehouse that supplies others food packages. The center is operated by volunteers in different roles, and their job is to handle orders made by the customers.

## The Program Flow
The program receives the path of the config file as the first command-line argument:
> string configurationFile=argv[1]

The config file describes the initial state of the warehouse before we start the simulation. It
contains info of the initial customers and volunteers.
Once the program starts, it initializes the warehouse according to a given config file, and then
starts the simulation by calling the `start()` method of Warehouse, followed by printing to
console: **“Warehouse is open!“**.

Then, the program waits for the user to enter an action to execute. After each executed action,
the program waits for the next action in a loop.

## Config file format
The input file contains the data of the initial program, each in a single line, by the following order:
- **Customers** – each line describes a customer in the following pattern:

> customer <customer_name> <customer_type> <customer_distance> <max_orders>

For example:

> customer Maya soldier 4 2 //Maya is a soldier, dist=4 time units with maxOrders=2

- **Volunteers** – each line describes a volunteer in the following pattern:

> volunteer <volunteer_name> <volunteer_role> <volunteer_coolDown>/<volunteer_maxDistance> <distance_per_step>(for drivers only) <volunteer_maxOrders>(optional)

For example:

> volunteer Ibrahim limited_collector 3 2


## Supported Actions
### 1. SimulateStep
perform a step is a core action in the simulator. Performing one step in the simulation is actually moving one unit of time forward.
- Syntax: `step <number_of_steps>`
- Example: `step 3`
- This action never results an error. Assume number_of_steps is positive number

### 2. AddOrder
This action creates an order. To create an order action, we only need the customer ID.
- Syntax: `order <customer_id>`
- Example: `order 4`
- This action should result an error if the provided customer ID doesn’t exist, or the customer reaches his maxOrders limit: ”Cannot place this order”.

### 3. AddCustomer
This action creates a new customer and store him in warehouse.
- Syntax: `customer <customer_name> <customer_type> <customer_distance> <max_orders>`
- Example: `customer Dani soldier 7 3`
- This action never results in an error.

### 4. PrintOrderStatus
This action prints an information on a given order, includes its status, the volunteers that associated with it and the customer who place it.
- Syntax: `orderStatus <order_id>`
- Example: `orderStatus 3` will prints:
```
OrderId: 3
OrderStatus: Pending/Collecting/Delivering/Completed 
CustomerID: <customer_id> 
Collector: <collector_id>/None 
Driver: <driver_id>/None
```

### 5. PrintCustomerStatus
This action prints all information about a particular customer, includes his ID, his orders status, and how many orders he still may place.
- Syntax: `customerStatus <customer_id>`
- Example: `customerStatus 6` will prints:
```
CustomerID: 6
OrderID: <order_id>
OrderStatus: Pending/Collecting/Delivering/Completed 
OrderID: <order_id> 
OrderStatus: Pending/Collecting/Delivering/Completed 
numOrdersLeft: <num_orders_left>
```
- This action should result an error if the customer_id doesn’t exist: **“Customer doesn’t exist”**.

### 6. PrintVolunteerStatus
This action prints all information about a particular volunteer, includes his ID, if he is busy or not, the order ID he currently processing, how much time is left until the processing of the order is completed, and how many orders he still may handle.
- Syntax: `volunteerStatus <volunteer_id>`
- Example: `volunteerStatus 1` will prints:
```
VolunteerID: 1
isBusy: True/False (Depends if the volunteer handle something or not)
OrderID: <order_id>/None (in case isBusy is False)
TimeLeft: <time_left/distance_left>/None (depends if the volunteer is a Collector/Driver/doesn’t process any order)
OrdersLeft: <ordersLeft>/No Limit (depends on the type of the volunteer)
```
- This action should result an error if the volunteer_id doesn’t exist: **“Volunteer doesn’t exist”**.

### 7. PrintActionsLog
Prints all the actions that were performed by the user (excluding current log action), from the first action to the last action
- Input Syntax: `log`
- Output Format:
```
<action_1_name> <action_1_args> <action_1_status>
<action_2_name> <action_2_args> <action_2_status>
 ….
<action_n_name> <action_n_args> <action_n_status>
```

### 8. Close
This action prints all orders with their status. Then, it closes the warehouse.
- Syntax: `close`
- Output Format:
```
OrderID: <order_1_id> , CustomerID<customer_1_id> , OrderStatus: <order_1_status>
OrderID: <order_2_id> , CustomerID:<customer_2_id> , OrderStatus: <order_2_status>
 ….
OrderID: <order_n_id> , CustomerID:<customer_n_id> , OrderStatus: <order_n_status>
```
- This action never results in an error.

### 9. BackupWarehouse
Save all warehouse information (customers, volunteers, orders, andactions history) in a backup. The program can keep only one backup: If it's called multiple times, the latest warehouse’s status will be stored and overwrite the previous one. 
- Syntax: `backup`
- This action never results in an error

### 10. RestoreWarehouse
Restore the backed-up warehouse status and overwrite the current warehouse status (warehouse itself, customers, volunteers, orders, and actions history).
- Syntax: `restore`
- If this action is called before backup action is called (which means "backup" is empty), then this action should result in an error: "No backup available"

## System Requirements
- CPP compiler
- makefile
