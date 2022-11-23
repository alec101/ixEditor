#version 450
//#extension GL_ARB_separate_shader_objects : enable

// in / out


// sets

layout(set= 0, binding= 0) uniform GlobalUniforms {
  mat4 cameraPersp;       // perspective camera matrix
  mat4 cameraUI;          // orthographic camera matrix
  vec3 cameraPos;
  vec2 vp;                // viewport position on the virtual desktop
  vec2 vs;                // viewport size

} glb;


layout(set= 1, binding= 0) uniform sampler2D texSampler0;
layout(set= 1, binding= 1) uniform sampler2D texSampler1;
layout(set= 1, binding= 2) uniform sampler2D texSampler2;
layout(set= 1, binding= 3) uniform sampler2D texSampler3;

//  push constants:

layout(push_constant) uniform PConsts {
} p;





void main() {

}

