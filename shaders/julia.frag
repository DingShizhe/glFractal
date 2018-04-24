#version 330 core

uniform float totalOffX;
uniform float totalOffY;
uniform float ratio;


float get_(float i, float j) {
    vec2 z;
    z.x = i / 512;
    z.y = j / 512;

    int ii;
    int iter = 200;
    vec2 c = vec2(-0.74543, 0.11301);
    for (ii = 0; ii < iter; ii ++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 7.0) break;
        z.x = x;
        z.y = y;
    }

    float base = ii <= 2 ? 0.0 : log(float(ii)) / log(iter);
    base = pow(base, 3);
    return base;
}

void main() {
    float base = get_(
        (gl_FragCoord.x - 400) / ratio + 400 - totalOffX,
        (gl_FragCoord.y - 300) / ratio + 300 - totalOffY
    );
    gl_FragColor = vec4(base, base, base, 0.5);
}