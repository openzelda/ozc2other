
BUILDPLATFORM = ${shell uname}



# Windows
CPP      = g++.exe
CC       = gcc.exe
WINDRES  = windres.exe
LIBS     = -static-libgcc -Wl,--enable-auto-import -lcomdlg32 -lrpcrt4  -s  -m32
INCS     =
CXXINCS  =
BIN      = ozc2other.exe
CXXFLAGS = -D__GNUWIN32__ -DWIN32 -D_WINDOWS $(CXXINCS) -m32 -std=c++11 -Wno-write-strings
CFLAGS   = -D__GNUWIN32__ -DWIN32 -D_WINDOWS $(INCS) -m32 -std=c99
RM       = rm -f


ifeq ($(BUILDPLATFORM), Darwin)

endif

ifeq ($(BUILDPLATFORM), Linux)

endif



OBJ      = main.o qst.o game.o elix_system.o stb_image.o rlebmp.o
LINKOBJ = $(OBJ) $(RES)


.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after


clean: clean-custom
	$(RM) $(OBJ) $(BIN)


$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(LIBS)

%.o : %.cpp
	@echo Compiling $<
	@$(CPP) $(CXXFLAGS) -o $@ -c $<

%.o : %.c
	@echo Compiling $<
	@$(CC) -c $(CFLAGS) -o $@ $<

