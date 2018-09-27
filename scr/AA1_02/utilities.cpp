#include "SDL.h"
#include "utilities.h"

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
