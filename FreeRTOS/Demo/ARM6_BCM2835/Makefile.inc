# =============================================================================
# $Id: Makefile.inc 2280 2017-11-09 14:43:06Z BiseL $
# $Author: BiseL $
# $Revision: 2280 $
# $URL: https://svn/svn/ITS/SW/Technical/MakefileSyd/Makefile.inc $
# Description:
# -----------------------------------------------------------------------------
# Makefile structure developed by Léonard Bise.
# Contains all the management of the makefile including the principal targets.
# The following targets are defined:
# - default_target: The default target (equivalent to all)
# - $(OUTDIR): Creates the output directory if required
# - $(OUTFILE): Links the output file (can be either an executable or a library)
# - $(OUTDIR)/%.o: Compiles .c or .S files
# - debug: Builds output with debug information
# - clean: Empties the output directories
# - list: List all the internal makefile variables
# =============================================================================
# ========================================================
# OUTPUT DIRECTORY
# Build Debug: OP=DEBUG
# Build Release: OP= or OP=RELEASE
# ========================================================
# Output debug directory
OUTDIRDEBUG=out-debug
# Output release directory
OUTDIRRELEASE=out
# If no operation, do release
ifeq ($(OP),)
OP=RELEASE
endif
# Output directory
OUTDIR=$(OUTDIR$(OP))
# Name and path of the output file
OUTFILE:=$(OUTDIR)/$(OUTNAME)
# Folder for metrics
METRICSDIR=$(OUTDIR)/metrics

# ========================================================
# FILES
# ========================================================
# Find all types of source files
# .c Files
CFILES:=$(foreach dir,$(SRCPATHS),$(wildcard $(dir)/*.c))
# Add additional files
CFILES+=$(ADDSRC)
# Preprocessed c files
IFILES:=$(patsubst %.c,$(OUTDIR)/%.i,$(notdir $(CFILES)))
METRICSFILES:=$(patsubst %.c,$(METRICSDIR)/%.html,$(notdir $(CFILES)))
COFILES:=$(patsubst %.c,$(OUTDIR)/%.o,$(notdir $(CFILES)))
# .h files
HFILES:=$(foreach dir,$(SRCPATHS),$(wildcard $(dir)/*.h))
# .S files
SFILES:=$(foreach dir,$(SRCPATHS),$(wildcard $(dir)/*.S))
SOFILES := $(patsubst %.S,$(OUTDIR)/%.o,$(notdir $(SFILES)))
# All sources
SRCFILES:=$(CFILES) $(HFILES) $(SFILES)
# All user objects
OBJFILES:=$(COFILES) $(SOFILES)
LINK_OBJS+=$(OBJFILES)
# Add all paths to VPATH
VPATH=$(SRCPATHS) $(dir $(ADDSRC))
# Add all source paths to INC	
INC+=$(patsubst %,-I%,$(SRCPATHS))

# ========================================================
# COMPILATION & LINKING
# ========================================================
CC=$(TOOLCHAINBIN)$(TOOLCHAINPREFIX)gcc
# Compilation flags
CFLAGS+= -Wall
ifeq ($(OP),DEBUG)
CFLAGS+= -g -D__DEBUG__
endif
CXX=$(TOOLCHAINBIN)$(TOOLCHAINPREFIX)g++
CPP=$(TOOLCHAINBIN)$(TOOLCHAINPREFIX)gcc
CPPFLAGS+=-E -w
AS=$(TOOLCHAINBIN)$(TOOLCHAINPREFIX)as
AR=$(TOOLCHAINBIN)$(TOOLCHAINPREFIX)ar
# Create new archive, insert the files
ARFLAGS=rc
NM=$(TOOLCHAINBIN)$(TOOLCHAINPREFIX)nm
NMFLAGS+=-A -a -n -s
LD=$(TOOLCHAINBIN)$(TOOLCHAINPREFIX)ld
SIZE=$(TOOLCHAINBIN)$(TOOLCHAINPREFIX)size
SIZEFLAGS+=
STRIP=$(TOOLCHAINBIN)$(TOOLCHAINPREFIX)strip
STRIPFLAGS+=
FRAMAC=frama-c
FRAMACFLAGS+=-metrics -metrics-by-function
OBJCOPY=$(TOOLCHAINBIN)$(TOOLCHAINPREFIX)objcopy
OBJDUMP=$(TOOLCHAINBIN)$(TOOLCHAINPREFIX)objdump
# Print debugging info, disassemble, section header, line numbers,
# relocation info, full content, source code, symbol table, all
# headers, wide format
OBJDUMPFLAGS+=-gdhlrsStxw
# Linking flags
LDFLAGS+=
# Libraries
LINK_LIBS+=$(LD_LIBS) $(LD_PATHS)
# Linker script
LINKERSCRIPT:=$(addprefix -T, $(LINKERSCRIPT))
# Linking
ifeq ($(suffix $(OUTFILE)),.a)
LINK=$(AR) $(ARFLAGS) $@ $(LINK_OBJS)
else
LINK=$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(LINK_OBJS) $(LINK_LIBS)	$(LINKERSCRIPT)
endif

# ========================================================
# APPLICATIONS
# ========================================================
ECHO=@echo
RM=rm
CP=cp
MAKE=make
MAKEFILE=Makefile
MKDIR=mkdir

# ========================================================
# TARGETS
# ========================================================
# The first target is always the default goal
default_target: all

# Create output folder
$(OUTDIR):
	test -d $(OUTDIR) || $(MKDIR) $(OUTDIR)

# Create output file
$(OUTFILE): $(OBJFILES)
	$(LINK)

# Build c file
$(OUTDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(INC) -o $@ $<

# Build S file
$(OUTDIR)/%.o: %.S
	$(AS) -o $@ $<

# Preprocess C file
$(OUTDIR)/%.i: %.c
	$(CPP) $(CPPFLAGS) $(INC) -o $@ $<

# Target debug calls the Makefile again with operation debug
debug:
	$(MAKE) -f $(MAKEFILE) OP=DEBUG

# Clean output directory
clean:
	$(RM) -rf $(OUTDIRDEBUG)/*
	$(RM) -rf $(OUTDIRRELEASE)/*

# List the makefile variables
list:
	$(ECHO) "List variables"
	$(ECHO) "--------------------"
	$(ECHO) "MAKEFILE $(MAKEFILE)"
	$(ECHO) "OP $(OP)"
	$(ECHO) "OUTNAME $(OUTNAME)"
	$(ECHO) "OUTDIR $(OUTDIR)"
	$(ECHO) "OUTFILE $(OUTFILE)"
	$(ECHO) "SRCPATHS $(SRCPATHS)"
	$(ECHO) "INC $(INC)"
	$(ECHO) "SRCFILES $(SRCFILES)"
	$(ECHO) "CFILES $(CFILES)"
	$(ECHO) "IFILES $(IFILES)"
	$(ECHO) "METRICSFILES $(METRICSFILES)"		
	$(ECHO) "COFILES $(COFILES)"
	$(ECHO) "HFILES $(HFILES)"
	$(ECHO) "SFILES $(SFILES)"
	$(ECHO) "SOFILES $(SOFILES)"
	$(ECHO) "OBJFILES $(OBJFILES)"
	$(ECHO) "LINK_OBJS $(LINK_OBJS)"
	
	$(ECHO) "VPATH $(VPATH)"
	$(ECHO) "TOOLCHAINROOT $(TOOLCHAINROOT)"
	$(ECHO) "MAKEFILE $(MAKEFILE)"
	$(ECHO) "CFLAGS $(CFLAGS)"
	$(ECHO) "LDFLAGS $(LDFLAGS)"
	$(ECHO) "--------------------"

objdump: $(OUTFILE)
	$(OBJDUMP) $(OBJDUMPFLAGS) $(OUTFILE) > $(basename $(OUTFILE)).objdump

srec: $(OUTFILE)
	$(OBJCOPY) -O srec $(SRECFLAGS) $(OUTFILE) $(basename $(OUTFILE)).srec

bin: $(OUTFILE)
	$(OBJCOPY) -O binary $(BINFLAGS) $(OUTFILE) $(basename $(OUTFILE)).bin

map: $(OUTFILE)
	$(NM) $(NMFLAGS) $(OUTFILE) > $(basename $(OUTFILE)).map

size: $(OUTFILE)
	$(SIZE) $(SIZEFLAGS) $(OUTFILE)

strip: $(OUTFILE)
	$(STRIP) $(STRIPFLAGS) $(OUTFILE)

$(METRICSDIR):
	test -d $(METRICSDIR) || $(MKDIR) $(METRICSDIR)

# Build metrics using framac
$(METRICSDIR)/%.html: $(IFILES)
	$(FRAMAC) $(FRAMACFLAGS) $< -metrics-output $@

framac-metrics: $(METRICSDIR) $(METRICSFILES)
