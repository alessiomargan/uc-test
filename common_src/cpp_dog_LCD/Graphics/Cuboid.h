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
 
#ifndef MBED_CUBOID_H
#define MBED_CUBOID_H

#include "Object3D.h"

/* Class: Cuboid
 * Displays a 3D wireframe cuboid.
 */
class Cuboid : public Object3D {

    public:
        Cuboid();
        
        /* Function: render
         * Draws the cuboid to the specified graphical context.
         *
         * Parameters:
         * g - The graphical context to which this cuboid should be rendered.
         */
        virtual void render(Graphics &g);
        
};
#endif