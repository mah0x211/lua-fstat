TARGET=fstat.$(LIB_EXTENSION)
SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:.c=.o)
INSTALL?=install

ifdef FSTAT_COVERAGE
COVFLAGS=--coverage
endif

.PHONY: all install clean

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(WARNINGS) $(COVFLAGS) $(CPPFLAGS) -o $@ -c $<

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS) $(PLATFORM_LDFLAGS) $(COVFLAGS)

install:
	$(INSTALL) $(TARGET) $(LIBDIR)
	rm -f ./src/*.o ./*.so ./src/*.gcda

clean:
	rm -f ./src/*.o ./*.so ./src/*.gcda ./src/*.gcno
