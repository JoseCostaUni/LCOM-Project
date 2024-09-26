# name of the program (Minix service)
PROG=lab5

# source code files to be compiled
SRCS = utils.c keyboard.c kbc.c graphic.c lab5.c  
# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

CPPFLAGS += -pedantic -DLAB3

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

