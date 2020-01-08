// #include "./ThreadPool/ThreadWorker.h"
// #include "./ThreadPool/Process.h"
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

	Node root;

	root.setPositionInParent(1, 1);
	root.setCenterOfRotation(128, 112);
	root.setRotationInParent(3.1416 / 6);

	Esperatto::Screen d(256, 224, 360);
	Bitmap b = al_load_bitmap("Test_Files/RedBox.png");
	d.pushFrame(b);
	Node last = root;
	for (int i = 0; i < 12; i++) {
		Node newNode;
		// newNode.setPositionInParent(0.01, 0.02);
		newNode.setZIndexInParent(0.01);
		newNode.setCenterOfRotation(128, 112);
		newNode.setRotationInParent(3.1416 / 12);
		last.addChild(newNode);
		last = newNode;
	}

	// auto pos = last.getGlobalPosition();
	// cout << "Lowest node: " << pos.x << ", " << pos.y << endl;
	const int i = 100;
	double total = 0;
	d.setFullscreen(true);
	d.setPixelStretch(7.0 / 6.0, 1);
	for (int j = i; j > 0; j--) {
		static map<void *, double> *parentMap = new map<void *, double>();
		Bitmap b3 = al_create_bitmap(2560, 2240);
		auto before = chrono::high_resolution_clock::now();
		al_set_target_bitmap(b3);
		al_clear_to_color(al_map_rgb(0, 0, 0));
		root.setRotationInParent(root.getRotationInParent() + 3.1416 / 12);
		static int abc = 0;

		root.setScaleInParent(1 + (abc%20 - 10) / 10.0, 1 + (abc % 20 - 10) / 10.0);
		abc++;
		for (SovereignNode node : root.makeNodeSet()) {
			static int xPos = 0;
			xPos += 1;
			xPos = (xPos % (2 * 3072));
			// al_translate_transform(&node.transformation, -128, -112);
			// al_rotate_transform(&node.transformation, (xPos) * 3.1416 / 6);
			// al_translate_transform(&node.transformation, 128, 112);
			al_translate_transform(&node.transformation, xPos, xPos);
			al_use_transform(&node.transformation);
			al_set_target_bitmap(b3);
			al_draw_bitmap(b, 0, 0, 0);
		}
		// delete parentMap;
		total += 1 / chrono::duration_cast<chrono::duration<double>>(
		                 chrono::high_resolution_clock::now() - before)
		                 .count();
		d.pushFrame(b3);
		al_destroy_bitmap(b3);
	}
	cout << "Average Framerate: " << total / i << endl;
	cout << b << endl;
	return 0;
}