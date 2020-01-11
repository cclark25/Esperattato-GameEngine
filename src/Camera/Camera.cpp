#include "./Camera.h"
#include "../Image/Image.h"

namespace Esperatto {
	Camera::Camera(const Screen &screen) {
		this->data = new cameraData();
		this->data->referenceCount = 1;
		this->data->screen = const_cast<Screen *>(&screen);
	}

	Camera::Camera(const Camera &source) {
		this->data = source.data;
		this->data->referenceCount++;
	}

	Camera::~Camera() {
		this->data->referenceCount--;
		if (this->data->referenceCount == 0) {
			al_destroy_bitmap(this->data->canvas);
			delete this->data;
		}
	}

	void Camera::drawToScreen(Node rootNode) {
		al_set_target_bitmap(this->data->canvas);
		al_clear_to_color(al_map_rgb(0,0,0));
		Transform thisTransform;
		al_identity_transform(&thisTransform);
		al_translate_transform(
		    &thisTransform,
		    this->data->xPosition - this->data->xCenterOfRotation,
		    this->data->yPosition - this->data->yCenterOfRotation);
		al_rotate_transform(&thisTransform, this->data->rotationRadians);
		al_translate_transform(&thisTransform, this->data->xCenterOfRotation,
		                       this->data->yCenterOfRotation);

		for (SovereignNode node : rootNode.makeNodeSet(thisTransform)) {
			al_compose_transform(&node.transformation, &thisTransform);
			al_use_transform(&node.transformation);
			if (node.node.getSubType() == typeid(Image).hash_code()) {
				Bitmap layer = (*(Image *)node.node.getDataPtr()).getBitmap();
				al_draw_bitmap(layer, 0, 0, 0);
			}
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
} // namespace Esperatto