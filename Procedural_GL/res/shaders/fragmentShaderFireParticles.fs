#version 460 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform float elapsedTime;
uniform float gradRotSpeed;
uniform float verticalSpeed;


//uniform vec4 u_Color;
//uniform sampler2D u_Texture;


float psrdnoise(vec2 x, vec2 period, float alpha, out vec2 gradient) {

// Transform to simplex space (axis-aligned hexagonal grid)
vec2 uv = vec2(x.x + x.y*0.5, x.y);

// Determine which simplex we're in, with i0 being the "base"
vec2 i0 = floor(uv);
vec2 f0 = fract(uv);
// o1 is the offset in simplex space to the second corner
float cmp = step(f0.y, f0.x);
vec2 o1 = vec2(cmp, 1.0-cmp);

// Enumerate the remaining simplex corners
vec2 i1 = i0 + o1;
vec2 i2 = i0 + vec2(1.0, 1.0);

// Transform corners back to texture space
vec2 v0 = vec2(i0.x - i0.y * 0.5, i0.y);
vec2 v1 = vec2(v0.x + o1.x - o1.y * 0.5, v0.y + o1.y);
vec2 v2 = vec2(v0.x + 0.5, v0.y + 1.0);

// Compute vectors from v to each of the simplex corners
vec2 x0 = x - v0;
vec2 x1 = x - v1;
vec2 x2 = x - v2;

vec3 iu, iv;
vec3 xw, yw;

// Wrap to periods, if desired
if(any(greaterThan(period, vec2(0.0)))) {
    xw = vec3(v0.x, v1.x, v2.x);
    yw = vec3(v0.y, v1.y, v2.y);

    if(period.x > 0.0){
        xw = mod(vec3(v0.x, v1.x, v2.x), period.x);
    }

    if(period.y > 0.0){
        yw = mod(vec3(v0.y, v1.y, v2.y), period.y);
    }
   
    // Transform back to simplex space and fix rounding errors
    iu = floor(xw + 0.5*yw + 0.5);
    iv = floor(yw + 0.5);
} else { // Shortcut if neither x nor y periods are specified
    iu = vec3(i0.x, i1.x, i2.x);
    iv = vec3(i0.y, i1.y, i2.y);
}

// Compute one pseudo-random hash value for each corner
vec3 hash = mod(iu, 289.0);
hash = mod((hash*51.0 + 2.0)*hash + iv, 289.0);
hash = mod((hash*34.0 + 10.0)*hash, 289.0);

// Pick a pseudo-random angle and add the desired rotation
vec3 psi = hash * 0.07482 + alpha;
vec3 gx = cos(psi);
vec3 gy = sin(psi);

// Reorganize for dot products below
vec2 g0 = vec2(gx.x,gy.x);
vec2 g1 = vec2(gx.y,gy.y);
vec2 g2 = vec2(gx.z,gy.z);

// Radial decay with distance from each simplex corner
vec3 w = 0.8 - vec3(dot(x0, x0), dot(x1, x1), dot(x2, x2));
w = max(w, 0.0);
vec3 w2 = w * w;
vec3 w4 = w2 * w2;

// The value of the linear ramp from each of the corners
vec3 gdotx = vec3(dot(g0, x0), dot(g1, x1), dot(g2, x2));

// Multiply by the radial decay and sum up the noise value
float n = dot(w4, gdotx);

// Compute the first order partial derivatives
vec3 w3 = w2 * w;
vec3 dw = -8.0 * w3 * gdotx;
vec2 dn0 = w4.x * g0 + dw.x * x0;
vec2 dn1 = w4.y * g1 + dw.y * x1;
vec2 dn2 = w4.z * g2 + dw.z * x2;
gradient = 10.9 * (dn0 + dn1 + dn2);

// Scale the return value to fit nicely into the range [-1,1]
return 10.9 * n;
}

mat2 rotate2D(float a){
float c = cos(a), s = sin(a);
return mat2(
        c, s, // column 1
        -s, c // column 2
    );

//return vec2((x[0]*2.0f*0.866f, x[1]*2.0f*(-0.5f),0.0f) + (x[0]*2.0f*0.5f, x[1]*2.0f*(-0.866f)));
//return vec2(x[0]*2.0f*0.866f + ,);


}

float fastSpline(float x){
    float t0 = x*2.0f;
    float r0 = 0.8f + t0*(-0.45f + t0*(-0.8f + t0*0.55f));
    float t1 = (x - 0.5f)*2.0f;
    float r1 = 0.1f + t1*(-0.4f + t1*(0.55f + t1*(-0.25)));

    return x < 0.5f ? r0 : r1;

}
       
void main(void){

    const float scale = 3.0f;
    vec2 v = scale * v_TexCoord;
    v[1] = v[1] - (elapsedTime * verticalSpeed);
    const vec2 p = vec2(0.0);
    float alpha = 0.0;
    vec2 g;
    mat2 rot = rotate2D(0.52f);
    float turb = abs(psrdnoise(v, p, elapsedTime * gradRotSpeed, g));
    v = v * rot * 2.0f;
    turb += abs(0.5 * psrdnoise(v, p, elapsedTime * gradRotSpeed, g));
     v = v * rot * 2.0f;
    turb += abs(0.25 * psrdnoise(v, p, elapsedTime * gradRotSpeed, g));
    v = v * rot * 2.0f;
    turb += abs(0.125 * psrdnoise(v, p, elapsedTime * gradRotSpeed, g));



    //Modify color depending on texture coordinates, so that the fire fades out verticly and horizontally. 
    float turbScale = 0.5f + 0.9f * ( v_TexCoord[1]);


    float x = (1.0f / (sqrt( 1- v_TexCoord[1]))) * (abs(2.0f*v_TexCoord[0] - 1) + turbScale*turb);
    float edgeDensity = 12.5f * fastSpline(clamp(x, 0.0f, 1.0f));
    
    //color of the edge region of the flame
    float flameTemperature = 0.6f;
    vec3 flameColor = {1.0,flameTemperature,flameTemperature - 0.2f}; 
    vec3 edgeColor = flameColor * edgeDensity;

    //Color of interior region  of the flame
    float innerDensity = (2.85f*turb + 0.55f) + (1- v_TexCoord[1])*0.35f;
    vec3 innerColor = flameColor * innerDensity;

    //Pick either edgeColor or innerColor
    vec3 finalColor = (edgeDensity > innerDensity) ? innerColor : edgeColor;
    float density = (edgeDensity > innerDensity) ? innerDensity : edgeDensity;

    finalColor = clamp(finalColor, 0.0f,1.0f);
    density = clamp(density, 0.0f, 1.0f);

    color = vec4(finalColor,density);

    //vec3 ncolor = vec3(turb) * turbScale;
    
    ////color = vec4(1.0f,0.0f,0.0f,1.0f);
    //color = vec4(ncolor,1.0f);
};

// Authors: Stefan Gustavson (stefan.gustavson@gmail.com)
// and Ian McEwan (ijm567@gmail.com)
// Version 2021-12-02, published under the MIT license (see below)
//
// Copyright (c) 2021 Stefan Gustavson and Ian McEwan.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.