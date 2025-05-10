PROJECT = $(shell basename $(CURDIR))
SRC_DIR = src

SRC_APP = src/simulator.cpp

OBJ_APP = $(SRC_APP:$(SRC_DIR)/%.cpp=$(G_OBJ)/$(PROJECT)/%.o)

DEP_APP = $(OBJ_APP:%.o=%.d)

INCLUDE_DIRS = -I$(G_SRC)/libcommon/include -I$(G_SRC)/libconfig/include $(INCLUDE_OPENCV)
LIBRARY_DIRS = -L$(G_BIN)
CXXFLAGS = -fPIC -MMD $(INCLUDE_DIRS)
LDFLAGS = -ldl -lpthread -latomic $(LIBRARY_DIRS) -lcommon -lconfig -lopencv_core

.PHONY: all 
all: $(G_BIN)/segbot_sim
	@echo "\033[0;97m [DONE] $(PROJECT)"
	
$(G_BIN)/segbot_sim: $(OBJ_APP)
	@echo "\033[0;32m [LINK] $@ \033[0;0m"
	@g++ -o $@ $^ $(LDFLAGS)  
    
-include $(DEP_APP)

$(G_OBJ)/$(PROJECT)/%.o: $(SRC_DIR)/%.cpp
	@echo "\033[0;93m [CC] $@ \033[0;0m"
	@g++ -c $< -o $@ $(CXXFLAGS) $(G_CXXFLAGS) -std=c++11

$(shell mkdir -p $(G_OBJ)/$(PROJECT))
$(shell mkdir -p $(G_BIN))
