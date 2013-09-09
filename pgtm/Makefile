# ==============================================================================
#
# Makefile.real
#
# ==============================================================================


# ==============================================================================
# Variables
# ==============================================================================

CC      := g++
CFLAGS  := -g -Wall -Winline -O3
#CFLAGS  += -m32
#CFLAGS  += -DPGTM_OPTIM_HASHLOG
#CFLAGS  += -DPGTM_RESIZE_HASHLOG
LD      := g++

LIBPGTM := libpgtm.a

SRCS := \
	pgtm.cpp \
#
OBJS := ${SRCS:.cpp=.o}

AR      := ar
RANLIB  := ranlib

RM := rm -f


# ==============================================================================
# Rules
# ==============================================================================

.PHONY: default
default: lazy

.PHONY: clean
clean:
	$(RM) $(LIBPGTM) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBPGTM): $(OBJS)
	$(AR) cru $@ $^
	$(RANLIB) $@

.PHONY: lazy
lazy: $(LIBPGTM)

.PHONY: eager
eager: CFLAGS += -DPGTM_EAGER
eager: $(LIBPGTM)

.PHONY: lazy-nocm
lazy-nocm: CFLAGS += -DPGTM_NOCM
lazy-nocm: $(LIBPGTM)

.PHONY: eager-nocm
eager-nocm: CFLAGS += -DPGTM_EAGER
eager-nocm: CFLAGS += -DPGTM_NOCM
eager-nocm: $(LIBPGTM)

.PHONY: otm
otm: CFLAGS += -m32
otm: $(LIBPGTM)


# ==============================================================================
# Dependencies
# ==============================================================================

%.o: %.h


# ==============================================================================
#
# End of Makefile.real
#
# ==============================================================================
