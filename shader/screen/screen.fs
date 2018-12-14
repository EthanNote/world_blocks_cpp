#version 400

in vec2 uv;
out vec4 color;

uniform sampler2D tex;

// layout(location=0) out vec4 color_out;
// layout(location=1) out vec4 normal_out;
// layout(location=2) out vec4 position_out;
uniform sampler2D textures[4];
uniform mat4 projection;

float ssao(){
    //sampler2D view_pos_tex = textures[2];

    vec3 view_pos = texture(textures[2], uv).xyz;
    float depth = view_pos.z;

    int ao=0;
    int ao_max=0;

    for(float x=-1.0;x<=1.0;x+=0.2){
    for(float y=-1.0;y<=1.0;y+=0.2){
    for(float z=-1.0;z<=1.0;z+=0.2){
        vec3 sample_pos = view_pos + vec3(x, y, z);
        vec4 sample_uv = projection * vec4(sample_pos, 1.0);
        sample_uv.xy /= sample_uv.w;
        sample_uv.xy = sample_uv.xy * 0.5 + vec2(0.5);
        float sample_depth = texture(textures[2], sample_uv.xy).z;
        if(depth>sample_depth){
            ao++;
        }
        ao_max++;

    }}}

    return float(ao)/float(ao_max*2);

}

void main(){
    //depth
    //color = vec4(-texture(textures[2], uv).z);

    color = vec4(ssao());

    //color
    //color = texture(textures[0], uv);
}