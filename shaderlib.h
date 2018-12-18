/*
Auto generated by c:\Users\snipe_000\source\repos\world_blocks\world_blocks++\shader_lib_gen.py
*/
#include "shader.h"



class CTextureShader : public CShader{
public:
    UniformSampler2D tex;
    void Load();
    void LocateUniforms();
};

typedef std::shared_ptr<CTextureShader> TextureShader;

class CSimpleSSAOShader : public CShader{
public:
    UniformSampler2D view_pos_map;
    UniformMatrix projection;
    UniformVec3Array ssao_kernel;
    void Load();
    void LocateUniforms();
};

typedef std::shared_ptr<CSimpleSSAOShader> SimpleSSAOShader;

class CMergeShader : public CShader{
public:
    UniformSampler2D texture_ssao;
    UniformSampler2D texture_color;
    void Load();
    void LocateUniforms();
};

typedef std::shared_ptr<CMergeShader> MergeShader;

class CBlockShader : public CShader{
public:
    UniformMatrix MVP;
    UniformMatrix MV;
    void Load();
    void LocateUniforms();
};

typedef std::shared_ptr<CBlockShader> BlockShader;

namespace shaderlib{
    // generated shader objects
    extern TextureShader texture_shader;
    extern SimpleSSAOShader ssao_shader;
    extern MergeShader merge_shader;
    extern BlockShader block_shader;

    void loadshaders();
}
