/* 
 * libmbed-graphics 2D and wireframe 3D graphics library for the MBED
 * microcontroller platform
 * Copyright (C) <2009> Michael Sheldon <mike@mikeasoft.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef MBED_OBJECT3D_H
#define MBED_OBJECT3D_H

#include "Graphics.h"

/* Class: Object3D
 * General purpose 3D base object, new 3D objects should inherit
 * from this class.
 */
class Object3D {

    public:
        Object3D();
        
        /* Function: position
         * Specify the position of the object in 3D space.
         *
         * Parameters:
         * x - X co-ordinate of the point that the centre of the object should be placed at.
         * y - Y co-ordinate of the point that the centre of the object should be placed at.
         * z - Z co-ordinate of the point that the centre of the object should be placed at.
         */
        void position(int x, int y, int z);
        
        /* Function: rotate
         * Rotate the object about it's centre point.
         *
         * Parameters:
         * rx - Component of the rotation along the x-axis in radians.
         * ry - Component of the rotation along the y-axis in radians.
         * rz - Component of the rotation along the z-axis in radians.
         */
        void rotate(float rx, float ry, float rz);
        
        /* Function: scale
         * Modify the size of the object.
         *
         * Parameters:
         * sx - Proportion to change the object's size by along the x axis.
         * sy - Proportion to change the object's size by along the y axis.
         * sz - Proportion to change the object's size by along the z axis.
         */
        void scale(float sx, float sy, float sz);
        
        /* Function: colour
         * Sets the colour of the object.
         *
         * Parameters:
         * colour - The colour of the object.
         */
        void colour(int colour);
        
        /* Function: render
         * Draws the object on the specified graphical context.
         *
         * Parameters:
         * g - The graphical context to which the object should be rendered.
         */
        virtual void render(Graphics &g) = 0;
        
    protected:
        int _x, _y, _z;
        float _rx, _ry, _rz;
        float _sx, _sy, _sz;
        int _colour;        
};

        
/* Function: rotate3d
 * Rotates point(s) in 3D space about origin (0,0,0)
 *
 * Parameters:
 * x - Pointer to the x co-ordinate(s) of the point(s) to be rotated.
 * y - Pointer to the y co-ordinate(s) of the point(s) to be rotated.
 * z - Pointer to the z co-ordinate(s) of the point(s) to be rotated.
 * anglex - The angle to rotate the point by around axis x in radians.
 * angley - The angle to rotate the point by around axis y in radians.
 * anglez - The angle to rotate the point by around axis z in radians.
 * count  - count of coordinates pointed to by x, y and z
 */
void rotate3d(int *x, int *y, int *z, float anglex, float angley, float anglez, int count = 1);

#endif