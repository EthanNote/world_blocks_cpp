#version 400
layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

in vec3 block_pos[];
in int color_index[];

flat out vec4 color;
out vec4 view_pos;
flat out vec3 world_normal;
flat out int color_palette_index;

uniform mat4 MVP;
uniform mat4 MV;

void main () {
  vec4 pos = vec4(block_pos[0],1.0);

  // z-
  color = vec4(0.5,0.5,0.0,1.0);
  world_normal = vec3(0.0, 0.0, -1.0);
  color_palette_index=color_index[0];
  gl_Position = MVP*(pos + vec4(0.0, 0.0, 0.0, 0.0));
      view_pos = MV*(pos + vec4(0.0, 0.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 0.0, 0.0, 0.0));
      view_pos = MV*(pos + vec4(1.0, 0.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(0.0, 1.0, 0.0, 0.0));
      view_pos = MV*(pos + vec4(0.0, 1.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 1.0, 0.0, 0.0));
      view_pos = MV*(pos + vec4(1.0, 1.0, 0.0, 0.0));
  EmitVertex();
  EndPrimitive();

  // z+
  color = vec4(0.0, 0.0, 1.0, 1.0);
  world_normal = vec3(0.0, 0.0, 1.0);
  color_palette_index=color_index[0];
  gl_Position = MVP*(pos + vec4(0.0, 0.0, 1.0, 0.0));
      view_pos = MV*(pos + vec4(0.0, 0.0, 1.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 0.0, 1.0, 0.0));
      view_pos = MV*(pos + vec4(1.0, 0.0, 1.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(0.0, 1.0, 1.0, 0.0));
      view_pos = MV*(pos + vec4(0.0, 1.0, 1.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 1.0, 1.0, 0.0));
      view_pos = MV*(pos + vec4(1.0, 1.0, 1.0, 0.0));
  EmitVertex();
  EndPrimitive();

  // x-
  color = vec4(0.0, 0.5, 0.5, 1.0);
  world_normal = vec3(-1.0, 0.0, 0.0);
  color_palette_index=color_index[0];
  gl_Position = MVP*(pos + vec4(0.0, 0.0, 0.0, 0.0));
      view_pos = MV*(pos + vec4(0.0, 0.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(0.0, 0.0, 1.0, 0.0));
      view_pos = MV*(pos + vec4(0.0, 0.0, 1.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(0.0, 1.0, 0.0, 0.0));
      view_pos = MV*(pos + vec4(0.0, 1.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(0.0, 1.0, 1.0, 0.0));
      view_pos = MV*(pos + vec4(0.0, 1.0, 1.0, 0.0));
  EmitVertex();
  EndPrimitive();

  // x+
  color = vec4(1.0, 0.0, 0.0, 1.0);
  world_normal = vec3(1.0, 0.0, 0.0);
  color_palette_index=color_index[0];
  gl_Position = MVP*(pos + vec4(1.0, 0.0, 0.0, 0.0));
      view_pos = MV*(pos + vec4(1.0, 0.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 0.0, 1.0, 0.0));
      view_pos = MV*(pos + vec4(1.0, 0.0, 1.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 1.0, 0.0, 0.0));
      view_pos = MV*(pos + vec4(1.0, 1.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 1.0, 1.0, 0.0));
      view_pos = MV*(pos + vec4(1.0, 1.0, 1.0, 0.0));
  EmitVertex();
  EndPrimitive();

  // y-
  color = vec4(0.5, 0.0, 0.5, 1.0);
  world_normal = vec3(0.0, -1.0, 0.0);
  color_palette_index=color_index[0];
  gl_Position = MVP*(pos + vec4(0.0, 0.0, 0.0, 0.0));
      view_pos = MV*(pos + vec4(0.0, 0.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 0.0, 0.0, 0.0));
      view_pos = MV*(pos + vec4(1.0, 0.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(0.0, 0.0, 1.0, 0.0));
      view_pos = MV*(pos + vec4(0.0, 0.0, 1.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 0.0, 1.0, 0.0));
      view_pos = MV*(pos + vec4(1.0, 0.0, 1.0, 0.0));
  EmitVertex();
  EndPrimitive();

  // y+
  color = vec4(0.0, 1.0, 0.0, 1.0);
  world_normal = vec3(0.0, 1.0, 0.0);
  color_palette_index=color_index[0];
  gl_Position = MVP*(pos + vec4(0.0, 1.0, 0.0, 0.0));
      view_pos = MV*(pos + vec4(0.0, 1.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 1.0, 0.0, 0.0));
      view_pos = MV*(pos + vec4(1.0, 1.0, 0.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(0.0, 1.0, 1.0, 0.0));
      view_pos = MV*(pos + vec4(0.0, 1.0, 1.0, 0.0));
  EmitVertex();
  gl_Position = MVP*(pos + vec4(1.0, 1.0, 1.0, 0.0));
      view_pos = MV*(pos + vec4(1.0, 1.0, 1.0, 0.0));
  EmitVertex();
  EndPrimitive();
  
}