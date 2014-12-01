/**
 This file is part of YeoLibrary.
 
 YeoLibrary is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, version 3.
 
 YeoLibrary is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with YeoLibrary.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <yeo/math.hpp>
#define YEO_DLL_EXPORTS

using namespace yeo;
using namespace arma;

YEO_API void YeoIdentity(yeo::float4x4& m) {
    m.eye();
}

YEO_API void YeoOrtho(double left,
                      double right,
                      double bottom,
                      double top,
                      double near,
                      double far,
                      float4x4& m) {
    double tx = (right + left) / (right - left);
    double ty = (top + bottom) / (top - bottom);
    double tz = (far + near) / (far - near);
    m << 2.0f / (right - left) << 0.f << 0.f << tx << endr
    << 0.f << 2.0f / (top - bottom) << 0.f  << ty << endr
    << 0.f << 0.f << -2.0f / (far - near) << tz << endr
    << 0.f << 0.f << 0.f << 1.f << endr;
}

YEO_API void YeoRotate(float angle,
                       const float3& axis,
                       float4x4& m){
    const float3& normAxis = normalise(axis);
    float c = cosf(angle);
    float s = sinf(angle);
    float x = normAxis(0);
    float y = normAxis(1);
    float z = normAxis(2);
    m << x*x*(1-c)+c << x*y*(1-c)-z*s << x*z*(1-c)+y*s << 0.0f << endr
      << y*x*(1-c) +z*s << y*y*(1-c)+c << y*z*(1-c)-x*s << 0.0f << endr
      << x*z*(1-c)-y*s << y*z*(1-c)+x*s << z*z*(1-c)+c << 0.0f << endr
      << 0.0f << 0.0f << 0.0f << 1.0f << endr;
}