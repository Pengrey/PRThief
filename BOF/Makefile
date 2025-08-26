NAME   := prthief

CC     := x86_64-w64-mingw32-gcc

CFLAGS := -c

CHS    :=  -Iinclude

SRC    := $(wildcard src/*.c)

# Logger
define log_info
	echo -e "[\033[0;33m*\033[0m] $(1)"
endef

define log_success
	echo -e "[\033[0;32m+\033[0m] Done"
endef

all: release

release:
	@ $(call log_info,[R] Compiling...)
	@ $(CC) $(SRC) $(CLIBS) $(CHS) $(CFLAGS) -o bin/$(NAME).o
	@ $(call log_success)

clean:
	@ $(call log_info,Cleaning build artifacts)
	@ rm -f bin/*.o
	@ $(call log_success)

.PHONY: release debug clean
