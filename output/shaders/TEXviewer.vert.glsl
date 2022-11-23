#version 450
//#extension GL_ARB_separate_shader_objects : enable

// in / out


// sets

layout(set= 0, binding= 0) uniform GlobalUniforms {
  mat4 cameraPersp;       // perspective camera matrix
  mat4 cameraUI;          // UI orthographic camera matrix
  vec3 cameraPos;         // camera position (eye) in the world
  vec2 vp;                // viewport position on the virtual desktop
  vec2 vs;                // viewport size

} glb;

layout(set= 1, binding= 1) uniform sampler2D texSampler;

//  push constants:

layout(push_constant) uniform PConsts {
} p;



// ###################################################################################
void main() {
}





// IMPORTANT TO LEARN/REMEMBER:
// -location acts on 32bit vectors, so 4x 32bit variables.
// -a variable/vector that is bigger than 4x32bit would use 2 slots
//   location imediatly after it is unavaible (you must use location=x+2 for the next variable)
// -vec3 v[3]; // in c++ this will be packed not padded, even with alignas(16) (micro compiler), in vksl array members are padded, aligned on 16
// -alignment:
//   alignas(4) float/int32 | alignas(8) vec2 | alignas(16) vec3/vec4 | alignas(16) mat4|
//   struct { vec2  a; vec3  b; };    has garbage of 8 between them, and garbage of 4 after vec3;
//   struct { vec2  a; vec2  b; };    has no garbage
//   struct { vec2  a; float b, c; }; has no garbage
//   struct { float a; vec4  b; };    has garbage of 12 between a and b

// -I SAW THIS, IT MIGHT BE NEEDED, I AM NOT SURE   <<< seems not
//  out gl_PerVertex {
//    vec4 gl_Position;
//        gl_PointSize
//        gl_ClipDistance
//        gl_CullDistance
//  };
// -max push constant= 128 to 256 (2 matrix tops)
