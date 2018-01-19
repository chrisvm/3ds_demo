#include "scene_context.h"

void SceneContext::InitShader() {
    // Load the vertex shader, create a shader program and bind it
	this->vshader_dvlb = DVLB_ParseFile((u32*) vshader_shbin, vshader_shbin_size);
	shaderProgramInit(&this->program);
	shaderProgramSetVsh(&this->program, &this->vshader_dvlb->DVLE[0]);
	C3D_BindProgram(&this->program);
}

SceneContext::~SceneContext() {
    // Free the shader program
	shaderProgramFree(&program);
	DVLB_Free(vshader_dvlb);

	// Free the vbo
	linearFree(vbo);
}
