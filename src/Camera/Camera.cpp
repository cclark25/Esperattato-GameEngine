#include "./Camera.h"
#include "../Animation/Animation.h"
#include "../Image/Image.h"
#include <allegro5/allegro_primitives.h>

namespace Esperatto {
	Camera::Camera(const Screen &screen) {
		this->data = shared_ptr<cameraData>(new cameraData());
		this->data->referenceCount = 1;
		this->data->screen = const_cast<Screen *>(&screen);
		this->setCenterOfRotation(this->data->width / 2.0,
		                          this->data->height / 2.0);
		al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
		this->data->canvas =
		    al_create_bitmap(this->data->width, this->data->height);
	}

	Camera::Camera(const Camera &source) {
		this->data = source.data;
		this->data->referenceCount++;
	}

	Camera::~Camera() {
		this->data->referenceCount--;
		if (this->data->referenceCount == 0) {
			al_destroy_bitmap(this->data->canvas);
			// delete this->data;
		}
	}

	void Camera::drawToScreen(Node rootNode) {
		al_set_target_bitmap(this->data->canvas);
		al_clear_to_color(al_map_rgb(128, 128, 128));
		Transform thisTransform;
		al_identity_transform(&thisTransform);
		al_translate_transform(&thisTransform, -(this->data->width / 2.0),
		                       -(this->data->height / 2.0));
		al_scale_transform(&thisTransform, 1 / this->data->zoom,
		                   1 / this->data->zoom);
		al_rotate_transform(&thisTransform, this->data->rotationRadians);
		al_translate_transform(
		    &thisTransform, this->data->xPosition + ((this->data->width / 2.0)),
		    this->data->yPosition + (this->data->height / 2.0));
		al_invert_transform(&thisTransform);

		for (SovereignNode node : rootNode.makeNodeSet(thisTransform)) {
			if (this->shouldDraw(node.transformation, 0, 0, 0)) {
				al_use_transform(&node.transformation);
				Bitmap layer = nullptr;
				if (node.node.getSubType() == typeid(Image).hash_code()) {
					layer = (*(Image *)node.node.getDataPtr().get()).getBitmap();
				} else if (node.node.getSubType() ==
				           typeid(Animation).hash_code()) {
					layer = (*(Animation *)node.node.getDataPtr().get()).getBitmap();
				} else {
					// throw bad_exception();
				}
				if (layer) {
					al_draw_bitmap(layer, 0, 0, 0);
				}
			}
		}

		if (this->data->drawAnchors) {
			al_use_transform(&thisTransform);
			al_draw_line(this->data->xPosition + this->data->width / 2,
			             this->data->yPosition + this->data->height / 2,
			             20 + this->data->xPosition + this->data->width / 2,
			             this->data->yPosition + this->data->height / 2,
			             al_map_rgb(255, 0, 0), 1);
			al_draw_line(this->data->xPosition + this->data->width / 2,
			             this->data->yPosition + this->data->height / 2,
			             this->data->xPosition + this->data->width / 2,
			             -20 + this->data->yPosition + this->data->height / 2,
			             al_map_rgb(0, 0, 255), 1);
			al_draw_filled_circle(this->data->xPosition + this->data->width / 2,
			                      this->data->yPosition +
			                          this->data->height / 2,
			                      2, al_map_rgb(255, 255, 255));
		}

		this->data->screen->pushFrame(this->data->canvas);
	}

	void Camera::move(double x, double y) {
		this->data->xPosition += x;
		this->data->yPosition += y;
	}

	void Camera::rotate(double radians) {
		this->data->rotationRadians += radians;
	}

	void Camera::zoomIn(double zoom) { this->data->zoom += zoom; }

	void Camera::setCanvasSize(int width, int height) {
		this->data->width = width;
		this->data->height = height;
		al_destroy_bitmap(this->data->canvas);
		this->data->canvas = al_create_bitmap(width, height);
	}

	int Camera::getCanvasWidth() { return this->data->width; }
	int Camera::getCanvasHeight() { return this->data->height; }

	void Camera::setPosition(double x, double y) {
		this->data->xPosition = x;
		this->data->yPosition = y;
	}
	Coordinates Camera::getPosition() {
		return {this->data->xPosition, this->data->yPosition};
	}

	void Camera::setRotation(double radians) {
		this->data->rotationRadians = radians;
	}

	double Camera::getRotation() { return this->data->rotationRadians; }

	Coordinates Camera::getCenterOfRotation() {
		return {this->data->xCenterOfRotation, this->data->yCenterOfRotation};
	}

	void Camera::setCenterOfRotation(double x, double y) {
		this->data->xCenterOfRotation = x;
		this->data->yCenterOfRotation = y;
	}

	double Camera::getZoom() { return this->data->zoom; }
	void Camera::setZoom(double zoom) { this->data->zoom = zoom; }

	void Camera::toggleAnchor() {
		this->data->drawAnchors = !this->data->drawAnchors;
	}
	void Camera::toggleAnchor(bool enable) { this->data->drawAnchors = enable; }

	bool Camera::shouldDraw(Transform &t, Bitmap bitmap, float destination_x,
	                        float destination_y) {

		pair<float, float> topLeft = {0, 0};
		al_transform_coordinates(&t, &topLeft.first, &topLeft.second);
		pair<float, float> topRight = {this->data->width, 0};
		al_transform_coordinates(&t, &topRight.first, &topRight.second);
		pair<float, float> bottomLeft = {0, this->data->height};
		al_transform_coordinates(&t, &bottomLeft.first, &bottomLeft.second);
		pair<float, float> bottomRight = {this->data->width,
		                                  this->data->height};
		al_transform_coordinates(&t, &bottomRight.first, &bottomRight.second);

		float leftmost =
		    std::min(std::min(topLeft.first, topRight.first),
		             std::min(bottomLeft.first, bottomRight.first));
		float rightmost =
		    std::max(std::max(topLeft.first, topRight.first),
		             std::max(bottomLeft.first, bottomRight.first));
		float topmost =
		    std::min(std::min(topLeft.second, topRight.second),
		             std::min(bottomLeft.second, bottomRight.second));
		float bottommost =
		    std::max(std::max(topLeft.second, topRight.second),
		             std::max(bottomLeft.second, bottomRight.second));

		bool shouldDraw = (destination_x + leftmost) <= this->data->width;
		shouldDraw =
		    shouldDraw && (destination_y + topmost) <= this->data->height;
		shouldDraw = shouldDraw && (destination_x + rightmost) > 0;
		shouldDraw = shouldDraw && (destination_y + bottommost) > 0;
		return shouldDraw;
	}
} // namespace Esperatto