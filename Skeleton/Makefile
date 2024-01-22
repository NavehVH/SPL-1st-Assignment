# define some Makefile variables for the compiler and compiler flags

# to use Makefile variables later in the Makefile: $()

CC = g++

CFLAGS = -g -Wall -Weffc++ -std=c++11

LFLAGS = -L/usr/lib


# All Targets

all: warehouse


# Tool invocations

# Executable "warehouse" depends on the files warehouse.o.

warehouse: clean bin/Action.o bin/Customer.o bin/main.o bin/Order.o bin/Volunteer.o bin/Warehouse.o

    @echo 'Building target: warehouse'

    @echo 'Invoking: C++ Linker'

	$(CC) -o bin/warehouse bin/Action.o bin/Customer.o bin/main.o bin/Order.o bin/Volunteer.o bin/Warehouse.o $(LFLAGS)

    @echo 'Finished building target: warehouse'

	@echo '	'


# Depends on the source and header files

bin/Action.o: src/Action.cpp

	$(CC) $(CFLAGS) -c -Iinclude -o bin/Action.o src/Action.cpp


bin/Customer.o: src/Customer.cpp

	$(CC) $(CFLAGS) -c -Iinclude -o bin/Customer.o src/Customer.cpp

bin/main.o: src/main.cpp

	$(CC) $(CFLAGS) -c -Iinclude -o bin/main.o src/main.cpp

bin/Order.o: src/Order.cpp

	$(CC) $(CFLAGS) -c -Iinclude -o bin/Order.o src/Order.cpp

bin/Volunteer.o: src/Volunteer.cpp

	$(CC) $(CFLAGS) -c -Iinclude -o bin/Volunteer.o src/Volunteer.cpp

bin/Warehouse.o: src/Warehouse.cpp

	$(CC) $(CFLAGS) -c -Iinclude -o bin/Warehouse.o src/Warehouse.cpp

#Clean the build directory

clean:

	rm -f bin/*