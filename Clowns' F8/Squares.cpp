#include "Application.h"
#include "Squares.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include <random>


Squares::Squares(float transition_time, bool is_scene_change, SCENE_TYPE scene_to_change, Color color) : Transition(transition_time)
{
	this->is_scene_change = is_scene_change;
	this->scene_to_change = scene_to_change;
	this->color = color;

	App->window->GetWindowSize(width, height);
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);

	square_width = (int)(width / total_squares_col);
	square_height = (int)(height / total_squares_row);

	for (int x = 0; x < total_squares_col; x++)
	{
		for (int y = 0; y < total_squares_row; y++)
		{
			positions.push_back({ (int)(x*square_width), (int)(y*square_height) });
		}
	}

	auto rng = std::default_random_engine{};
	std::shuffle(std::begin(positions), std::end(positions), rng);

}

Squares::~Squares()
{
}

void Squares::Entering()
{
	Transition::Entering();

	int num_Bars = (int)LerpValue(percent, 0, total_squares_col * total_squares_row);

	if (num_Bars < total_squares_col * total_squares_row)
		DrawSquares(num_Bars);
	else
		DrawSquares(total_squares_col * total_squares_row);
}

void Squares::Action()
{
	DrawSquares(total_squares_col * total_squares_row);

	if (is_scene_change)
	{
		App->scene_manager->ChangeScene(scene_to_change);
	}

	Transition::Action();
}

void Squares::Exiting()
{
	Transition::Exiting();

	int num_Bars = (int)LerpValue(percent, total_squares_col * total_squares_row, 0);


	if (num_Bars > 0 && num_Bars < total_squares_col * total_squares_row)
		DrawSquares(num_Bars);
	else if (num_Bars > total_squares_col * total_squares_row)
		DrawSquares(total_squares_col * total_squares_row);

	
		

}

void Squares::SetColor(Color color)
{
	this->color = color;
}

void Squares::DrawSquares(int num_Squares)
{
	SDL_Rect* Squares = new SDL_Rect[num_Squares];

	for (int i = 0; i < num_Squares; i++)
	{
		Squares[i].x = positions[i].first;
		Squares[i].y = positions[i].second;
		Squares[i].w = square_width;
		Squares[i].h = square_height;
	}

	SDL_SetRenderDrawColor(App->render->renderer, color.r, color.g, color.b, 255);
	SDL_RenderFillRects(App->render->renderer, Squares, num_Squares);
}