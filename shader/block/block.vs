#version 400
layout(location=0) in ivec4 block_space;
out vec3 block_pos;

void main () {
  block_pos = vec3(block_space.xyz);
}