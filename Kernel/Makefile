include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm
LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

# Exclude files for specific targets
ifeq ($(MAKECMDGOALS), buddy)
    SOURCES := $(filter-out MyMemoryManager.c, $(SOURCES)) buddy.c
else ifeq ($(MAKECMDGOALS), all)
    SOURCES := $(filter-out buddy.c, $(SOURCES))
endif

OBJECTS=$(SOURCES:.c=.o)

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

buddy: $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(filter-out buddy.o, $(OBJECTS)) $(OBJECTS_ASM) buddy.o
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(filter-out buddy.o, $(OBJECTS)) $(OBJECTS_ASM) buddy.o

clean:
	rm -rf asm/*.o *.o *.bin

.PHONY: all clean buddy
