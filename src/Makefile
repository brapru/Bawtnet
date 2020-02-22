
STD=-std=c11 -pedantic 
CFLAGS=-Wall -W 

SERVER_NAME=command-server
SERVER_OBJ= server.o
CLI_NAME=control-shell

all: $(SERVER_NAME)
	@echo ""
	@echo "Successfully compiled $(SERVER_NAME)"
	@echo ""

clean:
	rm -rf $(SERVER_NAME) *.o

.PHONY: clean

# botnet-server
$(SERVER_NAME): $(SERVER_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
