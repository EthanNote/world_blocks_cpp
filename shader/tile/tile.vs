#version 400
layout(location=0) in ivec4 space;
layout(location=1) in ivec2 grad;

out ivec4 tile_space;
out ivec2 tile_grad;
void main(){
    tile_space = space;
    tile_grad = grad;
}