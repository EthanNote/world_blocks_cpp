#version 400
layout(location=0) in ivec4 block_space;
layout(location=1) in ivec3 props;
out vec3 block_pos;
out int color_index;
void main () {
  block_pos = vec3(block_space.xyz);
  color_index = props.x;
}