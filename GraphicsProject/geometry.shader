#version 330 core

layout (triangles) in;
layout (points, max_vertices = 3) out;

in vec4 color[];
in vec3 normals[];

uniform float time;

out vec4 colorToFrag;


void main(){
    float u = 1.0;
    float a = 1.0;

    vec3 surf = vec3(0, 1, 0);

    if (normals[0].y < 0) reflect(normals[0], surf);
    if (normals[1].y < 0) reflect(normals[1], surf);
    if (normals[2].y < 0) reflect(normals[2], surf);

    vec4 k0 = vec4( ( normals[0] * ((u * time) + (0.5f * a * (time*time))) ), 1.0);
    vec4 k1 = vec4( ( normals[1] * ((u * time) + (0.5f * a * (time*time))) ), 1.0);
    vec4 k2 = vec4( ( normals[2] * ((u * time) + (0.5f * a * (time*time))) ), 1.0);

    vec4 vertexPos = gl_in[0].gl_Position + k0;
    gl_Position = vertexPos;
    colorToFrag = color[0] ;
    EmitVertex();
    EndPrimitive();

    vertexPos = gl_in[1].gl_Position + k1;
    gl_Position = vertexPos;
    colorToFrag = color[0] ;
    EmitVertex();
    EndPrimitive();

    vertexPos = gl_in[2].gl_Position + k2;
    gl_Position = vertexPos;
    colorToFrag = color[0] ;
    EmitVertex();
    EndPrimitive();
}