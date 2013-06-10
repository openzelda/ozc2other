
BUILDPLATFORM = ${shell uname}


# Windows
CPP      = g++.exe
CC       = gcc.exe
RM       = rm -f
WINDRES  = windres.exe

BIN      = ozc2other.exe

LIBS     = -static-libgcc -Wl,--enable-auto-import -lcomdlg32 -lrpcrt4 -g3  -m32
INCS     =
CXXINCS  =
CXXFLAGS = -D__GNUWIN32__ -DWIN32 -D_WINDOWS $(CXXINCS) -m32 -std=c++11 -Wno-write-strings -g3
CFLAGS   = -D__GNUWIN32__ -DWIN32 -D_WINDOWS $(INCS) -m32 -std=c99 -g3

ifeq ($(BUILDPLATFORM), Darwin)

endif

ifeq ($(BUILDPLATFORM), Linux)
	LIBS     = -static-libgcc -luuid
	INCS     =
	CXXINCS  =
	CXXFLAGS = -std=c++11 -Wno-write-strings
	CFLAGS   =  $(INCS) -std=c99

	CPP = g++
	CC  = gcc
endif



OBJ      = main.o qst.o game.o elix_system.o stb_image.o rlebmp.o
LINKOBJ = $(OBJ) $(RES)


.PHONY: all all-before all-after clean clean-custom

all: $(BIN)
	@echo -----------------------


clean: clean-custom
	$(RM) $(OBJ) $(BIN)


$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(LIBS)

%.o : %.cpp
	@echo Compiling $<
	@$(CPP) $(CXXFLAGS) -o $@ -c $<

%.o : %.c
	@echo Compiling $<
	@$(CC) -c $(CFLAGS) -o $@ -c $<


