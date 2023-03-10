#pragma once
/*
    #version:1# (machine generated, don't edit!)

    Generated by sokol-shdc (https://github.com/floooh/sokol-tools)

    Cmdline: sokol-shdc --input asset/mesh_shader.glsl --output asset/macosx/mesh_shader.h --slang metal_macos --format sokol_impl --reflection

    Overview:

        Shader program 'mesh':
            Get shader desc: mesh_shader_desc(sg_query_backend());
            Vertex shader: mesh_vs
                Attribute slots:
                    ATTR_mesh_vs_xyuv = 0
                    ATTR_mesh_vs_rgba = 1
                Uniform block 'mesh_vs_proj':
                    C struct: mesh_vs_proj_t
                    Bind slot: SLOT_mesh_vs_proj = 0
                Uniform block 'mesh_vs_model':
                    C struct: mesh_vs_model_t
                    Bind slot: SLOT_mesh_vs_model = 1
            Fragment shader: mesh_fs
                Image 'tex':
                    Type: SG_IMAGETYPE_2D
                    Component Type: SG_SAMPLERTYPE_FLOAT
                    Bind slot: SLOT_tex = 0


    Shader descriptor structs:

        sg_shader mesh = sg_make_shader(mesh_shader_desc(sg_query_backend()));

    Vertex attribute locations for vertex shader 'mesh_vs':

        sg_pipeline pip = sg_make_pipeline(&(sg_pipeline_desc){
            .layout = {
                .attrs = {
                    [ATTR_mesh_vs_xyuv] = { ... },
                    [ATTR_mesh_vs_rgba] = { ... },
                },
            },
            ...});

    Image bind slots, use as index in sg_bindings.vs_images[] or .fs_images[]

        SLOT_tex = 0;

    Bind slot and C-struct for uniform block 'mesh_vs_proj':

        mesh_vs_proj_t mesh_vs_proj = {
            .proj = ...;
        };
        sg_apply_uniforms(SG_SHADERSTAGE_[VS|FS], SLOT_mesh_vs_proj, &SG_RANGE(mesh_vs_proj));

    Bind slot and C-struct for uniform block 'mesh_vs_model':

        mesh_vs_model_t mesh_vs_model = {
            .model = ...;
        };
        sg_apply_uniforms(SG_SHADERSTAGE_[VS|FS], SLOT_mesh_vs_model, &SG_RANGE(mesh_vs_model));

*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#if !defined(SOKOL_GFX_INCLUDED)
  #error "Please include sokol_gfx.h before mesh_shader.h"
#endif
#if !defined(SOKOL_SHDC_ALIGN)
  #if defined(_MSC_VER)
    #define SOKOL_SHDC_ALIGN(a) __declspec(align(a))
  #else
    #define SOKOL_SHDC_ALIGN(a) __attribute__((aligned(a)))
  #endif
#endif
const sg_shader_desc* mesh_shader_desc(sg_backend backend);
int mesh_attr_slot(const char* attr_name);
int mesh_image_slot(sg_shader_stage stage, const char* img_name);
int mesh_uniformblock_slot(sg_shader_stage stage, const char* ub_name);
size_t mesh_uniformblock_size(sg_shader_stage stage, const char* ub_name);
int mesh_uniform_offset(sg_shader_stage stage, const char* ub_name, const char* u_name);
sg_shader_uniform_desc mesh_uniform_desc(sg_shader_stage stage, const char* ub_name, const char* u_name);
#define ATTR_mesh_vs_xyuv (0)
#define ATTR_mesh_vs_rgba (1)
#define SLOT_tex (0)
#define SLOT_mesh_vs_proj (0)
#pragma pack(push,1)
SOKOL_SHDC_ALIGN(16) typedef struct mesh_vs_proj_t {
    ::ant2d::math::Mat4 proj;
} mesh_vs_proj_t;
#pragma pack(pop)
#define SLOT_mesh_vs_model (1)
#pragma pack(push,1)
SOKOL_SHDC_ALIGN(16) typedef struct mesh_vs_model_t {
    ::ant2d::math::Mat4 model;
} mesh_vs_model_t;
#pragma pack(pop)
#if defined(SOKOL_SHDC_IMPL)
/*
    #include <metal_stdlib>
    #include <simd/simd.h>
    
    using namespace metal;
    
    struct mesh_vs_proj
    {
        float4x4 proj;
    };
    
    struct mesh_vs_model
    {
        float4x4 model;
    };
    
    struct main0_out
    {
        float4 outColor [[user(locn0)]];
        float2 outTexCoord [[user(locn1)]];
        float4 gl_Position [[position]];
    };
    
    struct main0_in
    {
        float4 xyuv [[attribute(0)]];
        float4 rgba [[attribute(1)]];
    };
    
    #line 21 "asset/mesh_shader.glsl"
    vertex main0_out main0(main0_in in [[stage_in]], constant mesh_vs_proj& _32 [[buffer(0)]], constant mesh_vs_model& _38 [[buffer(1)]])
    {
        main0_out out = {};
    #line 21 "asset/mesh_shader.glsl"
        out.outColor = in.rgba;
    #line 22 "asset/mesh_shader.glsl"
        out.outTexCoord = in.xyuv.zw;
    #line 24 "asset/mesh_shader.glsl"
        out.gl_Position = (_32.proj * _38.model) * float4(in.xyuv.xy, 0.0, 1.0);
        return out;
    }
    
*/
static const char mesh_vs_source_metal_macos[846] = {
    0x23,0x69,0x6e,0x63,0x6c,0x75,0x64,0x65,0x20,0x3c,0x6d,0x65,0x74,0x61,0x6c,0x5f,
    0x73,0x74,0x64,0x6c,0x69,0x62,0x3e,0x0a,0x23,0x69,0x6e,0x63,0x6c,0x75,0x64,0x65,
    0x20,0x3c,0x73,0x69,0x6d,0x64,0x2f,0x73,0x69,0x6d,0x64,0x2e,0x68,0x3e,0x0a,0x0a,
    0x75,0x73,0x69,0x6e,0x67,0x20,0x6e,0x61,0x6d,0x65,0x73,0x70,0x61,0x63,0x65,0x20,
    0x6d,0x65,0x74,0x61,0x6c,0x3b,0x0a,0x0a,0x73,0x74,0x72,0x75,0x63,0x74,0x20,0x6d,
    0x65,0x73,0x68,0x5f,0x76,0x73,0x5f,0x70,0x72,0x6f,0x6a,0x0a,0x7b,0x0a,0x20,0x20,
    0x20,0x20,0x66,0x6c,0x6f,0x61,0x74,0x34,0x78,0x34,0x20,0x70,0x72,0x6f,0x6a,0x3b,
    0x0a,0x7d,0x3b,0x0a,0x0a,0x73,0x74,0x72,0x75,0x63,0x74,0x20,0x6d,0x65,0x73,0x68,
    0x5f,0x76,0x73,0x5f,0x6d,0x6f,0x64,0x65,0x6c,0x0a,0x7b,0x0a,0x20,0x20,0x20,0x20,
    0x66,0x6c,0x6f,0x61,0x74,0x34,0x78,0x34,0x20,0x6d,0x6f,0x64,0x65,0x6c,0x3b,0x0a,
    0x7d,0x3b,0x0a,0x0a,0x73,0x74,0x72,0x75,0x63,0x74,0x20,0x6d,0x61,0x69,0x6e,0x30,
    0x5f,0x6f,0x75,0x74,0x0a,0x7b,0x0a,0x20,0x20,0x20,0x20,0x66,0x6c,0x6f,0x61,0x74,
    0x34,0x20,0x6f,0x75,0x74,0x43,0x6f,0x6c,0x6f,0x72,0x20,0x5b,0x5b,0x75,0x73,0x65,
    0x72,0x28,0x6c,0x6f,0x63,0x6e,0x30,0x29,0x5d,0x5d,0x3b,0x0a,0x20,0x20,0x20,0x20,
    0x66,0x6c,0x6f,0x61,0x74,0x32,0x20,0x6f,0x75,0x74,0x54,0x65,0x78,0x43,0x6f,0x6f,
    0x72,0x64,0x20,0x5b,0x5b,0x75,0x73,0x65,0x72,0x28,0x6c,0x6f,0x63,0x6e,0x31,0x29,
    0x5d,0x5d,0x3b,0x0a,0x20,0x20,0x20,0x20,0x66,0x6c,0x6f,0x61,0x74,0x34,0x20,0x67,
    0x6c,0x5f,0x50,0x6f,0x73,0x69,0x74,0x69,0x6f,0x6e,0x20,0x5b,0x5b,0x70,0x6f,0x73,
    0x69,0x74,0x69,0x6f,0x6e,0x5d,0x5d,0x3b,0x0a,0x7d,0x3b,0x0a,0x0a,0x73,0x74,0x72,
    0x75,0x63,0x74,0x20,0x6d,0x61,0x69,0x6e,0x30,0x5f,0x69,0x6e,0x0a,0x7b,0x0a,0x20,
    0x20,0x20,0x20,0x66,0x6c,0x6f,0x61,0x74,0x34,0x20,0x78,0x79,0x75,0x76,0x20,0x5b,
    0x5b,0x61,0x74,0x74,0x72,0x69,0x62,0x75,0x74,0x65,0x28,0x30,0x29,0x5d,0x5d,0x3b,
    0x0a,0x20,0x20,0x20,0x20,0x66,0x6c,0x6f,0x61,0x74,0x34,0x20,0x72,0x67,0x62,0x61,
    0x20,0x5b,0x5b,0x61,0x74,0x74,0x72,0x69,0x62,0x75,0x74,0x65,0x28,0x31,0x29,0x5d,
    0x5d,0x3b,0x0a,0x7d,0x3b,0x0a,0x0a,0x23,0x6c,0x69,0x6e,0x65,0x20,0x32,0x31,0x20,
    0x22,0x61,0x73,0x73,0x65,0x74,0x2f,0x6d,0x65,0x73,0x68,0x5f,0x73,0x68,0x61,0x64,
    0x65,0x72,0x2e,0x67,0x6c,0x73,0x6c,0x22,0x0a,0x76,0x65,0x72,0x74,0x65,0x78,0x20,
    0x6d,0x61,0x69,0x6e,0x30,0x5f,0x6f,0x75,0x74,0x20,0x6d,0x61,0x69,0x6e,0x30,0x28,
    0x6d,0x61,0x69,0x6e,0x30,0x5f,0x69,0x6e,0x20,0x69,0x6e,0x20,0x5b,0x5b,0x73,0x74,
    0x61,0x67,0x65,0x5f,0x69,0x6e,0x5d,0x5d,0x2c,0x20,0x63,0x6f,0x6e,0x73,0x74,0x61,
    0x6e,0x74,0x20,0x6d,0x65,0x73,0x68,0x5f,0x76,0x73,0x5f,0x70,0x72,0x6f,0x6a,0x26,
    0x20,0x5f,0x33,0x32,0x20,0x5b,0x5b,0x62,0x75,0x66,0x66,0x65,0x72,0x28,0x30,0x29,
    0x5d,0x5d,0x2c,0x20,0x63,0x6f,0x6e,0x73,0x74,0x61,0x6e,0x74,0x20,0x6d,0x65,0x73,
    0x68,0x5f,0x76,0x73,0x5f,0x6d,0x6f,0x64,0x65,0x6c,0x26,0x20,0x5f,0x33,0x38,0x20,
    0x5b,0x5b,0x62,0x75,0x66,0x66,0x65,0x72,0x28,0x31,0x29,0x5d,0x5d,0x29,0x0a,0x7b,
    0x0a,0x20,0x20,0x20,0x20,0x6d,0x61,0x69,0x6e,0x30,0x5f,0x6f,0x75,0x74,0x20,0x6f,
    0x75,0x74,0x20,0x3d,0x20,0x7b,0x7d,0x3b,0x0a,0x23,0x6c,0x69,0x6e,0x65,0x20,0x32,
    0x31,0x20,0x22,0x61,0x73,0x73,0x65,0x74,0x2f,0x6d,0x65,0x73,0x68,0x5f,0x73,0x68,
    0x61,0x64,0x65,0x72,0x2e,0x67,0x6c,0x73,0x6c,0x22,0x0a,0x20,0x20,0x20,0x20,0x6f,
    0x75,0x74,0x2e,0x6f,0x75,0x74,0x43,0x6f,0x6c,0x6f,0x72,0x20,0x3d,0x20,0x69,0x6e,
    0x2e,0x72,0x67,0x62,0x61,0x3b,0x0a,0x23,0x6c,0x69,0x6e,0x65,0x20,0x32,0x32,0x20,
    0x22,0x61,0x73,0x73,0x65,0x74,0x2f,0x6d,0x65,0x73,0x68,0x5f,0x73,0x68,0x61,0x64,
    0x65,0x72,0x2e,0x67,0x6c,0x73,0x6c,0x22,0x0a,0x20,0x20,0x20,0x20,0x6f,0x75,0x74,
    0x2e,0x6f,0x75,0x74,0x54,0x65,0x78,0x43,0x6f,0x6f,0x72,0x64,0x20,0x3d,0x20,0x69,
    0x6e,0x2e,0x78,0x79,0x75,0x76,0x2e,0x7a,0x77,0x3b,0x0a,0x23,0x6c,0x69,0x6e,0x65,
    0x20,0x32,0x34,0x20,0x22,0x61,0x73,0x73,0x65,0x74,0x2f,0x6d,0x65,0x73,0x68,0x5f,
    0x73,0x68,0x61,0x64,0x65,0x72,0x2e,0x67,0x6c,0x73,0x6c,0x22,0x0a,0x20,0x20,0x20,
    0x20,0x6f,0x75,0x74,0x2e,0x67,0x6c,0x5f,0x50,0x6f,0x73,0x69,0x74,0x69,0x6f,0x6e,
    0x20,0x3d,0x20,0x28,0x5f,0x33,0x32,0x2e,0x70,0x72,0x6f,0x6a,0x20,0x2a,0x20,0x5f,
    0x33,0x38,0x2e,0x6d,0x6f,0x64,0x65,0x6c,0x29,0x20,0x2a,0x20,0x66,0x6c,0x6f,0x61,
    0x74,0x34,0x28,0x69,0x6e,0x2e,0x78,0x79,0x75,0x76,0x2e,0x78,0x79,0x2c,0x20,0x30,
    0x2e,0x30,0x2c,0x20,0x31,0x2e,0x30,0x29,0x3b,0x0a,0x20,0x20,0x20,0x20,0x72,0x65,
    0x74,0x75,0x72,0x6e,0x20,0x6f,0x75,0x74,0x3b,0x0a,0x7d,0x0a,0x0a,0x00,
};
/*
    #include <metal_stdlib>
    #include <simd/simd.h>
    
    using namespace metal;
    
    struct main0_out
    {
        float4 outputColor [[color(0)]];
    };
    
    struct main0_in
    {
        float4 outColor [[user(locn0)]];
        float2 outTexCoord [[user(locn1)]];
    };
    
    #line 13 "asset/mesh_shader.glsl"
    fragment main0_out main0(main0_in in [[stage_in]], texture2d<float> tex [[texture(0)]], sampler texSmplr [[sampler(0)]])
    {
        main0_out out = {};
    #line 13 "asset/mesh_shader.glsl"
        out.outputColor = tex.sample(texSmplr, in.outTexCoord) * in.outColor;
        return out;
    }
    
*/
static const char mesh_fs_source_metal_macos[540] = {
    0x23,0x69,0x6e,0x63,0x6c,0x75,0x64,0x65,0x20,0x3c,0x6d,0x65,0x74,0x61,0x6c,0x5f,
    0x73,0x74,0x64,0x6c,0x69,0x62,0x3e,0x0a,0x23,0x69,0x6e,0x63,0x6c,0x75,0x64,0x65,
    0x20,0x3c,0x73,0x69,0x6d,0x64,0x2f,0x73,0x69,0x6d,0x64,0x2e,0x68,0x3e,0x0a,0x0a,
    0x75,0x73,0x69,0x6e,0x67,0x20,0x6e,0x61,0x6d,0x65,0x73,0x70,0x61,0x63,0x65,0x20,
    0x6d,0x65,0x74,0x61,0x6c,0x3b,0x0a,0x0a,0x73,0x74,0x72,0x75,0x63,0x74,0x20,0x6d,
    0x61,0x69,0x6e,0x30,0x5f,0x6f,0x75,0x74,0x0a,0x7b,0x0a,0x20,0x20,0x20,0x20,0x66,
    0x6c,0x6f,0x61,0x74,0x34,0x20,0x6f,0x75,0x74,0x70,0x75,0x74,0x43,0x6f,0x6c,0x6f,
    0x72,0x20,0x5b,0x5b,0x63,0x6f,0x6c,0x6f,0x72,0x28,0x30,0x29,0x5d,0x5d,0x3b,0x0a,
    0x7d,0x3b,0x0a,0x0a,0x73,0x74,0x72,0x75,0x63,0x74,0x20,0x6d,0x61,0x69,0x6e,0x30,
    0x5f,0x69,0x6e,0x0a,0x7b,0x0a,0x20,0x20,0x20,0x20,0x66,0x6c,0x6f,0x61,0x74,0x34,
    0x20,0x6f,0x75,0x74,0x43,0x6f,0x6c,0x6f,0x72,0x20,0x5b,0x5b,0x75,0x73,0x65,0x72,
    0x28,0x6c,0x6f,0x63,0x6e,0x30,0x29,0x5d,0x5d,0x3b,0x0a,0x20,0x20,0x20,0x20,0x66,
    0x6c,0x6f,0x61,0x74,0x32,0x20,0x6f,0x75,0x74,0x54,0x65,0x78,0x43,0x6f,0x6f,0x72,
    0x64,0x20,0x5b,0x5b,0x75,0x73,0x65,0x72,0x28,0x6c,0x6f,0x63,0x6e,0x31,0x29,0x5d,
    0x5d,0x3b,0x0a,0x7d,0x3b,0x0a,0x0a,0x23,0x6c,0x69,0x6e,0x65,0x20,0x31,0x33,0x20,
    0x22,0x61,0x73,0x73,0x65,0x74,0x2f,0x6d,0x65,0x73,0x68,0x5f,0x73,0x68,0x61,0x64,
    0x65,0x72,0x2e,0x67,0x6c,0x73,0x6c,0x22,0x0a,0x66,0x72,0x61,0x67,0x6d,0x65,0x6e,
    0x74,0x20,0x6d,0x61,0x69,0x6e,0x30,0x5f,0x6f,0x75,0x74,0x20,0x6d,0x61,0x69,0x6e,
    0x30,0x28,0x6d,0x61,0x69,0x6e,0x30,0x5f,0x69,0x6e,0x20,0x69,0x6e,0x20,0x5b,0x5b,
    0x73,0x74,0x61,0x67,0x65,0x5f,0x69,0x6e,0x5d,0x5d,0x2c,0x20,0x74,0x65,0x78,0x74,
    0x75,0x72,0x65,0x32,0x64,0x3c,0x66,0x6c,0x6f,0x61,0x74,0x3e,0x20,0x74,0x65,0x78,
    0x20,0x5b,0x5b,0x74,0x65,0x78,0x74,0x75,0x72,0x65,0x28,0x30,0x29,0x5d,0x5d,0x2c,
    0x20,0x73,0x61,0x6d,0x70,0x6c,0x65,0x72,0x20,0x74,0x65,0x78,0x53,0x6d,0x70,0x6c,
    0x72,0x20,0x5b,0x5b,0x73,0x61,0x6d,0x70,0x6c,0x65,0x72,0x28,0x30,0x29,0x5d,0x5d,
    0x29,0x0a,0x7b,0x0a,0x20,0x20,0x20,0x20,0x6d,0x61,0x69,0x6e,0x30,0x5f,0x6f,0x75,
    0x74,0x20,0x6f,0x75,0x74,0x20,0x3d,0x20,0x7b,0x7d,0x3b,0x0a,0x23,0x6c,0x69,0x6e,
    0x65,0x20,0x31,0x33,0x20,0x22,0x61,0x73,0x73,0x65,0x74,0x2f,0x6d,0x65,0x73,0x68,
    0x5f,0x73,0x68,0x61,0x64,0x65,0x72,0x2e,0x67,0x6c,0x73,0x6c,0x22,0x0a,0x20,0x20,
    0x20,0x20,0x6f,0x75,0x74,0x2e,0x6f,0x75,0x74,0x70,0x75,0x74,0x43,0x6f,0x6c,0x6f,
    0x72,0x20,0x3d,0x20,0x74,0x65,0x78,0x2e,0x73,0x61,0x6d,0x70,0x6c,0x65,0x28,0x74,
    0x65,0x78,0x53,0x6d,0x70,0x6c,0x72,0x2c,0x20,0x69,0x6e,0x2e,0x6f,0x75,0x74,0x54,
    0x65,0x78,0x43,0x6f,0x6f,0x72,0x64,0x29,0x20,0x2a,0x20,0x69,0x6e,0x2e,0x6f,0x75,
    0x74,0x43,0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x20,0x20,0x20,0x20,0x72,0x65,0x74,0x75,
    0x72,0x6e,0x20,0x6f,0x75,0x74,0x3b,0x0a,0x7d,0x0a,0x0a,0x00,
};
const sg_shader_desc* mesh_shader_desc(sg_backend backend) {
  if (backend == SG_BACKEND_METAL_MACOS) {
    static sg_shader_desc desc;
    static bool valid;
    if (!valid) {
      valid = true;
      desc.vs.source = mesh_vs_source_metal_macos;
      desc.vs.entry = "main0";
      desc.vs.uniform_blocks[0].size = 64;
      desc.vs.uniform_blocks[0].layout = SG_UNIFORMLAYOUT_STD140;
      desc.vs.uniform_blocks[1].size = 64;
      desc.vs.uniform_blocks[1].layout = SG_UNIFORMLAYOUT_STD140;
      desc.fs.source = mesh_fs_source_metal_macos;
      desc.fs.entry = "main0";
      desc.fs.images[0].name = "tex";
      desc.fs.images[0].image_type = SG_IMAGETYPE_2D;
      desc.fs.images[0].sampler_type = SG_SAMPLERTYPE_FLOAT;
      desc.label = "mesh_shader";
    }
    return &desc;
  }
  return 0;
}
int mesh_attr_slot(const char* attr_name) {
  (void)attr_name;
  if (0 == strcmp(attr_name, "xyuv")) {
    return 0;
  }
  if (0 == strcmp(attr_name, "rgba")) {
    return 1;
  }
  return -1;
}
int mesh_image_slot(sg_shader_stage stage, const char* img_name) {
  (void)stage; (void)img_name;
  if (SG_SHADERSTAGE_FS == stage) {
    if (0 == strcmp(img_name, "tex")) {
      return 0;
    }
  }
  return -1;
}
int mesh_uniformblock_slot(sg_shader_stage stage, const char* ub_name) {
  (void)stage; (void)ub_name;
  if (SG_SHADERSTAGE_VS == stage) {
    if (0 == strcmp(ub_name, "mesh_vs_proj")) {
      return 0;
    }
    if (0 == strcmp(ub_name, "mesh_vs_model")) {
      return 1;
    }
  }
  return -1;
}
size_t mesh_uniformblock_size(sg_shader_stage stage, const char* ub_name) {
  (void)stage; (void)ub_name;
  if (SG_SHADERSTAGE_VS == stage) {
    if (0 == strcmp(ub_name, "mesh_vs_proj")) {
      return sizeof(mesh_vs_proj_t);
    }
    if (0 == strcmp(ub_name, "mesh_vs_model")) {
      return sizeof(mesh_vs_model_t);
    }
  }
  return 0;
}
int mesh_uniform_offset(sg_shader_stage stage, const char* ub_name, const char* u_name) {
  (void)stage; (void)ub_name; (void)u_name;
  if (SG_SHADERSTAGE_VS == stage) {
    if (0 == strcmp(ub_name, "mesh_vs_proj")) {
      if (0 == strcmp(u_name, "proj")) {
        return 0;
      }
    }
    if (0 == strcmp(ub_name, "mesh_vs_model")) {
      if (0 == strcmp(u_name, "model")) {
        return 0;
      }
    }
  }
  return -1;
}
sg_shader_uniform_desc mesh_uniform_desc(sg_shader_stage stage, const char* ub_name, const char* u_name) {
  (void)stage; (void)ub_name; (void)u_name;
  #if defined(__cplusplus)
  sg_shader_uniform_desc desc = {};
  #else
  sg_shader_uniform_desc desc = {0};
  #endif
  if (SG_SHADERSTAGE_VS == stage) {
    if (0 == strcmp(ub_name, "mesh_vs_proj")) {
      if (0 == strcmp(u_name, "proj")) {
        desc.name = "proj";
        desc.type = SG_UNIFORMTYPE_MAT4;
        desc.array_count = 1;
        return desc;
      }
    }
    if (0 == strcmp(ub_name, "mesh_vs_model")) {
      if (0 == strcmp(u_name, "model")) {
        desc.name = "model";
        desc.type = SG_UNIFORMTYPE_MAT4;
        desc.array_count = 1;
        return desc;
      }
    }
  }
  return desc;
}
#endif /* SOKOL_SHDC_IMPL */
