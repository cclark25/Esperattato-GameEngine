#include "./Camera.h"
#include "../Animation/Animation.h"
#include "../Image/Image.h"
#include <allegro5/allegro_primitives.h>

namespace Esperatto
{
	Camera::Camera(const Screen &screen) : Node()
	{
		this->referenceCount = 1;
		this->screen = const_cast<Screen *>(&screen);
		this->setCenterOfRotation(this->width / 2.0,
								  this->height / 2.0);
		al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
		this->canvas =
			al_create_bitmap(this->width, this->height);
	}

	Camera::~Camera()
	{
		if (this->referenceCount == 0)
		{
			al_destroy_bitmap(this->canvas);
		}
	}

	void Camera::drawToScreen(Node &rootNode)
	{
		al_set_target_bitmap(this->canvas);
		al_clear_to_color(this->canvasColor);
		static Transform thisTransform;
		al_identity_transform(&thisTransform);
		al_translate_transform(&thisTransform, -(this->width / 2.0),
							   (this->height / 2.0));
		al_scale_transform(&thisTransform, 1 / this->zoom,
						   1 / this->zoom);
		al_rotate_transform(&thisTransform, this->getGlobalRotation());
		Coordinates globalPosition = this->getGlobalPosition();
		al_translate_transform(
			&thisTransform, globalPosition.x + ((this->width / 2.0)),
			-(globalPosition.y + (this->height / 2.0)));
		al_invert_transform(&thisTransform);

		for (SovereignNode node : rootNode.makeNodeSet(thisTransform))
		{
			if (this->shouldDraw(node.transformation, 0, 0, 0))
			{
				al_use_transform(&node.transformation);
				Bitmap layer = node.node->getBitmap();

				if (layer)
				{
					al_draw_bitmap(layer, -globalPosition.x, globalPosition.y, 0);
				}
			}
		}

		if (this->drawAnchors)
		{
			al_use_transform(&thisTransform);
			al_draw_line(this->width / 2,
						 this->height / 2,
						 20 + this->width / 2,
						 this->height / 2,
						 al_map_rgb(255, 0, 0), 1);
			al_draw_line(this->width / 2,
						 this->height / 2,
						 this->width / 2,
						 -20 + this->height / 2,
						 al_map_rgb(0, 0, 255), 1);
			al_draw_filled_circle(this->width / 2,
								  this->height / 2,
								  2, al_map_rgb(255, 255, 255));
		}

		this->screen->pushFrame(this->canvas);
	}

	void Camera::zoomIn(double zoom) { this->zoom += zoom; }

	void Camera::setCanvasSize(int width, int height)
	{
		this->width = width;
		this->height = height;
		al_destroy_bitmap(this->canvas);
		this->canvas = al_create_bitmap(width, height);
	}
	void Camera::setCanvasColor(ALLEGRO_COLOR color)
	{
		this->canvasColor = color;
	}

	int Camera::getCanvasWidth() { return this->width; }
	int Camera::getCanvasHeight() { return this->height; }

	double Camera::getZoom() { return this->zoom; }
	void Camera::setZoom(double zoom) { this->zoom = zoom; }

	void Camera::toggleAnchor()
	{
		this->drawAnchors = !this->drawAnchors;
	}
	void Camera::toggleAnchor(bool enable) { this->drawAnchors = enable; }

	bool Camera::shouldDraw(Transform &t, Bitmap bitmap, float destination_x,
							float destination_y)
	{
		Coordinates cameraPosition = this->getGlobalPosition();
		destination_x -= cameraPosition.x;
		destination_y += cameraPosition.y;

		pair<float, float> topLeft = {0, 0};
		al_transform_coordinates(&t, &topLeft.first, &topLeft.second);
		pair<float, float> topRight = {this->width, 0};
		al_transform_coordinates(&t, &topRight.first, &topRight.second);
		pair<float, float> bottomLeft = {0, this->height};
		al_transform_coordinates(&t, &bottomLeft.first, &bottomLeft.second);
		pair<float, float> bottomRight = {this->width,
										  this->height};
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

		bool shouldDraw = (destination_x + leftmost) <= this->width;
		shouldDraw =
			shouldDraw && (destination_y + topmost) <= this->height;
		shouldDraw = shouldDraw && (destination_x + rightmost) > 0;
		shouldDraw = shouldDraw && (destination_y + bottommost) > 0;
		return shouldDraw;
	}
} // namespace Esperatto