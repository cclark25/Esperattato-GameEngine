CXX= g++
CXXFLAGS= -std=c++17 -Wall -pedantic -ggdb
# RM= rm -vf

OUT_DIR=./BUILD/object_files
# DEPENDENCIES=./Dependencies/AllegroCPPWrappers/BUILD/AllegroWrappers.o

./BUILD/Esperattato.o: $(OUT_DIR)/Animation.o $(OUT_DIR)/Screen.o $(OUT_DIR)/Node.o $(OUT_DIR)/NodeSubTypes.o $(OUT_DIR)/Camera.o $(OUT_DIR)/ThreadWorker.o $(OUT_DIR)/Process.o $(OUT_DIR)/Keyboard.o
	g++ $(CXXFLAGS) $(OUT_DIR)/*.o -shared -o ./BUILD/Esperattato.o -lallegro -lallegro_image -lallegro_primitives

# $(DEPENDENCIES):
# 	@$(MAKE) -C Dependencies/AllegroCPPWrappers 

test: ./BUILD/Esperattato.o
	$(CXX) $(CXXFLAGS) src/main.cpp BUILD/Esperattato.o -lallegro -lallegro_image -lallegro_primitives

$(OUT_DIR)/Animation.o: ./src/Animation/Animation.h ./src/Animation/Animation.cpp ./src/Image/Image.h
	$(CXX) $(CXXFLAGS)  -c ./src/Animation/Animation.cpp -fPIC -o "$(OUT_DIR)/Animation.o"

$(OUT_DIR)/Screen.o:  ./src/Screen/Screen.cpp ./src/Screen/Screen.h
	$(CXX) $(CXXFLAGS)  -c ./src/Screen/Screen.cpp -fPIC -o "$(OUT_DIR)/Screen.o"

$(OUT_DIR)/NodeSubTypes.o: ./src/Node/NodeSubTypes.h ./src/Node/NodeSubTypes.cpp
	$(CXX) $(CXXFLAGS) -c ./src/Node/NodeSubTypes.cpp -fPIC -o "$(OUT_DIR)/NodeSubTypes.o"

$(OUT_DIR)/Node.o: $(OUT_DIR)/NodeSubTypes.o ./src/Node/Node.h ./src/Node/Node.cpp
	$(CXX) $(CXXFLAGS) $(OUT_DIR)/NodeSubTypes.o -c ./src/Node/Node.cpp -fPIC -o "$(OUT_DIR)/Node.o"

$(OUT_DIR)/Camera.o: ./src/Camera/Camera.cpp ./src/Camera/Camera.h
	$(CXX) $(CXXFLAGS)  -c ./src/Camera/Camera.cpp -fPIC -o "$(OUT_DIR)/Camera.o"

$(OUT_DIR)/ThreadWorker.o: ./src/ThreadWorker/ThreadWorker.cpp ./src/ThreadWorker/ThreadWorker.h
	$(CXX) $(CXXFLAGS)  -c ./src/ThreadWorker/ThreadWorker.cpp -fPIC -o "$(OUT_DIR)/ThreadWorker.o" 

$(OUT_DIR)/Process.o: ./src/Process/Process.cpp ./src/Process/Process.h
	$(CXX) $(CXXFLAGS)  -c ./src/Process/Process.cpp -fPIC -o "$(OUT_DIR)/Process.o" 

$(OUT_DIR)/Keyboard.o: ./src/Keyboard/Keyboard.cpp ./src/Keyboard/Keyboard.h
	$(CXX) $(CXXFLAGS)  -c ./src/Keyboard/Keyboard.cpp -fPIC -o "$(OUT_DIR)/Keyboard.o"
