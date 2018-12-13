#version 400
layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

in vec3 block_pos[];

flat out vec4 color;
uniform mat4 MVP;
void main () {
  vec4 pos = vec4(block_pos[0],1.0);

  // z-
  color = vec4(0.5,0.5,0.0,1.0);
  gl_Position = MVP*(pos + vec4(0.0,0.0,0.0,0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0,0.0,0.0,0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(0.0,1.0,0.0,0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0,1.0,0.0,0.0));
  EmitVertex();
  EndPrimitive();

  // z+
  gl_Position = MVP*(pos + vec4(0.0,0.0,1.0,0.0));
  color = vec4(0.0, 0.0, 1.0, 1.0);
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0,0.0,1.0,0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(0.0,1.0,1.0,0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0,1.0,1.0,0.0));
  EmitVertex();
  EndPrimitive();

  // x-
  color = vec4(0.0, 0.5, 0.5, 1.0);
  gl_Position = MVP*(pos + vec4(0.0, 0.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(0.0, 0.0, 1.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(0.0, 1.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(0.0, 1.0, 1.0, 0.0));
  EmitVertex();
  EndPrimitive();

  // x+
  color = vec4(1.0, 0.0, 0.0, 1.0);
  gl_Position = MVP*(pos + vec4(1.0, 0.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 0.0, 1.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 1.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 1.0, 1.0, 0.0));
  EmitVertex();
  EndPrimitive();

  // y-
  color = vec4(0.5, 0.0, 0.5, 1.0);
  gl_Position = MVP*(pos + vec4(0.0, 0.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 0.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(0.0, 0.0, 1.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 0.0, 1.0, 0.0));
  EmitVertex();
  EndPrimitive();

  // y+
  color = vec4(0.0, 1.0, 0.0, 1.0);
  gl_Position = MVP*(pos + vec4(0.0, 1.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 1.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(0.0, 1.0, 1.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 1.0, 1.0, 0.0));
  EmitVertex();
  EndPrimitive();
  
}