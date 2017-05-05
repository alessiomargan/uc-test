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
 
#include "Object3D.h"

Object3D::Object3D() {
    _x = 0;
    _y = 0;
    _z = 0;
    _rx = 0;
    _ry = 0;
    _rz = 0;
    _sx = 1;
    _sy = 1;
    _sz = 1;
    _colour = 0xffffff;
}

void Object3D::position(int x, int y, int z) {
    _x = x;
    _y = y;
    _z = z;
}

void Object3D::rotate(float rx, float ry, float rz) {
    _rx = rx;
    _ry = ry;
    _rz = rz;
}

void Object3D::scale(float sx, float sy, float sz) {
    _sx = sx;
    _sy = sy;
    _sz = sz;
}

void Object3D::colour(int colour) {
    _colour = colour;
}

#define FASTSINCOS

#ifdef FASTSINCOS

// http://www.dspguru.com/dsp/tricks/parabolic-approximation-of-sin-and-cos

#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616
#define M_C        0.71256755058

void sincosf(float angle, float *sinx, float *cosx)
{
    int quarter = int(floor(angle/M_PI_2));
    
    float modphase = angle - quarter*M_PI_2;
    if ( modphase < 0 )
        modphase += M_PI_2;
    
    quarter %= 4;
    if ( quarter < 0 )
        quarter += 4;
    
    modphase = modphase/M_PI_2;

    //printf("Angle: %f, q: %d, mod: %f\n", angle, quarter, modphase);
    float x, temp;
    switch (quarter)
    { 
    case 0:
        // First quarter, angle = 0 .. pi/2
        x = modphase - 0.5f;      // 1 sub
        temp = (2 - 4*M_C)*x*x + M_C; // 2 mul, 1 add
        *sinx = temp + x;              // 1 add
        *cosx = temp - x;              // 1 sub
        break;
    case 1:
        // Second quarter, angle = pi/2 .. pi
        x = 0.5f - modphase;      // 1 sub
        temp = (2 - 4*M_C)*x*x + M_C; // 2 mul, 1 add
        *sinx = x + temp;              // 1 add
        *cosx = x - temp;              // 1 sub
        break;
    case 2:
        // Third quarter, angle = pi .. 1.5pi
        x = modphase - 0.5f;      // 1 sub
        temp = (4*M_C - 2)*x*x - M_C; // 2 mul, 1 sub
        *sinx = temp - x;              // 1 sub
        *cosx = temp + x;              // 1 add
        break;
    case 3:
        // Fourth quarter, angle = 1.5pi..2pi
        x = modphase - 0.5f;      // 1 sub
        temp = (2 - 4*M_C)*x*x + M_C; // 2 mul, 1 add
        *sinx = x - temp;              // 1 sub
        *cosx = x + temp;              // 1 add
        break;
    }
}

#else // FASTSINCOS

void sincosf(float angle, float *sinx, float *cosx)
{
  *sinx = sin(angle);
  *cosx = cos(angle);
}

#endif // FASTSINCOS


void rotate3d(int *x, int *y, int *z, float anglex, float angley, float anglez, int count)
{    
    float sinx, cosx, siny, cosy, sinz, cosz;
    sincosf(anglex, &sinx, &cosx);    
    sincosf(angley, &siny, &cosy);
    sincosf(anglez, &sinz, &cosz);
    
    for ( int i=0; i < count; i++)
    {
        int x1 = x[i];
        int y1 = y[i];
        int z1 = z[i];
        
        int y2 = cosx * y1 - sinx * z1;
        int z2 = sinx * y1 + cosx * z1;
    
        int x2 = cosy * x1 + siny * z2;
        int z3 = -siny * x1 + cosy * z2;
        
        int x3 = cosz * x2 - sinz * y2;
        int y3 = sinz * x2 + cosz * y2;
        
        x[i] = x3;
        y[i] = y3;
        z[i] = z3;
    }
}
