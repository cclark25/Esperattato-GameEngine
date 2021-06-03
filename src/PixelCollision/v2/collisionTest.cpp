
#include "../.././Types.h"
#include "../.././XM/XM.h"
#include "../../Animation/Animation.h"
#include "../../Camera/Camera.h"
#include "../../Image/Image.h"
#include "../../Keyboard/Keyboard.h"
#include "../../Node/Node.h"
#include "../../PixelCollision/v2/collisionSquare.h"
#include "../../Process/Process.h"
#include "../../Screen/Screen.h"
#include "../../ThreadWorker/ThreadWorker.h"
#include "cmath"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <chrono>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <unistd.h>

using namespace Esperatto;

int main(int argc, char **args) {
	al_init();

	al_init_image_addon();
	al_init_primitives_addon();
	al_install_audio();

	al_reserve_samples(1);

	// bool x = CollisionSquare(Coordinates({0,0}),Coordinates({10,10}), true)
	// 	.Intersects(CollisionSquare(Coordinates({5,5}),Coordinates({15,15}), true));
	// std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	// if(!x) throw "Test Failed!"; 

	// x = CollisionSquare(Coordinates({0,0}),Coordinates({10,10}), true)
	// 	.Intersects(CollisionSquare(Coordinates({10,5}),Coordinates({15,15}), true));
	// std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	// if(!x) throw "Test Failed!"; 
	
	// x = CollisionSquare(Coordinates({0,0}),Coordinates({10,10}), true)
	// 	.Intersects(CollisionSquare(Coordinates({11,5}),Coordinates({15,15}), true));
	// std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	// if(x) throw "Test Failed!"; 

	// x = CollisionSquare(Coordinates({0,0}),Coordinates({10,10}), true)
	// 	.Intersects(CollisionSquare(Coordinates({-10,5}),Coordinates({15,15}), true));
	// std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	// if(!x) throw "Test Failed!"; 

	// x = CollisionSquare(Coordinates({0,0}),Coordinates({10,10}), true)
	// 	.Intersects(CollisionSquare(Coordinates({-10,5}),Coordinates({0,15}), true));
	// std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	// if(!x) throw "Test Failed!"; 

	// x = CollisionSquare(Coordinates({0,0}),Coordinates({10,10}), true)
	// 	.Intersects(CollisionSquare(Coordinates({-10,5}),Coordinates({-1,15}), true));
	// std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	// if(x) throw "Test Failed!"; 


	// //

	// x = CollisionSquare(Coordinates({0,0}),Coordinates({10,10}), true)
	// 	.Intersects(CollisionSquare(Coordinates({5,10}),Coordinates({15,15}), true));
	// std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	// if(!x) throw "Test Failed!"; 
	
	// x = CollisionSquare(Coordinates({0,0}),Coordinates({10,10}), true)
	// 	.Intersects(CollisionSquare(Coordinates({5,11}),Coordinates({15,15}), true));
	// std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	// if(x) throw "Test Failed!"; 

	// x = CollisionSquare(Coordinates({0,0}),Coordinates({10,10}), true)
	// 	.Intersects(CollisionSquare(Coordinates({5,-10}),Coordinates({15,15}), true));
	// std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	// if(!x) throw "Test Failed!"; 

	// x = CollisionSquare(Coordinates({0,0}),Coordinates({10,10}), true)
	// 	.Intersects(CollisionSquare(Coordinates({5,-10}),Coordinates({15,0}), true));
	// std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	// if(!x) throw "Test Failed!"; 

	// x = CollisionSquare(Coordinates({0,0}),Coordinates({10,10}), true)
	// 	.Intersects(CollisionSquare(Coordinates({5,-10}),Coordinates({15,-1}), true));
	// std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	// if(x) throw "Test Failed!"; 

	// const auto x = CollisionSquare(Coordinates({0,0}), true);
	auto x = make_shared<CollisionSquare>(Coordinates({3,3}), true);
	Node root = Node(make_shared<CollisionSquare>(Coordinates({10,10}), true));
	Node secondNode = Node(x);
	root.setPositionInParent(10,10);

	root.addChild(secondNode);
	secondNode.setPositionInParent(3,3);

	x->Intersects(*x);

	al_uninstall_audio();

	return 0;
}
