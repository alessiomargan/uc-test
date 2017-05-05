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

#include "Cuboid.h"

// six sides, four vertices in each
int _cube[6][4][3]={
    {
        {-10,-10,-10},
        { 10,-10,-10},
        { 10, 10,-10},
        {-10, 10,-10}
    },{
        {-10,-10, 10},
        { 10,-10, 10},
        { 10, 10, 10},
        {-10, 10, 10}
    },{
        {-10,-10,-10},
        { 10,-10,-10},
        { 10,-10, 10},
        {-10,-10, 10}
    },{
        {-10, 10,-10},
        { 10, 10,-10},
        { 10, 10, 10},
        {-10, 10, 10}
    },{
        {-10,-10,-10},
        {-10, 10,-10},
        {-10, 10, 10},
        {-10,-10, 10}
    },{
        { 10,-10,-10},
        { 10, 10,-10},
        { 10, 10, 10},
        { 10,-10, 10}
    }
};


Cuboid::Cuboid()
        : Object3D::Object3D()  {

}

void Cuboid::render(Graphics &g)
{
    int side, v;
    int x[4], y[4], z[4];
    
    for (side = 0; side < 6; side++)
    {
        for (v = 0; v < 4; v++)
        {
            x[v] = _cube[side][v][0];
            y[v] = _cube[side][v][1];
            z[v] = _cube[side][v][2];
        }
        rotate3d(x, y, z, _rx, _ry, _rz, 4);
        for (v = 0; v < 4; v++)
        {
            x[v] += _x;
            y[v] += _y;
            z[v] += _z;
        }
        for (v = 0; v < 4; v++)
        {
            g.line3d(x[v], y[v], z[v], x[(v+1)%4], y[(v+1)%4], z[(v+1)%4], _colour);
        }
    }

    /*x0 = _cube[1][0][0];
    y0 = _cube[1][0][1];
    z0 = _cube[1][0][2];
    rotate3d(&x0, &y0, &z0, _rx, _ry, _rz);
    x1 = _cube[0][0][0];
    y1 = _cube[0][0][1];
    z1 = _cube[0][0][2];
    rotate3d(&x1, &y1, &z1, _rx, _ry, _rz);
    g.line3d(x0 + _x, y0 + _y, z0 + _z, x1 + _x, y1 + _y, z1 + _z, _colour);*/
}