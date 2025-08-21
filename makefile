# =========================
# Project: absolute_chaos
# z88dk + DeZog + (optional) emulators
# =========================

# ---- Names & layout ----
NAME       := absolute_chaos
SRC_DIR    := src
BUILD_DIR  := /Users/stu/src/absolute_chaos/build
SRCS       := $(wildcard $(SRC_DIR)/*.c)
HEADERS    := $(wildcard $(SRC_DIR)/*.h)
OUTBASE    := $(BUILD_DIR)/$(NAME)

# ---- Tools ----
ZCC        ?= zcc
MONO       ?= mono

# Default CSpect location (can be overridden on make cmdline)
CSPECT_EXE ?= /Users/stu/src/NextBuild/Emu/CSpect/CSpect.exe
ZESARUX    ?= /Applications/ZEsarUX.app/Contents/MacOS/zesarux

# ---- Common flags, optimized for debugging ----
CFLAGS_COMMON := -vn -O0 -clib=sdcc_iy -Cs"--max-allocs-per-node 200000"
DEBUG_FLAGS   := -m --list --c-code-in-asm

# ---- ZX Next (.nex) ----
TARGET_ZXN    := +zxn
LDFLAGS_ZXN   := -subtype=nex -create-app
NEX           := $(OUTBASE).nex

# ---- ZX 128K (.sna) ----
TARGET_ZX128  := +zx
LDFLAGS_ZX128 := -subtype=sna -create-app
SNA           := $(OUTBASE).sna

# =========================
# Phony targets
# =========================
.PHONY: all zxn zx128 run-zxn run-zx128 clean

# Default: build both
all: zxn zx128

# ---------- Build: ZX Next ----------
zxn: $(NEX)

$(NEX): $(SRCS) $(HEADERS) | $(BUILD_DIR)
	$(ZCC) $(TARGET_ZXN) $(CFLAGS_COMMON) $(DEBUG_FLAGS) \
	      -o $(OUTBASE) $(LDFLAGS_ZXN) $(SRCS)

# ---------- Build: ZX 128K ----------
zx128: $(SNA)

$(SNA): $(SRCS) $(HEADERS) | $(BUILD_DIR)
	$(ZCC) $(TARGET_ZX128) $(CFLAGS_COMMON) $(DEBUG_FLAGS) \
	      -o $(OUTBASE) $(LDFLAGS_ZX128) $(SRCS)

# ---------- Optional runners ----------
run-zxn: $(NEX)
	$(MONO) "$(CSPECT_EXE)" -zxnext -w2 -tv -nex "$(NEX)"

run-zx128: $(SNA)
	$(ZESARUX) --machine 128k --quickexit --snap "$(SNA)"

# ---------- Utils ----------
$(BUILD_DIR):
	mkdir -p "$(BUILD_DIR)"

clean:
	rm -rf "$(BUILD_DIR)"
