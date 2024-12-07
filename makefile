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
# executable and object files 
EXE=sim
OBJECTS=sim.o scheduling.o circular_queue.o display.o compare.o

# C compiler stuff
CC=gcc		# C compiler program
CFLAGS=-g	# flags for the C compiler 


#--------- Building, Linking, Cleaning ---------------------
# default target for conditional recompilation
default: $(EXE)

# target for full rebuild 
all: clean $(EXE)

# Linking target executable from object files
$(EXE): $(OBJECTS)
	$(CC) $(CLFAGS) $(OBJECTS) -o $(EXE)

# Cleaning up the old object files and executable
clean:
	rm -f *.o $(EXE) circ 

#--------- Creating Object Files --------------------------- 
sim.o: sim.c sim.h globals.h process.h options.h
	$(CC) $(CFLAGS) -c sim.c 

scheduling.o: scheduling.c scheduling.h globals.h process.h circular_queue.h options.h compare.h display.h
	$(CC) $(CFLAGS) -c scheduling.c 

circular_queue.o: circular_queue.h globals.h process.h options.h
	$(CC) $(CFLAGS) -c circular_queue.c

display.o: globals.h process.h options.h compare.h
	$(CC) $(CFLAGS) -c display.c

compare.o: globals.h process.h options.h
	$(CC) $(CFLAGS) -c compare.c 

# The empty line above this comment must remain to avoid errors