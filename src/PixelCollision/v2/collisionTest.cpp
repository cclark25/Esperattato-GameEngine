
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
	Node root = Node(make_shared<CollisionSquare>(Coordinates({10,10}), true));
	Node second = Node(make_shared<CollisionSquare>(Coordinates({15,15}), true));
	auto rootSquare = *((CollisionSquare*) root.getDataPtr().get());
	auto secondSquare = *((CollisionSquare*) second.getDataPtr().get());

	/*
		TODO: rewrite all of these tests from scratch
	*/
	// Coordinates({5,5})
	second.setPositionInParent(5,5);
	bool x = rootSquare
		.Intersects(secondSquare);
	std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	if(!x) throw std::logic_error("Test Failed!"); 

	// Coordinates({10,5})
	second.setPositionInParent(10,5);
	x = rootSquare
		.Intersects(secondSquare);
	std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	if(!x) throw std::logic_error("Test Failed!"); 
	
	// Coordinates({11,5})
	second.setPositionInParent(11,5);
	x = rootSquare
		.Intersects(secondSquare);
	std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	if(x) throw std::logic_error("Test Failed!"); 

	// Coordinates({-10,5})
	second.setPositionInParent(-10,5);
	x = rootSquare
		.Intersects(secondSquare);
	std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	if(!x) throw std::logic_error("Test Failed!"); 

	// Coordinates({-10,5})
	root.setPositionInParent(-10,5);
	secondSquare.setSecondCoords(Coordinates({0,15}));
	x = rootSquare
		.Intersects(secondSquare);
	std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	if(!x) throw std::logic_error("Test Failed!"); 

	// Coordinates({-10,5}))
	root.setPositionInParent(-10,5);
	secondSquare.setSecondCoords(Coordinates({-1,15}));
	x = rootSquare
		.Intersects(secondSquare);
	std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	if(x) throw std::logic_error("Test Failed!"); 


	//

	// Coordinates({5,10})
	second.setPositionInParent(5,10);
	secondSquare.setSecondCoords(Coordinates({15,15}));
	x = rootSquare
		.Intersects(secondSquare);
	std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	if(!x) throw std::logic_error("Test Failed!"); 
	
	// Coordinates({5,11})
	second.setPositionInParent(5,11);
	secondSquare.setSecondCoords(Coordinates({15,15}));
	x = rootSquare
		.Intersects(secondSquare);
	std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	if(x) throw std::logic_error("Test Failed!"); 

	// Coordinates({5,-10})
	second.setPositionInParent(5,-10);
	secondSquare.setSecondCoords(Coordinates({15,15}));
	x = rootSquare
		.Intersects(secondSquare);
	std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	if(!x) throw std::logic_error("Test Failed!"); 

	// Coordinates({5,-10})
	second.setPositionInParent(5,-10);
	secondSquare.setSecondCoords(Coordinates({15,0}));
	x = rootSquare
		.Intersects(secondSquare);
	std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	if(!x) throw std::logic_error("Test Failed!"); 

	// Coordinates({5,-10})
	second.setPositionInParent(5, -10);
	secondSquare.setSecondCoords(Coordinates({15, -1}));
	x = rootSquare
		.Intersects(secondSquare);
	std::cout << "Collision: " << (x ? "True" : "False") << std::endl; 
	if(x) throw std::logic_error("Test Failed!"); 

	// const auto x = CollisionSquare(Coordinates({0,0}), true);
	// Node secondNode = Node(x);
	// root.setPositionInParent(10,10);

	// root.addChild(secondNode);
	// secondNode.setPositionInParent(3,3);

	// x->Intersects(*x);

	al_uninstall_audio();

	return 0;
}
