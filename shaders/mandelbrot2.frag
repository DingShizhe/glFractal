#version 130

uniform float totalOffX;
uniform float totalOffY;
uniform float ratio;

float get_ (float i, float j) {
    float a = 0, b = 0, c, d, n=0;
    while ( (c=a*a) + (d=b*b) < 4 && n++ < 880) {
        b = 2*a*b + j*8e-9 - .645411;
        a = c-d + i*8e-9 + .356888;
    }
    return (n-80) / 800;
}

void main (void) {
    float base;
    base = get_(
        (gl_FragCoord.x - 400) / ratio + 400 - totalOffX,
        (gl_FragCoord.y - 300) / ratio + 300 - totalOffY
    );
    vec3 color;
    color.x = pow(base,3.);
    color.y = pow(base,.7);
    color.z = pow(base,.5);
    // if (base != 0.0) color.z = (128.0 - base * 23.0) / 256.0;
    // else color.z = 0.0;
    gl_FragColor = vec4(color, 1.0);
}

