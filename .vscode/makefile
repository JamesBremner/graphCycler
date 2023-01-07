#source file search paths
VPATH = ./src ../raven-set

	# compiler include search paths
INCS = -I./include \
	-I../ravencode/pub/src \
	-I../raven-set \

LIBS = \
	-lgdiplus -lgdi32 -lcomdlg32 -lstdc++fs \
	#-mwindows

static_link_flags = -static-libstdc++ -static-libgcc -static

# folders
ODIR = ./.vscode/obj

OPT = -g
CCflags = -std=c++17 $(OPT) -D_mingw_

# sources
OBJ_starter = \
	$(ODIR)/main.o \
	$(ODIR)/cGraph.o

OBJ_test = \
	$(ODIR)/test.o \
	$(ODIR)/cGraph.o \
	$(ODIR)/cutest.o

### COMPILE

$(ODIR)/%.o: %.cpp cGraph.h
	g++ -c $<  \
	-o $@ $(INCS) $(CCflags)

starter: $(OBJ_starter)
	g++ $(OPT) \
	-o bin/starter.exe $^  \
	$(static_link_flags) $(LIBS)

test: $(OBJ_test)
	g++ $(OPT) \
	-o bin/test.exe $^  \
	$(static_link_flags) $(LIBS)

.PHONY: clean
clean:
	del /q .vscode\\obj\\*.o

build: starter test

rebuild: clean starter test

