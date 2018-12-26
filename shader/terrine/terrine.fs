#version 400
varying float height;
out vec4 color;

void main(){
    color = vec4(height/255);
}