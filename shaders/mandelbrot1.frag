// mandelbrot
// z = (x, y), c = (i, j)
// z = z ^ 2 + c
// (x, y) =>  (x^2-y^2+i, 2xy+j)

#version 330 core

// in vec4 gl_FragCoord;
// out vec4 gl_FragColor;

uniform float totalOffX;
uniform float totalOffY;
uniform float ratio;

float get_(float i, float j) {
    float x = 0.0, y = 0.0;
    float ret = 0.0;
    for (int k = 0; k < 300; k++) {
        float a = x*x - y*y + (i - 800.0) / 512;
        y = 2 * x * y + (j - 300.0) / 512;
        x = a;
        ret = ret + 1.0;
        if (x*x + y*y > 10)
            break;
    }
    if (ret >= 254.0)
        return 0.0;
    else
        return log(ret);
}


void main (void) {
    float base;
    base = get_(
        (gl_FragCoord.x - 400) / ratio + 400 - totalOffX,
        (gl_FragCoord.y - 300) / ratio + 300 - totalOffY
    );
    vec3 color;
    color.x = base * 47.0 / 256.0;
    color.y = base * 47.0 / 256.0;
    if (base != 0.0) color.z = (128.0 - base * 23.0) / 256.0;
    else color.z = 0.0;
    gl_FragColor = vec4(color, 1.0);
}

