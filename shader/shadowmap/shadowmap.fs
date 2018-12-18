#version 400
flat in vec4 color;
in vec4 view_pos;
flat in vec3 world_normal;

out vec4 depth;

void main () {
    depth = vec4(view_pos.z);
};