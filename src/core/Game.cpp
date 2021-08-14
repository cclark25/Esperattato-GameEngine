#include "./Game.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

namespace Esperatto {
	Game::Game(Configuration &config)
	    : display(config.screenWidth, config.screenHeight,
	              config.framesPerSecond),
	      camera(this->display), 
		  keyboard() {
		this->config = config;
		for (unsigned int i = 0; i < this->config.workerCount; i++) {
			this->workers.push_back(ThreadWorker(
			    this->threadWork, (1.00s / this->config.framesPerSecond)));
		}
		camera.setCanvasSize(config.screenWidth, config.screenHeight);
		camera.setCanvasColor(config.canvasColor);
		rootNode.addChild(camera);
	}

	void Game::StartGame() {
		al_init();

		al_init_image_addon();
		al_init_primitives_addon();
		al_install_audio();

		al_reserve_samples(1);

		for (ThreadWorker &worker : this->workers) {
			worker.take_off_standby();
		}

		this->display.setFullscreen(true);
		this->display.setPixelStretch(this->config.screenPixelStretchX /
		                                  this->config.screenPixelStretchY,
		                              1);

		while (!this->shouldStop) {
			this->camera.drawToScreen(this->rootNode);
		}

		al_uninstall_audio();
	}

	void Game::StopGame() { this->shouldStop = true; }

} // namespace Esperatto