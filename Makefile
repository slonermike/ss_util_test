# Object files to either reference or create                          
OBJECTS = main.o ss_util/checksum.o ss_util/error_reporting.o ss_util/math/vector.o ss_util/math/sa_math.o

# The executable file that will be created at the end                 
EXEC = ss_util_test

# The flags to use for compilation                                    
FLAGS = -Wall                                                         

# The code compiler to use for compilation                            
CC = g++                                                              


# Perform action on all object files (May or may not exist)           
all: $(OBJECTS)
	$(CC) $(FLAGS) -o $(EXEC) $(OBJECTS)
	
clean:
	rm *.o
	rm ss_util/*.o
	rm ss_util/math/*.o
