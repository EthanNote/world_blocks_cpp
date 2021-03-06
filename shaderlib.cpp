/*
Auto generated by .\shader_lib_gen.py
*/
#include <GL/glew.h>
#include "shaderlib.h"



void CTextureShader::LocateUniforms(){
    this->LocateUniform("tex", this->tex);
}

void CTextureShader::Load(){
    CShader::Load("./shader/screen/screen.vs", "./shader/screen/texture.fs", "./shader/screen/screen.gs");
}

void CSimpleSSAOShader::LocateUniforms(){
    this->LocateUniform("view_pos_map", this->view_pos_map);
    this->LocateUniform("projection", this->projection);
    this->LocateUniform("ssao_kernel", this->ssao_kernel);
}

void CSimpleSSAOShader::Load(){
    CShader::Load("./shader/screen/screen.vs", "./shader/screen/ssao.fs", "./shader/screen/screen.gs");
}

void CMergeShader::LocateUniforms(){
    this->LocateUniform("texture_ssao", this->texture_ssao);
    this->LocateUniform("texture_color", this->texture_color);
}

void CMergeShader::Load(){
    CShader::Load("./shader/screen/screen.vs", "./shader/screen/merge.fs", "./shader/screen/screen.gs");
}

void CBlockShader::LocateUniforms(){
    this->LocateUniform("palette", this->palette);
    this->LocateUniform("MVP", this->MVP);
    this->LocateUniform("MV", this->MV);
}

void CBlockShader::Load(){
    CShader::Load("./shader/block/block.vs", "./shader/block/block.fs", "./shader/block/block.gs");
}

void CTerrineShader::LocateUniforms(){
    this->LocateUniform("MVP", this->MVP);
    this->LocateUniform("MV", this->MV);
}

void CTerrineShader::Load(){
    CShader::Load("./shader/terrine/terrine.vs", "./shader/terrine/terrine.fs", NULL);
}

void CTileShader::LocateUniforms(){
    this->LocateUniform("MVP", this->MVP);
    this->LocateUniform("MV", this->MV);
}

void CTileShader::Load(){
    CShader::Load("./shader/tile/tile.vs", "./shader/tile/tile.fs", "./shader/tile/tile.gs");
}

// global shader objects
namespace shaderlib {
    TextureShader texture_shader = nullptr;
    SimpleSSAOShader ssao_shader = nullptr;
    MergeShader merge_shader = nullptr;
    BlockShader block_shader = nullptr;
    TerrineShader terrine_shader = nullptr;
    TileShader tile_shader = nullptr;
}

void shaderlib::loadshaders(){

    texture_shader = TextureShader(new CTextureShader);
    texture_shader -> Load();
    texture_shader -> LocateUniforms();


    ssao_shader = SimpleSSAOShader(new CSimpleSSAOShader);
    ssao_shader -> Load();
    ssao_shader -> LocateUniforms();


    merge_shader = MergeShader(new CMergeShader);
    merge_shader -> Load();
    merge_shader -> LocateUniforms();


    block_shader = BlockShader(new CBlockShader);
    block_shader -> Load();
    block_shader -> LocateUniforms();


    terrine_shader = TerrineShader(new CTerrineShader);
    terrine_shader -> Load();
    terrine_shader -> LocateUniforms();


    tile_shader = TileShader(new CTileShader);
    tile_shader -> Load();
    tile_shader -> LocateUniforms();

}
