#version 450
//#extension GL_ARB_separate_shader_objects : enable

// in / out

layout(location= 0) in vec3 in_UV;
layout(location= 1) in vec3 in_nrm;
layout(location= 2) in vec3 in_fragPos;

layout(location= 0) out vec4 out_color;

// sets

layout(set= 0, binding= 0) uniform GlobalUniforms {
  mat4 cameraPersp;       // perspective camera matrix
  mat4 cameraOrtho;       // orthographic camera matrix
  vec3 cameraPos;
  int vpx, vpy;           // viewport position on the virtual desktop

  vec3 sunPos;
  vec3 sunColor;
  float sunAmbientStr;
  float sunSpecularStr;
} glb;

/*
layout(set= 1, binding= 0) uniform sampler2DArray texSampler0;
layout(set= 1, binding= 1) uniform sampler2DArray texSampler1;
layout(set= 1, binding= 2) uniform sampler2DArray texSampler2;
layout(set= 1, binding= 3) uniform sampler2DArray texSampler3;
*/

layout(set= 1, binding= 0) uniform sampler2D texSampler0;
layout(set= 1, binding= 1) uniform sampler2D texSampler1;
layout(set= 1, binding= 2) uniform sampler2D texSampler2;
layout(set= 1, binding= 3) uniform sampler2D texSampler3;

//  push constants:

layout(push_constant) uniform PConsts {
  mat4 model;
  int map0, map1, map2, map3;   // index of texture mappings
  int flags;                    // 0x0001 selected block
} p;




// negative ambientStr negates ambient
// negative specularStr negates specular compute
vec3 pointLight(vec3 in_lightPos, vec3 in_ambientColor, vec3 in_diffuseColor, vec3 in_specularColor, float in_ambientStr, float in_specularStr, int in_shininess) {
  vec3 norm= normalize(in_nrm);
  vec3 lightDir= normalize(in_lightPos- in_fragPos);
  vec3 retLight= vec3(0.0, 0.0, 0.0);

  // ambient
  if(in_ambientStr>= 0.0)
    retLight+= vec3(in_ambientStr* in_ambientColor);

  // diffuse
  float diff= max(dot(norm, lightDir), 0.0);
  retLight+= vec3(diff* in_diffuseColor);

  // specular
  if(in_specularStr>= 0.0) {
    vec3 viewDir= normalize(glb.cameraPos- in_fragPos);
    vec3 reflectDir= reflect(-lightDir, norm);  
    float spec= pow(max(dot(viewDir, reflectDir), 0.0), in_shininess);
    retLight+= in_specularStr* spec* in_specularColor; 
  }

  return retLight;
}



void main() {

  //out_color= texture2D(texSampler0, vec2(in_UV.xy); // layer 0 atm <<<<<
  //out_color= texture2DArray(texSampler0, vec3(in_UV.xy, 0).rgb); // layer 0 atm <<<<<
  // out_color= texture(texSampler0, vec3(in_UV.xy, p.map0).rgb);  // <<< array

  out_color= texture(texSampler0, vec2(in_UV.xy));  // <<< array
  //if((p.flags& 0x0001) == 1) out_color= vec4(1.0, 1.0, 1.0, 1.0);

  out_color= vec4(1, 1, 1, 1);

  vec3 sunColor= pointLight(glb.sunPos, glb.sunColor, glb.sunColor, glb.sunColor, glb.sunAmbientStr, glb.sunSpecularStr, 32);
  out_color= vec4(sunColor, 1)* out_color;

  // maybe the normal matrix can be sent too: Normal = mat3(transpose(inverse(model))) * aNormal;  - this should be computed and sent by the cpu

  //out_color= vec4(1.0, 1.0, 1.0, 0.8); // DEBUG<<<<<<<<<
}

