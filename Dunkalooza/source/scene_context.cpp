#include "scene_context.h"

SceneContext::SceneContext()
{
	InitShader();

	// Allocate VBO
	vbo = (VBOEntry*) linearAlloc(sizeof(VBOEntry) * 12);

	// Configure buffers
	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vbo, sizeof(VBOEntry), 2, 0x10);

	// Compute the projection matrix
	// Note: we're setting top to 240 here so origin is at top left.
	Mtx_OrthoTilt(&projection, 0.0, 400.0, 240.0, 0.0, 0.0, 1.0, true);

	// init physics
	//const C3D_FVec gravity = FVec3_New(0.0f, 0.0f, 0.0f);
	Scene_Init(&phy_scene, DELTA_TIME, FVec3_New(0.0f, 0.0f, 0.0f), 1);
}

void SceneContext::InitShader() {
    // Load the vertex shader, create a shader program and bind it
	vshader_dvlb = DVLB_ParseFile((u32*) vshader_shbin, vshader_shbin_size);
	shaderProgramInit(&program);
	shaderProgramSetVsh(&program, &vshader_dvlb->DVLE[0]);
	C3D_BindProgram(&program);

	// Get the location of the uniforms
	uLoc_projection = shaderInstanceGetUniformLocation(program.vertexShader,
                                                       "projection");

	uLoc_model = shaderInstanceGetUniformLocation(program.vertexShader,
													  "model");

	// Configure attributes for use with the vertex shader
	// Attribute format and element count are ignored in immediate mode
	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v2=texcoord

	// Configure the first fragment shading substage to just pass through
    // the texture color
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, 0, 0);
	C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);

	// Configure depth test to overwrite pixels with the same depth
    // (needed to draw overlapping sprites)
	C3D_DepthTest(true, GPU_GEQUAL, GPU_WRITE_ALL);
}

SceneContext::~SceneContext() {
    // Free the shader program
	shaderProgramFree(&program);
	DVLB_Free(vshader_dvlb);

	// Free the vbo
	linearFree(vbo);
}
