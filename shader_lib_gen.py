import re
import json
# class Uniform:
#     def __init__(self, var_name, var_type):
#         self.var_name = var_name
#         self.var_type = var_type

# class ArrayUniform(Uniform):
#     def __init__(self, var_name, var_type, arr_size):
#         Uniform(self, var_name, var_type)
#         self.arr_size = arr_size

def scan_uniforms(file):
    if file is None:
        return []
    f = open(file, 'r')
    lines = f.readlines()
    # print(f, lines)
    results = []
    for l in lines:
        # print(l)
        if re.match('^uniform[\s]+[^\s]+[\s]+([a-z]|[A-Z]|_)([a-z]|[A-Z]|[0-9]|_)*;$', l):
            sp = l.split()
            var_type = sp[1]
            var_name = sp[2].replace(';','')
            results.append({"var_name":var_name, "var_type":var_type, "array":False})

        elif re.match('^uniform[\s]+[^\s]+[\s]+([a-z]|[A-Z]|_)([a-z]|[A-Z]|[0-9]|_)*\[(.*)\];$', l):
            sp = l.split()
            var_type = sp[1]
            var_name = sp[2].replace(' ','').replace('[', ' ').split()[0]
            arr_size = sp[2].replace(' ','').replace('[', ' ').split()[1].replace(']',' ').split()[0]
            results.append({"var_name":var_name, "var_type":var_type, "array":{"size":arr_size}})
    f.close()
    return results

    

decl_header="""/*
Auto generated by %s
*/
#include "shader.h"
"""%(__file__)

impl_header="""/*
Auto generated by %s
*/
#include <GL/glew.h>
#include "shaderlib.h"
"""%(__file__)

#(class_name, [uniform_declare_line])
cls_decl = """
class C%s : public CShader{
public:
%s
    void Load();
    void LocateUniforms();
};

typedef std::shared_ptr<C%s> %s;
"""
namespace_decl = """
namespace shaderlib{
    // generated shader objects
%s

    void loadshaders();
}
"""

namespace_impl = """
// global shader objects
namespace shaderlib {
%s
}
"""

extern_entry = """    extern %s %s;"""
extern_impl = """    %s %s = nullptr;"""

load_impl = """
void shaderlib::loadshaders(){
%s
}
"""

extern_assign = """
    %s = %s(new C%s);
    %s -> Load();
    %s -> LocateUniforms();
"""

#(uniform_class, uniform_cpp_name)
uniform_declare_line = """    %s %s;"""

#(class_name, [uniform_locating_line], class_name, vs_fname, fs_fname, gs_fname)
cls_impl = """
void C%s::LocateUniforms(){
%s
}

void C%s::Load(){
    CShader::Load(%s, %s, %s);
}
"""

#(uniform_glsl_name, uniform_cpp_name)
uniform_locating_line ="""    this->LocateUniform("%s", this->%s);"""

single_type_map = {
    'int':'UniformInt',
    'float':'UniformFloat',
    'vec3':'UniformVec3',
    'vec4':'UniformVec4',
    'mat4':'UniformMatrix',
    'sampler2D':'UniformSampler2D',
}

array_type_map = {
    'int':'UniformIntArray',
    'float':'UniformFloatArray',
    'vec3':'UniformVec3Array',
    'vec4':'UniformVec4Array',
    'mat4':'UniformMat4Array',
    'sampler2D':'UniformSampler2DArray',
}


# shaders = [
#     {"name":"TestShader", "identifier":"test_shader","source":[None, './screen/screen.fs', None]}
# ]

def fname_str(fname):
    if fname is None:
        return 'NULL'
    return '"%s"'%(fname,)

def type_str(basetype, isarray):
    if not isarray:
        return single_type_map[basetype]
    else:
        return array_type_map[basetype]

def gen(shaders):
    decl_gen = decl_header+"\n\n"
    impl_gen = impl_header+"\n\n"
    for s in shaders:
        r = scan_uniforms(s["source"][0])+scan_uniforms(s["source"][1])+scan_uniforms(s["source"][2])
        delcare_lines = [uniform_declare_line%(single_type_map[u["var_type"]] if not u["array"] else array_type_map[u["var_type"]] ,u["var_name"]) for u in r]
        locating_lines = [uniform_locating_line%(u["var_name"],u["var_name"]) for u in r]

        decl_gen += cls_decl%(s["name"], '\n'.join(delcare_lines), s["name"], s["name"])
        impl_gen += cls_impl%(s["name"], '\n'.join(locating_lines), s["name"], fname_str(s["source"][0]), fname_str(s["source"][1]), fname_str(s["source"][2]))
    
    externs = [extern_entry%(s["name"], s["identifier"]) for s in shaders]
    extern_impl_list = [extern_impl%(s["name"], s["identifier"]) for s in shaders]

    namespace_decl_gen = namespace_decl%('\n'.join(externs), )
    namespace_impl_gen = namespace_impl%('\n'.join(extern_impl_list), )
    decl_gen += namespace_decl_gen
    impl_gen += namespace_impl_gen
    # print("E" namespace_impl_gen)
    externs_assign_list = [extern_assign%(u["identifier"], u["name"], u["name"],u["identifier"],u["identifier"]) for u in shaders]
    load_impl_gen = load_impl%('\n'.join(externs_assign_list), )
    impl_gen +=load_impl_gen
    with open("shaderlib.h","w") as f:
        f.write(decl_gen)
    with open("shaderlib.cpp","w") as f:
        f.write(impl_gen)
    print(decl_gen)

    print(impl_gen)



gen(json.load(open("shader_lib.json")))