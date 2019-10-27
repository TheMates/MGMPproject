#ifndef _MGMPPROJECTNAMESPACE_
#define _MGMPPROJECTNAMESPACE_
#include <string>
#include "OBJ_Loader.h"

#define PIover180 0.017453292519943f
#define PI 3.14159265358979323846f

#define GAME_EXIT 1001
#define RUN_ANIMATION 1002
#define STOP_ANIAMTION 1003
#define RESET_PLAYER_POSITION 1004
#define WORLD_LOAD_LEVEL 1005


///<summary> Useful structures and functions for this project. </summary>
namespace MGMPproject
{
	struct WorldBorders
	{
		float TopBorder;
		float BottomBorder;
		float NegativeXBorder;
		float PositiveXBorder;
		float NegativeZBorder;
		float PositiveZBorder;
	};

	enum FontSize
	{
		Small,
		Middle,
		Large
	};
	
	/// <summary> Draws text on the screen. </summary>
	/// <param name="text"> Text to be drawn </param>
	/// <param name="xPosition"> X position from top left corner. </param>
	/// <param name="yPosition"> Y position from top left corner. </param>
	/// <param name="fontSize"> Small - 10, Middle - 12, Large - 18 </param>
	void drawText(const char* text, int xPosition, int yPosition, FontSize fontSize = FontSize::Small);

	/// <summary> Draws text on the screen. </summary>
	/// <param name="text"> Text to be drawn </param>
	/// <param name="xPosition"> X position from top left corner. </param>
	/// <param name="yPosition"> Y position from top left corner. </param>
	/// <param name="fontSize"> Small - 10, Middle - 12, Large - 18 </param>
	void drawText(const std::string text, int xPosition, int yPosition, FontSize fontSize = FontSize::Small);

	/// <summary> Draws object from blender loaded by objl::Loader class from OBJ_Loader.h. To see the actual material, be sure to enable light 
	/// before drawing the object. </summary>
	/// <param name="loader"> Pointer to loader object, that contains loaded models. </param>
	/// <param name="meshIndex"> Index of model, that should be drawn. If you have included multiple models with your loader file select which one you want to draw. </param>
	void drawBlenderObject(objl::Loader* loader, unsigned int meshIndex);

}

#endif