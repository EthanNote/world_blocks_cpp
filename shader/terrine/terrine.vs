#version 400
layout(location = 0) in vec3 pos;

uniform mat4 MVP;
uniform mat4 MV;

out float height;
out vec4 view_pos;
void main(){
    gl_Position = MVP * vec4(pos, 1.0);
    view_pos = MV * vec4(pos, 1.0);
    height = pos.y;
}

