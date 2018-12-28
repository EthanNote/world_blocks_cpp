in float height;

in vec4 view_pos;

layout(location=0) out vec4 color_out;
layout(location=2) out vec4 position_out;

void main(){
    color_out = vec4((height+50)/50, 1.0,0.0,1.0);
    position_out = view_pos;
}