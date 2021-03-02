CXX = g++
CXXFLAG = -Wall -g

LINCLUDE = include
LLIB = lib
LOBJ = obj
LSRC = src
LAPP = bin
LDEP = dep

APPNAME = main

SRC = $(wildcard $(LSRC)/*.cpp)
SRC += $(wildcard $(LSRC)/*.c)
OBJ  = $(SRC:$(LSRC)/%=$(LOBJ)/%.o)
INCLUDE = $(LINCLUDE:%=-I%)
LIB = $(LLIB:%=-L%)
APP := $(LAPP)/$(APPNAME)

.PHONY: all, clean, run

ifeq ($(OS), Windows_NT)
# Window

LFLAG = -lglfw3 -lwinmm -lgdi32
DEP = $(OBJ:$(LOBJ)/%.o=$(LDEP)\\%.d)

$(LDEP)\\%.d: $(LSRC)/%
	@$(CXX) $(INCLUDE) $< -MM -MT $(@:$(LDEP)\\%.d=$(LOBJ)/%.o) >$@

RM = del
APP := $(LAPP)/$(APPNAME).exe
APP := $(subst /,\,$(APP))

all: $(APP)

clean:
	$(RM) $(subst /,\,$(OBJ))
	$(RM) $(APP)
	$(RM) $(subst /,\,$(DEP))
	
#end Window
else
# Linux

LFLAG = -lglfw -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -ldl -lpthread
DEP = $(OBJ:$(LOBJ)/%.o=$(LDEP)/%.d)

$(LDEP)/%.d: $(LSRC)/%
	@$(CXX) $(INCLUDE) $< -MM -MT $(@:$(LDEP)/%.d=$(LOBJ)/%.o) >$@

RM = rm
APP := ./$(APP)

all: $(APP)

clean:
	$(RM) $(OBJ)
	$(RM) $(APP)
	$(RM) $(DEP)

#end Linux
endif

$(APP):	$(OBJ)
	$(CXX) $(CXXFLAG) -o $@ $^ $(INCLUDE) $(LFLAG) $(LIB)

ifneq ($(MAKECMDGOALS), clean)
-include $(DEP)
endif

$(LOBJ)/%.o: $(LSRC)/%
	$(CXX) $(CXXFLAG) -o $@ -c $< $(INCLUDE)

run: $(APP)
	$(APP)

