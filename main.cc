#include <iostream>
#include <SDL2/SDL.h>

#define BLOCK_VOID 0

using namespace std;

class Camera;
class Entity;

class Block {
	public:
		int id;
		Block();
};

Block::Block()
{
	id = BLOCK_VOID;
}

class World {
	public:
		int width, height; // blocks
		Block **grid;
		World(int, int);
};

World::World(int w, int h)
{
	width = w;
	height = h;
	/* init world grid */
	grid = new Block*[width];
	for (int i = 0; i < width; i++)
		grid[i] = new Block[height];
}

class Camera {
	private:
		int scale;         // pixel per block edge
	public:
		int width, height; // pixels
		float x, y;        // center block 
		World *world;
		SDL_Renderer *renderer;
		SDL_Window *window;
		Camera(World *, int, int, float, float, float);
		void render();
		bool zoom_in(int);
		bool zoom_out(int);
};

Camera::Camera(World *world, int width, int height, float x, float y, float scale)
{
	this->world = world;
	this->width = width;
	this->height = height;
	this->x = x;
	this->y = y;
	this->scale = scale;
	window = SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void
Camera::render()
{
	int n_width_block = ceil(width / scale) + 1;
	int n_height_block = ceil(height / scale) + 1;
	int x_origin = (n_width_block * scale - width) / 2;
	int y_origin = (n_height_block * scale - height) / 2;
	for (int i = 0 ; i < n_width_block; i++) {
		for (int j = 0; j < n_height_block; j++) {
			SDL_Rect rect = { - x_origin + i * scale, - y_origin + j * scale , scale, scale};
			if (!((j + i) % 2)) {
				SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
				SDL_RenderDrawRect(renderer, &rect);
			} else {
				SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
				SDL_RenderFillRect(renderer, &rect);
			}
			//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		}
	}
}

bool
Camera::zoom_in(int factor)
{
	scale += factor;
	return true;
}

bool
Camera::zoom_out(int factor)
{
	if (ceil(width / scale) + 1 <= world->width && ceil(height / scale) + 1 <= world->height) {
		scale -= factor;
		return true;
	}
	return false;
}

class Entity {
	public:
		float velocity[2];
		float x, y;
		Entity(float, float);
		void move();
		void render(SDL_Renderer *);
};

Entity::Entity(float x, float y)
{
	this->x = x;
	this->y = y;
}

void
Entity::move()
{
	/* move toward x axis */
	x += velocity[0];
	/* y axis */
	y += velocity[1];

}

void
Entity::render(SDL_Renderer *rend)
{

	SDL_Rect rect = { (int)x, (int)y, 10, 10, };
	SDL_SetRenderDrawColor(rend, 255, 165, 0, 255);
	SDL_RenderDrawRect(rend, &rect);
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
}

int
main(int argc, char *argv[])
{
	SDL_Event ev;
	bool close;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		cerr << "error: can't init SDL: " << SDL_GetError();
	World world(50, 50);
	Camera camera(&world, 1000, 1000, 25, 25, 80);
	SDL_Rect background;
	background.x = 0;
	background.y = 0;
	background.w = 1000;
	background.h = 1000;
	Entity ent = Entity(500, 500);
	ent.velocity[0] = 0;
	ent.velocity[1] = 0;
	close = false;
	while (!close) {
		while (SDL_PollEvent(&ev)) {
			switch (ev.type) {
			case SDL_QUIT:
				close = true;
				break;
			case SDL_KEYDOWN:
				switch (ev.key.keysym.scancode) {
				case SDL_SCANCODE_W:
					ent.velocity[1] -= 0.1;
					break;
				case SDL_SCANCODE_A:
					ent.velocity[0] -= 0.1;
					break;
				case SDL_SCANCODE_S:
					ent.velocity[1] += 0.1;
					break;
				case SDL_SCANCODE_D:
					ent.velocity[0] += 0.1;
					break;
				case SDL_SCANCODE_U:
					camera.zoom_out(1);
					break;
				case SDL_SCANCODE_I:
					camera.zoom_in(1);
					break;
				case SDL_SCANCODE_Q:
					close = true;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
		SDL_RenderClear(camera.renderer);
		SDL_SetRenderDrawColor(camera.renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(camera.renderer, &background);
		SDL_SetRenderDrawColor(camera.renderer, 0, 0, 0, 255);
		camera.render();
		ent.move();
		ent.render(camera.renderer);
        SDL_RenderPresent(camera.renderer);

        SDL_Delay(1000 / 60);
	}
	SDL_DestroyRenderer(camera.renderer);
    SDL_DestroyWindow(camera.window);
    SDL_Quit();
	return 0;
}
