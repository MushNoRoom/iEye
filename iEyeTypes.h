#ifndef IEYETYPES_H
#define IEYETYPES_H

/*!
 * \file Containning important typedef for the iEye app
 */

namespace iEye
{
    typedef enum SceneMovement
    {
		NoMove,
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        MoveCenter

    }SceneMovement;

	const int FRAME_WIDTH = 160;
	const int FRAME_HEIGHT = 120;
}

#endif // IEYETYPES_H
