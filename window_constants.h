#pragma once
namespace window_const {
	constexpr int WINDOW_WIDTH = 1920;
	constexpr int WINDOW_HEIGHT = 1080;
	constexpr float ASPECT_RATIO = WINDOW_WIDTH / WINDOW_HEIGHT;
	constexpr float WORLD_HEIGHT = 10000;
	constexpr float WORLD_WIDTH = WORLD_HEIGHT * ASPECT_RATIO;
}