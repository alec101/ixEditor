#version 450
//#extension GL_ARB_separate_shader_objects : enable

// in / out

layout(location= 0) in vec3 in_pos;    // vert channel
layout(location= 1) in vec3 in_nrm;    // nrm channel
layout(location= 2) in vec2 in_tex;    // tex channel
layout(location= 0) out vec3 out_UV;

// sets

layout(set= 0, binding= 0) uniform GlobalUniforms {
  mat4 cameraPersp;       // perspective camera matrix
  mat4 cameraOrtho;       // orthographic camera matrix
  int vpx, vpy;           // viewport position on the virtual desktop
} glb;

layout(set= 1, binding= 1) uniform sampler2D texSampler;

//layout(set= 1, binding= 0) uniform UniformBufferObject {
//  mat4 camera;
//} ubo;

//  push constants:

/*
layout(push_constant) uniform PConsts {
  int blockID;    // terrain block index
  //int view;     // one of the 4 possible camera positions
  float x, y, z;  // position
  // view could be flag-type, and you flag what side is visible
} p;

*/


// ###################################################################################
void main() {
  //if((gl_VertexIndex- p.posID)
  //gl_PointSize= 5;

  /*
  mat4 t= mat4(1, 0, 0, 0,
               0, 1, 0, 0,
               0, 0, 1, 0,
               p.x, p.y, 0, 1);
               */
  //gl_Position= vec4(p.x, p.y, 0, 1);
  //gl_Position= glb.cameraPersp* gl_Position;
  //gl_Position= glb.cameraPersp* vec4(in_pos, 1);


  gl_Position= vec4(in_pos, 1); //+ vec4(p.x, p.y, p.z, 0);
  gl_Position= glb.cameraPersp* gl_Position;


  out_UV= vec3(in_tex, 1);
}




// IMPORTANT TO LEARN/REMEMBER:
// -location acts on 32bit vectors, so 4x 32bit variables.
// -a variable/vector that is bigger than 4x32bit would use 2 slots
//   location imediatly after it is unavaible (you must use location=x+2 for the next variable)
// -vec3 v[3]; // in c++ this will be packed not padded, even with alignas(16) (micro compiler), in vksl array members are padded, aligned on 16
// -I SAW THIS, IT MIGHT BE NEEDED, I AM NOT SURE   <<< seems not
//  out gl_PerVertex {
//    vec4 gl_Position;
//        gl_PointSize
//        gl_ClipDistance
//        gl_CullDistance
//  };
// -max push constant= 128 to 256 (2 matrix tops)
