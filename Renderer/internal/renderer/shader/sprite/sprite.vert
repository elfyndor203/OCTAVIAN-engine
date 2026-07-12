#version 330 core

layout(location = 0) in vec2 quadXY; //    quad
layout(location = 1) in vec2 quadUV;

layout(location = 2) in vec4 texUV;
layout(location = 3) in vec4 color;
layout(location = 4) in vec2 dimensions;
layout(location = 5) in uint arrayLayer;
layout(location = 6) in vec3 transformCol0;
layout(location = 7) in vec3 transformCol1;
layout(location = 8) in vec3 transformCol2;

uniform mat3 cameraProj;

//uniform mat3 worldProj;
//uniform mat3 cameraProj;

out vec2 fragUV;
out vec4 fragColor;
out float fragArrayLayer;

void main() {
    mat3 cameraInverse = inverse(cameraProj);
    mat3 transform = mat3(transformCol0, transformCol1, transformCol2);
    vec3 position = cameraInverse * transform * vec3(quadXY * dimensions, 1.0);
    //vec3 view = cameraProj * position;
    //vec3 final = worldProj * view;

    gl_Position = vec4(position.xy, 0.0, 1.0);
    fragUV = texUV.xy + quadUV * texUV.zw;
    fragColor = color;
    fragArrayLayer = float(arrayLayer);
}