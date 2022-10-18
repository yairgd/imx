bindir=.
BIN = asm 

YSRCS +=calc.yacc
LSRCS +=calc.lex
CSRCS +=instructions.c
OBJEXT ?= .o



COBJS = $(CSRCS:.c=$(OBJEXT))
MAINOBJ = $(MAINSRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS)


all: default

y.tab.c: $(YSRCS)
	yacc -d $<

lex.yy.c: $(LSRCS)
	lex $<


%.o: %.c
	@$(CC)  $(CFLAGS) -c $< -o $@
	@echo "CC $<"
    
default: $(AOBJS) $(COBJS) $(MAINOBJ) $(YSRCS) y.tab.c 
	$(CC) -o $(BIN) $(MAINOBJ) $(COBJS) $(LDFLAGS) y.tab.c lex.yy.c

clean: 
	rm -f $(BIN) $(AOBJS) $(COBJS) $(MAINOBJ)
	$(rm) $@.tab.c $@.tab.h $@.lex.c

