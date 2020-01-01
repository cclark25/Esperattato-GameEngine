CXX= g++
CXXFLAGS= -std=c++17
# RM= rm -vf

OUT_DIR=./BUILD/object_files
DEPENDENCIES=./Dependencies/AllegroCPPWrappers/BUILD/AllegroWrappers.o

./BUILD/Esperattato.o: $(DEPENDENCIES) $(OUT_DIR)/Display.o $(OUT_DIR)/Process.o $(OUT_DIR)/ThreadWorker.o $(OUT_DIR)/Node.o
	g++ $(OUT_DIR)/*.o $(DEPENDENCIES) -shared -o ./BUILD/Esperattato.o -lallegro -lallegro_image

$(DEPENDENCIES):
	@$(MAKE) -C Dependencies/AllegroCPPWrappers 

test: ./BUILD/Esperattato.o
	$(CXX) $(CXXFLAGS) src/main.cpp BUILD/Esperattato.o $(DEPENDENCIES) -lallegro -lallegro_image

$(OUT_DIR)/Node.o: ./src/Node/Node.cpp ./src/Node/Node.cpp
	$(CXX) $(CXXFLAGS)  -c ./src/Node/Node.cpp -fPIC -o "$(OUT_DIR)/Node.o"

$(OUT_DIR)/Display.o:  ./src/Display/Display.cpp ./src/Display/Display.h
	$(CXX) $(CXXFLAGS)  -c ./src/Display/Display.cpp -fPIC -o "$(OUT_DIR)/Display.o"

$(OUT_DIR)/Process.o: ./src/Process/Process.cpp ./src/Process/Process.h
	$(CXX) $(CXXFLAGS)  -c ./src/Process/Process.cpp -fPIC -o "$(OUT_DIR)/Process.o" 

$(OUT_DIR)/ThreadWorker.o: ./src/ThreadWorker/ThreadWorker.cpp ./src/ThreadWorker/ThreadWorker.h
	$(CXX) $(CXXFLAGS)  -c ./src/ThreadWorker/ThreadWorker.cpp -fPIC -o "$(OUT_DIR)/ThreadWorker.o" 

# $(OUT_DIR)/Transform.o: ./src/Transform/Transform.cpp ./src/Transform/Transform.h
# 	g++ -c ./src/Transform/Transform.cpp -fPIC -o "$(OUT_DIR)/Transform.o";

# $(OUT_DIR)/Display.o: ./src/Display/Display.cpp ./src/Display/Display.h
# 	g++ -c ./src/Display/Display.cpp -fPIC -o "$(OUT_DIR)/Display.o";
