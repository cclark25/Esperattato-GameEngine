// #include "./ThreadPool/ThreadWorker.h"
// #include "./ThreadPool/Process.h"
#include "Image/Image.h"
#include "Node/Node.h"
#include "Screen/Screen.h"
#include <allegro5/allegro_image.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <map>
#include <unistd.h>

using namespace Esperatto;

int main(int argc, char **args) {
	al_init();
	al_init_image_addon();

	Node root = Node(new Image("Test_Files/RedBox.png"));

	root.setPositionInParent(1, 1);
	root.setCenterOfRotation(128, 112);

	Esperatto::Screen d(256, 224, 360);
	Bitmap b = al_load_bitmap("Test_Files/RedBox.png");
	d.pushFrame(b);
	Node last = root;
	for (int i = 0; i < 20; i++) {
		Node newNode(new Image("Test_Files/stock_image.png"));
		// newNode.setPositionInParent(0.01, 0.02);
		newNode.setZIndexInParent(0.01);
		newNode.setCenterOfRotation(128, 112);
		last.addChild(newNode);
		last = newNode;
	}

	// auto pos = last.getGlobalPosition();
	// cout << "Lowest node: " << pos.x << ", " << pos.y << endl;
	const int i = 256;
	double total = 0;
	d.setFullscreen(true);
	d.setPixelStretch(7.0 / 6.0, 1);
	Bitmap b3 = al_create_bitmap(256, 224);
	for (int j = i; j > 0; j--) {
		static map<void *, double> *parentMap = new map<void *, double>();
		auto before = chrono::high_resolution_clock::now();
		al_set_target_bitmap(b3);
		last.move(1, 0);

		for (SovereignNode node : root.makeNodeSet()) {
			static int xPos = 0;
			xPos += 1;
			xPos = (xPos % (2 * 3072));
			node.node.move(0.05, 0.05);
			// al_translate_transform(&node.transformation, -128, -112);
			// al_rotate_transform(&node.transformation, (xPos) * 3.1416 / 6);
			// al_translate_transform(&node.transformation, 128, 112);
			// al_translate_transform(&node.transformation, xPos, xPos);
			al_use_transform(&node.transformation);
			al_set_target_bitmap(b3);
			// cout << "This Type Hash: " << node.node.getNodeType() << endl;
			// cout << "Node Type Hash: " << Node::getType() << endl;
			if (node.node.getSubType() == typeid(Image).hash_code()) {
				Bitmap layer = (*(Image *)node.node.getDataPtr()).getBitmap();
				al_draw_bitmap(layer, 0, 0, 0);
			}
			// al_draw_bitmap(b, 0, 0, 0);
		}
		// delete parentMap;
		d.pushFrame(b3);
		total += 1 / chrono::duration_cast<chrono::duration<double>>(
		                 chrono::high_resolution_clock::now() - before)
		                 .count();
	}
	al_destroy_bitmap(b3);
	cout << "Average Framerate: " << total / i << endl;
	cout << b << endl;
	return 0;
}