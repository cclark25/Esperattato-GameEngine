CXX= g++
CXXFLAGS= -std=c++17
# RM= rm -vf

OUT_DIR=./BUILD/object_files
# DEPENDENCIES=./Dependencies/AllegroCPPWrappers/BUILD/AllegroWrappers.o

./BUILD/Esperattato.o: $(OUT_DIR)/NodeSubTypes.o $(OUT_DIR)/Screen.o $(OUT_DIR)/Process.o $(OUT_DIR)/ThreadWorker.o $(OUT_DIR)/Node.o
	g++ $(OUT_DIR)/*.o -shared -o ./BUILD/Esperattato.o -lallegro -lallegro_image

# $(DEPENDENCIES):
# 	@$(MAKE) -C Dependencies/AllegroCPPWrappers 

test: ./BUILD/Esperattato.o
	$(CXX) $(CXXFLAGS) src/main.cpp BUILD/Esperattato.o -lallegro -lallegro_image

$(OUT_DIR)/Node.o: $(OUT_DIR)/NodeSubTypes.o ./src/Node/Node.h ./src/Node/Node.cpp
	$(CXX) $(CXXFLAGS)  -c ./src/Node/Node.cpp $(OUT_DIR)/NodeSubTypes.o -fPIC -o "$(OUT_DIR)/Node.o"

$(OUT_DIR)/NodeSubTypes.o: ./src/Node/NodeSubTypes.h ./src/Node/NodeSubTypes.cpp
	$(CXX) $(CXXFLAGS)  -c ./src/Node/NodeSubTypes.cpp -fPIC -o "$(OUT_DIR)/NodeSubTypes.o"

$(OUT_DIR)/Screen.o:  ./src/Screen/Screen.cpp ./src/Screen/Screen.h
	$(CXX) $(CXXFLAGS)  -c ./src/Screen/Screen.cpp -fPIC -o "$(OUT_DIR)/Screen.o"

$(OUT_DIR)/Process.o: ./src/Process/Process.cpp ./src/Process/Process.h
	$(CXX) $(CXXFLAGS)  -c ./src/Process/Process.cpp -fPIC -o "$(OUT_DIR)/Process.o" 

$(OUT_DIR)/ThreadWorker.o: ./src/ThreadWorker/ThreadWorker.cpp ./src/ThreadWorker/ThreadWorker.h
	$(CXX) $(CXXFLAGS)  -c ./src/ThreadWorker/ThreadWorker.cpp -fPIC -o "$(OUT_DIR)/ThreadWorker.o" 

# $(OUT_DIR)/Transform.o: ./src/Transform/Transform.cpp ./src/Transform/Transform.h
# 	g++ -c ./src/Transform/Transform.cpp -fPIC -o "$(OUT_DIR)/Transform.o";

# $(OUT_DIR)/Display.o: ./src/Display/Display.cpp ./src/Display/Display.h
# 	g++ -c ./src/Display/Display.cpp -fPIC -o "$(OUT_DIR)/Display.o";
