# varilables
CC = g++
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging version
# -O2 option is for optimized version
CFLAGS = -c
AR = ar
ARFLAGS = rcv
DBGFLAGS = -g -D_DEBUG_ON_
OPTFLAGS = -O2

all	: ./DPT_balance_color
	@echo "Compile optimal version success!\n"

./DPT_balance_color : graph_opt.o main_opt.o
	$(CC) $(OPTFLAGS) graph_opt.o main_opt.o -o $@
main_opt.o : ./main.cpp
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
graph_opt.o : ./graph.cpp ./graph.h
	$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

# DEBUG Version
dbg : ./DPT_balance_color_dbg
	@echo "Compile debug version success!\n"
./DPT_balance_color_dbg : graph_dbg.o main_dbg.o
	$(CC) $(DBGFLAGS) graph_dbg.o main_dbg.o -o $@
main_dbg.o : ./main.cpp
	$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@
graph_dbg.o : ./graph.cpp ./graph.h
	$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

# clean all the .o and executable files
clean:
	rm -f *.o DPT_balance_color*
