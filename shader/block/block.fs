#version 400
flat in vec4 color;
layout(location=0) out vec4 color_out;
layout(location=1) out vec4 normal_out;
layout(location=2) out vec4 position_out;

void main () {
    //gl_FragColor=color;
    color_out = color;
    normal_out = color;
    position_out = color;
};