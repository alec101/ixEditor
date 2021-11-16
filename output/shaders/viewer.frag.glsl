#version 450
//#extension GL_ARB_separate_shader_objects : enable

// in / out

layout(location= 0) in vec3 in_UV;
layout(location= 0) out vec4 out_color;

// sets

layout(set= 0, binding= 0) uniform GlobalUniforms {
  mat4 cameraPersp;       // perspective camera matrix
  mat4 cameraOrtho;       // orthographic camera matrix
  int vpx, vpy;           // viewport position on the virtual desktop
  vec4 outsideLight;
  vec4 sun;
} glb;

layout(set= 1, binding= 0) uniform sampler2DArray texSampler0;
layout(set= 1, binding= 1) uniform sampler2DArray texSampler1;
layout(set= 1, binding= 2) uniform sampler2DArray texSampler2;
layout(set= 1, binding= 3) uniform sampler2DArray texSampler3;

//  push constants:

layout(push_constant) uniform PConsts {
  //int blockID;   // terrain block index
  //int view;      // one of the 4 possible camera positions
  //float x, y, z;    // position
  // view could be flag-type, and you flag what side is visible
  int map0, map1, map2, map3;   // index of texture mappings

  int flags;                    // 0x0001 selected block
} p;


void main() {

  //out_color= texture2D(texSampler0, vec2(in_UV.xy); // layer 0 atm <<<<<
  //out_color= texture2DArray(texSampler0, vec3(in_UV.xy, 0).rgb); // layer 0 atm <<<<<

  out_color= texture(texSampler0, vec3(in_UV.xy, p.map0).rgb);



  if((p.flags& 0x0001) == 1) out_color= vec4(1.0, 1.0, 1.0, 1.0);
  out_color*= glb.outsideLight;

  //out_color= vec4(1.0, 1.0, 1.0, 0.8); // DEBUG<<<<<<<<<
}

