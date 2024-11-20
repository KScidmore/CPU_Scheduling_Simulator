#----------- ID HEADER -------------------------------------
#  Author(s):	Andrew Boisvert, Kyle Scidmore
#  Email(s):    abois526@mtroyal.ca, kscid125@mtroyal.cas
#  File Name:   makefile
#
#  Program Purpose(s):
#    To help determine what parts of the program need to be
#    recompiled, and to aid in the process of doing so.
#-----------------------------------------------------------

#--------- Variable declarations ---------------------------
# Targets and objects
TARGET=sim
OBJECTS=sim.o scheduling.o

# CC stuff
CC=gcc
CFLAGS=-g


#--------- Building, Linking, Cleaning ---------------------
# Building the target
all: clean $(TARGET)

# Linking target executable from object files
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Cleaning up the old object files and executable
clean:
	rm -f *.o $(TARGET)


#--------- Creating Object Files --------------------------- 
sim.o: sim.c sim.h globals.h process.h
	$(CC) $(CFLAGS) -c sim.c 

scheduling.o: scheduling.c scheduling.h globals.h process.h
	$(CC) $(CFLAGS) -c scheduling.c 

# The empty line above this comment must remain to avoid errors