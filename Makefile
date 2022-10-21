bindir=.
BIN = asm 

YSRCS +=calc.yacc
LSRCS +=calc.lex
CSRCS +=instructions.c
OBJEXT ?= .o



COBJS = $(CSRCS:.c=$(OBJEXT))
MAINOBJ = $(MAINSRC:.c=$(OBJEXT))


all: default

y.tab.c: $(YSRCS)
	yacc -d $<

lex.yy.c: $(LSRCS)
	lex $<


%.o: %.c
	@$(CC)  $(CFLAGS) -c $< -o $@ -O0 -g
	@echo "CC $<"
    
default: $(AOBJS) $(COBJS) $(MAINOBJ) $(YSRCS) y.tab.c  lex.yy.c
	$(CC) -o $(BIN) $(MAINOBJ) $(COBJS) $(LDFLAGS) y.tab.c lex.yy.c -O0 -g

clean: 
	rm -f $(BIN) $(AOBJS) $(COBJS) $(MAINOBJ) y.tab.c lex.yy.c y.tab.h

