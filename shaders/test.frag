#version 130

uniform float totalOffX;
uniform float totalOffY;
uniform float ratio;


float get_(float i, float j) {
    return (sin((i * j) / 300) + 1) / 2;
}

void main (void) {
    float base = get_(
        (gl_FragCoord.x - 400) / ratio + 400 - totalOffX, 
        (gl_FragCoord.y - 300) / ratio + 300 - totalOffY
    );
    
    vec3 color = vec3(base, base, base);
    gl_FragColor = vec4(color, 1.0);
}