CXX= g++
CXXFLAGS= -std=c++17 -Wall -pedantic -ggdb
CFLAGS=  -Wall -pedantic -ggdb -fPIC

libxmFLAGS= -I./Dependencies/libxm/src -I./Dependencies/libxm/include -I/usr/include
# RM= rm -vf

OUT_DIR=./BUILD/object_files
# DEPENDENCIES=./Dependencies/AllegroCPPWrappers/BUILD/AllegroWrappers.o

./BUILD/Esperattato.o: $(OUT_DIR)/XM.o $(OUT_DIR)/Animation.o $(OUT_DIR)/Screen.o $(OUT_DIR)/Node.o $(OUT_DIR)/NodeSubTypes.o $(OUT_DIR)/Camera.o $(OUT_DIR)/ThreadWorker.o $(OUT_DIR)/Process.o $(OUT_DIR)/Keyboard.o $(OUT_DIR)/PixelCollision.o $(OUT_DIR)/CollisionTree.o $(OUT_DIR)/collisionSquare.o
	g++ $(CXXFLAGS) $(libxmFLAGS) $(OUT_DIR)/libxm/*.o $(OUT_DIR)/*.o -shared -o ./BUILD/Esperattato.o -lallegro -lallegro_image -lallegro_primitives -lallegro_audio -fPIC

# $(DEPENDENCIES):
# 	@$(MAKE) -C Dependencies/AllegroCPPWrappers 

test: ./BUILD/Esperattato.o
	$(CXX) $(CXXFLAGS) $(libxmFLAGS) src/main.cpp BUILD/Esperattato.o -lallegro -lallegro_image -lallegro_primitives -lallegro_audio -lpthread

testCollision: ./BUILD/Esperattato.o
	$(CXX) $(CXXFLAGS) $(libxmFLAGS) src/PixelCollision/v2/collisionTest.cpp BUILD/Esperattato.o -lallegro -lallegro_image -lallegro_primitives -lallegro_audio -lpthread


$(OUT_DIR)/Animation.o: ./src/Animation/Animation.h ./src/Animation/Animation.cpp ./src/Image/Image.h
	$(CXX) $(CXXFLAGS) -c ./src/Animation/Animation.cpp -fPIC -o "$(OUT_DIR)/Animation.o"

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

$(OUT_DIR)/XM.o: ./src/XM/XM.cpp $(OUT_DIR)/libxm
	$(CXX) $(CXXFLAGS) $(libxmFLAGS) -c ./src/XM/XM.cpp $(OUT_DIR)/libxm -fPIC -o "$(OUT_DIR)/XM.o" 

$(OUT_DIR)/PixelCollision.o: ./src/PixelCollision/PixelCollision.cpp ./src/PixelCollision/PixelCollision.h ./src/Image/Image.h ./src/PixelCollision/CollisionTree.h
	$(CXX) $(CXXFLAGS)  -c ./src/PixelCollision/PixelCollision.cpp -fPIC -o "$(OUT_DIR)/PixelCollision.o" 

$(OUT_DIR)/CollisionTree.o: ./src/PixelCollision/CollisionTree.cpp ./src/PixelCollision/CollisionTree.h 
	$(CXX) $(CXXFLAGS)  -c ./src/PixelCollision/CollisionTree.cpp -fPIC -o "$(OUT_DIR)/CollisionTree.o"

$(OUT_DIR)/collisionSquare.o: ./src/PixelCollision/v2/collisionSquare.cpp ./src/PixelCollision/v2/collisionSquare.h 
	$(CXX) $(CXXFLAGS)  -c ./src/PixelCollision/v2/collisionSquare.cpp -fPIC -o "$(OUT_DIR)/collisionSquare.o"


$(OUT_DIR)/libxm: ./Dependencies/libxm/src/* 
	[ -e $(OUT_DIR)/libxm ] || mkdir $(OUT_DIR)/libxm

	cd $(OUT_DIR)/libxm && \
	gcc $(CFLAGS) -c ../../../Dependencies/libxm/src/*.c \
		-I../../../Dependencies/libxm/include -I../../../Dependencies/libxm/src -I/usr/include \
		-D XM_DEFENSIVE=true -D XM_LIBXMIZE_DELTA_SAMPLES=true -D XM_LINEAR_INTERPOLATION=true -D XM_DEBUG


