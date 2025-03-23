#include "../include/Bomber.h"
#include <SDL_image.h>
#include <iostream>

int const SPRITESHEET_UP = 0;
int const SPRITESHEET_LEFT = 1;
int const SPRITESHEET_RIGHT = 2;
int const SPRITESHEET_DOWN = 3;

Bomber::Bomber() : m_image("assets/texture/Player.png", 4, 4)
{
	m_position.x = 0;
	m_position.y = 0;
	m_position.w = 22;
	m_position.h = 43;

	m_x = 0.0;
	m_y = 0.0;

	m_direction = Direction::NONE;

	m_image.select_sprite(0, 2);
	m_spritesheet_column = 0;
}

void Bomber::update(double delta_time) {

	switch (m_direction)
	{
	case Direction::NONE:
		m_x += 0.0;
		m_y += 0.0;
		m_image.select_sprite(0, 0);
		break;
	case Direction::UP:
		m_y = m_y - (500.0 * delta_time);
		m_image.select_sprite(m_spritesheet_column, SPRITESHEET_UP);
		break;
	case Direction::DOWN:
		m_y = m_y + (500.0 * delta_time);
		m_image.select_sprite(m_spritesheet_column, SPRITESHEET_DOWN);
		break;
	case Direction::LEFT:
		m_x = m_x - (500.0 * delta_time);
		m_image.select_sprite(m_spritesheet_column, SPRITESHEET_LEFT);
		break;
	case Direction::RIGHT:
		m_x = m_x + (500.0 * delta_time);
		m_image.select_sprite(m_spritesheet_column, SPRITESHEET_RIGHT);
		break;
	}

	m_position.x = m_x;
	m_position.y = m_y;
	m_spritesheet_column++;
	if (m_spritesheet_column > 3)
		m_spritesheet_column = 0;
}

void Bomber::draw(SDL_Surface* window_surface)
{
	m_image.draw_selected_sprite(window_surface, &m_position, 2.0f);
	SDL_Delay(100);
}

void Bomber::handleInput(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		Uint8 const* keys = SDL_GetKeyboardState(nullptr);

		if (keys[SDL_SCANCODE_W] == 1)
			m_direction = Direction::UP;
		else if (keys[SDL_SCANCODE_S] == 1)
			m_direction = Direction::DOWN;
		else if (keys[SDL_SCANCODE_A] == 1)
			m_direction = Direction::LEFT;
		else if (keys[SDL_SCANCODE_D] == 1)
			m_direction = Direction::RIGHT;
		break;
	}
}