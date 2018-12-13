#version 400

in vec2 uv;
out vec4 color;

uniform sampler2D tex;
void main(){
    //color = vec4(uv, 0.0, 1.0);
    color = texture(tex, uv);
}