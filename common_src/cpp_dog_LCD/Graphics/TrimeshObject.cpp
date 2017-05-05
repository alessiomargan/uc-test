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
 
#include "TrimeshObject.h"
 
TrimeshObject::TrimeshObject(int vertices[][3], int faces[][3], int num_faces)
    : Object3D::Object3D() {
     _vertices = vertices;
     _faces = faces;
     _num_faces = num_faces;
}

void TrimeshObject::render(Graphics &g) {
    int face, fv0, fv1, fv2;
    int *v0, *v1, *v2;
    int x[3], y[3], z[3];
    for (face = 0; face < _num_faces; face++) {
        fv0 = _faces[face][0];
        fv1 = _faces[face][1];
        fv2 = _faces[face][2];
        v0 = _vertices[fv0];
        v1 = _vertices[fv1];
        v2 = _vertices[fv2];

        x[0] = v0[0];    x[1] = v1[0];    x[2] = v2[0];
        y[0] = v0[1];    y[1] = v1[1];    y[2] = v2[1];
        z[0] = v0[2];    z[1] = v1[2];    z[2] = v2[2];
        rotate3d(x, y, z, _rx, _ry, _rz, 3);
        
        for ( int i=0; i < 3; i++ )
        {
            x[i] += _x;
            y[i] += _y;
            z[i] += _z;
        }
        
        g.line3d(x[0], y[0], z[0], x[1], y[1], z[1], _colour);
        g.line3d(x[1], y[1], z[1], x[2], y[2], z[2], _colour);
        g.line3d(x[2], y[2], z[2], x[0], y[0], z[0], _colour);
    }
}