#version 400
layout(points) in;
layout(triangle_strip, max_vertices = 12) out;

in ivec4 tile_space[];
in ivec2 tile_grad[];

out float height;
out vec4 view_pos;
// flat out vec3 world_normal;
// flat out int color_palette_index;

uniform mat4 MVP;
uniform mat4 MV;

vec4 v[8];

void face_vertex(int a, int b, int c, int d){
    gl_Position  = MVP * v[a];
        view_pos = MV  * v[a];
          height = v[a].y;
    EmitVertex();
    gl_Position  = MVP * v[b];
        view_pos = MV  * v[b];
          height = v[b].y;
    EmitVertex();
    gl_Position  = MVP * v[c];
        view_pos = MV  * v[c];
          height = v[c].y;
    EmitVertex();
    gl_Position  = MVP * v[d];
        view_pos = MV  * v[d];
          height = v[d].y;
    EmitVertex();
    EndPrimitive();
}

void main(){
    vec4 pos = vec4(tile_space[0].xyz, 1.0);
    float size = tile_space[0].w;
    if(size<=0){
        EndPrimitive();
        return;
    }
    float g_x = tile_grad[0].x;
    float g_z = tile_grad[0].y;
  
    v[0] = pos;
    v[1] = pos + vec4(0.0,  0.0, size, 0.0);
    v[2] = pos + vec4(size, 0.0, 0.0,  0.0);
    v[3] = pos + vec4(size, 0.0, size, 0.0);
    v[4] = pos + vec4(0.0,  g_z, size, 0.0);
    v[5] = pos + vec4(size, g_z, size, 0.0);
    v[6] = pos + vec4(size, g_x, size, 0.0);
    v[7] = pos + vec4(size, g_x, 0.0,  0.0);

    
    face_vertex(0,1,2,3);
    face_vertex(1,4,3,5);
    face_vertex(2,3,7,6);
    // if(g_z>0){
    //     face_vertex(1,3,4,5);
    // }
    // else if(g_z<0){
    //     face_vertex(1,4,3,5);
    // }

    // if(g_x>0){
    //     face_vertex(2,7,3,6);
    // }
    // else if(g_x<0){
    //     face_vertex(2,3,7,6);
    // }
}