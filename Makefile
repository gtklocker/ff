PROG = minidd
SRCS = minidd.c args.c

CCFLAGS += -Wall

default:
	@echo $(CC) $(SRCS) -o $(PROG)
	@${CC} ${CCFLAGS} $(SRCS) -o $(PROG)

clean:
	@echo rm -rf $(PROG)
	@rm -rf $(PROG)
