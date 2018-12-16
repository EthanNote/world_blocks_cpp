/*
Auto generated by c:\Users\snipe_000\source\repos\world_blocks\world_blocks++\shader_lib_gen.py
*/
#include "shader.h"



class CAutoScreenShader : public CShader{
public:
    UniformSampler2D tex;
    UniformSampler2DArray textures;
    UniformMatrix projection;
    UniformVec3Array ssao_kernel;
    void Load();
    void LocateUniforms();
};

typedef std::shared_ptr<CAutoScreenShader> AutoScreenShader;

class CAutoBlockShader : public CShader{
public:
    UniformMatrix MVP;
    UniformMatrix MV;
    void Load();
    void LocateUniforms();
};

typedef std::shared_ptr<CAutoBlockShader> AutoBlockShader;

namespace shaderlib{
    // generated shader objects
    extern AutoScreenShader screen_shader;
    extern AutoBlockShader block_shader;

    void loadshaders();
}
