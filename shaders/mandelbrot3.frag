#version 130

uniform float totalOffX;
uniform float totalOffY;
uniform float ratio;


float get_ (float i, float j) {
    float a=0, b=0, d,n=0;
    for( ;a*a+(d=b*b) < 10 && n++<2000; b=2*a*b+j/1e3+.06, a=a*a-d+i/1e3+.34);
    return n / 4;
}


void main (void) {
    float base;
    base = get_(
        (gl_FragCoord.x - 400) / ratio + 400 - totalOffX,
        (gl_FragCoord.y - 300) / ratio + 300 - totalOffY
    );
    vec3 color;
    color.x = base / 256.;
    color.y = 2 * base / 256.;
    color.z = 4 * base / 256.;
    gl_FragColor = vec4(color, 1.0);
}

