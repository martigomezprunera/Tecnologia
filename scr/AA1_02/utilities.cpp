#include "SDL.h"
#include "utilities.h"
#include "time.h"

bool utilities::hover(int xMouse, int yMouse, SDL_Rect rect)
{
	if (((xMouse > rect.x) && (xMouse < (rect.w + rect.x))))
	{
		if ((yMouse > rect.y) && (yMouse < (rect.h + rect.y)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool utilities::colision(SDL_Rect sprite, SDL_Rect rect)
{
	if (hover(sprite.x, sprite.y, rect))
	{
		return true;
	}
	if (hover((sprite.x + sprite.w), (sprite.y), rect))
	{
		return true;
	}
	if (hover((sprite.x), (sprite.y + sprite.h), rect))
	{
		return true;
	}
	if (hover((sprite.x + sprite.w), (sprite.y + sprite.h), rect))
	{
		return true;
	}
	return false;
}

