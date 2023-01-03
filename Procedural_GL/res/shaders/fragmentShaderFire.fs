#version 460 core
        
layout(location = 0) out vec4 color;

//in vec2 v_TexCoord;

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
       
void main(void){


    
    color = vec4(1.0f,0.0f,0.0f,1.0f);
};