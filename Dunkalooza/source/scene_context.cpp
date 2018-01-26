#include "scene_context.h"

void SceneContext::InitShader() {
    // Load the vertex shader, create a shader program and bind it
	this->vshader_dvlb = DVLB_ParseFile((u32*) vshader_shbin, vshader_shbin_size);
	shaderProgramInit(&this->program);
	shaderProgramSetVsh(&this->program, &this->vshader_dvlb->DVLE[0]);
	C3D_BindProgram(&this->program);

	// Get the location of the uniforms
	scene->uLoc_projection = shaderInstanceGetUniformLocation(scene->program.vertexShader,
                                                              "projection");

	// Configure attributes for use with the vertex shader
	// Attribute format and element count are ignored in immediate mode
	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v2=texcoord

	// Configure the first fragment shading substage to just pass through
    // the texture color
	// See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, 0, 0);
	C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);
}

SceneContext::~SceneContext() {
    // Free the shader program
	shaderProgramFree(&program);
	DVLB_Free(vshader_dvlb);

	// Free the vbo
	linearFree(vbo);
}
