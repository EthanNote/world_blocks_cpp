#version 400

uniform sampler2D texture_ssao;
uniform sampler2D texture_color;

in vec2 uv;
out vec4 color;

void main(){
    color = vec4(texture(texture_color, uv).rgb * texture(texture_ssao, uv).r, 1.0);    
}