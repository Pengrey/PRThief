NAME   := prthief

CC     := clang -target x86_64-w64-mingw32

CFLAGS := -Os -nostdlib -fno-asynchronous-unwind-tables -std=c++20
CFLAGS += -fno-ident -fpack-struct=8 -falign-functions=1 -s -w -mno-sse
CFLAGS += -ffunction-sections -falign-jumps=1 -falign-labels=1
CFLAGS += -Wl,-s,--no-seh,--enable-stdcall-fixup -fno-exceptions -e start

CHS    :=  -Iinclude

CLIBS  :=  -lkernel32 -luser32 -lole32 -lnetapi32

SRC    := $(wildcard src/*.cc)

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
	@ $(CC) $(SRC) $(CLIBS) $(CHS) $(CFLAGS) -o bin/$(NAME).exe
	@ $(call log_success)

debug:
	@ $(call log_info,[D] Compiling...)
	@ $(CC) $(SRC) $(CLIBS) $(CHS) $(CFLAGS) -D DEBUG -o bin/$(NAME).exe
	@ $(call log_success)

clean:
	@ $(call log_info,Cleaning build artifacts)
	@ rm -f bin/*.exe
	@ $(call log_success)

.PHONY: release debug clean
