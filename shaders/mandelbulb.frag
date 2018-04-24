#version 130

uniform float totalOffX;
uniform float totalOffY;
uniform float ratio;

vec3 map(vec3 q) {
    mat3 rotxz = mat3(
        cos(totalOffX / 200.0), 0.0, sin(totalOffX / 200.0), 
        0.0, 1.0, 0.0,
        -sin(totalOffX / 200.0), 0.0, cos(totalOffX / 200.0)
    );
    vec3 p = rotxz * q;

    vec3 z = p, dz = vec3(0.0, 0.0, 0.0);
    float power = 8.0, r, tta, phi, dr = 1.0, t0 = 1.0;
    for (int i = 0; i < 4; ++i) {
        if ((r = sqrt(z.x * z.x + z.y * z.y + z.z * z.z)) > 2.0) break;
        tta = acos(z.y / r) * power;
        phi = atan(z.z / z.x) * power;
        // spherical
        dr = pow(r, power - 1.0) * dr * power + 1.0;
        // derevative
        r = pow(r, power);
        z = vec3(sin(tta) * cos(phi), cos(tta), sin(tta) * sin(phi)) * r + p;
        t0 = t0 < r ? t0 : r;
        // orbit trap, to mimic ao
    }
    return vec3(0.5 * log(r) * r / dr, t0, 0.0);
    // distance estimation
}


vec3 trace(float x, float y) {
    // ray marcher
    vec3 uv = vec3((x * 2.0 / 800.0 - 1.0) * 800.0 / 600.0, y * 2.0 / 600.0 - 1.0, 0.0);
    vec3 ro = vec3(0.0, 2.0, 2.2), up = vec3(0.0, 1.0, 0.0);
    vec3 cf = normalize(ro * -1.0), cs = normalize(cross(cf, up));
    vec3 rd = normalize(cs * uv.x + normalize(cross(cs, cf)) * uv.y + cf * 2.8);
    vec3 p, col; float t = 0.0;
    while (t < 20.0) {
        p = ro + rd * t;
        vec3 akuma = map(p);
        if (akuma.x < 0.0001) return vec3(akuma.y, akuma.y, akuma.y);
        t += akuma.x;
    }
    return vec3(1.0, 1.0, 1.0);
}


void main (void) {
    vec3 color = trace(
        (gl_FragCoord.x - 400) / ratio + 400, 
        (gl_FragCoord.y - 300) / ratio + 300
    );
    gl_FragColor = vec4(color, 1.0);
}