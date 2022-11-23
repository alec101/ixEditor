#include "ixEditor.h"


I3Deditor::I3Deditor(EditorWindow *in_win): win(in_win), ui(this, in_win), input(this, in_win), output(this, in_win) {
  ix= &in_win->ix;

  sl= null;
  cmd[0]= cmd[1]= null;
  pool[0]= pool[1]= null;
  defCamPos.set(0, 1, 20);
  //camPos= defCamPos;


  defAngle.set(-54.7356f, 0.0f, 45.0f);   // isometric
  //angle= defAngle;

  /* old method with angles
  defDist= 1.0f;
  defAngle.set(-54.7356f, 0.0f, 45.0f);   // isometric

  dist= defDist;
  angle= defAngle;
  */
}


I3Deditor::~I3Deditor() {
}


void I3Deditor::delData() {
}




void I3Deditor::init() {
  ui.init();
  //input.init();
  output.init();

  sunColor.set(1.0f, 1.0f, 0.95f);
  sunPos.set(3000, 2000, 5000);   // it's actually good, nothing's wrong; rotate the sculpture, and you will notice, -x and -y are actually right
  sunAmbientStr= 0.05f;
  sunSpecularStr= 0.9f;

  backgroundColor.set(0.3f, 0.3f, 0.3f, 1.0f);
  
  // shader
  sl= new ixShader(&win->ix);
  sl->vk->loadModuleVert("shaders/I3Dviewer.vert.spv");
  sl->vk->loadModuleFrag("shaders/I3Dviewer.frag.spv");

  sl->vk->setInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
  //sl->vk->setInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP, VK_FALSE);
  sl->vk->setFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
  //sl->vk->setFrontFace(VK_FRONT_FACE_CLOCKWISE);

  sl->vk->setCullModeFlags(VK_CULL_MODE_BACK_BIT);
  //sl->vk->setCullModeFlags(VK_CULL_MODE_NONE);
  sl->vk->setRenderPass(*ix->vki.render.handle);
  sl->vk->setDynamicViewports(1);
  sl->vk->addColorBlendAttachement(true, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                         VK_BLEND_FACTOR_ONE,       VK_BLEND_FACTOR_ZERO,                VK_BLEND_OP_ADD,
                                         VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

  /// in/out
  sl->vk->addVertexBinding(0, sizeof(vec3)+ sizeof(vec3)+ sizeof(vec2), VK_VERTEX_INPUT_RATE_VERTEX);       // size of stride is vec3(pos), vec3(nrm), vec2(tex)
  //sl->vk->addVertexBinding(0, sizeof(vec3)+ sizeof(vec3)+ sizeof(vec2), VK_VERTEX_INPUT_RATE_INSTANCE);
  sl->vk->addVertexAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0);    // position
  sl->vk->addVertexAttribute(1, 0, VK_FORMAT_R32G32B32_SFLOAT, 12);   // normals
  sl->vk->addVertexAttribute(2, 0, VK_FORMAT_R32G32_SFLOAT, 24);      // tex coords
  /// sets
  sl->vk->addDescriptorSetFromExisting(ix->vki.glb[0]->layout);       /// set 0, binding 0 - global buffer
  sl->vk->addDescriptorSetFromExisting(ix->res.tex.vkd.standard4mapsLayout);  /// set 1, binding 0 - material
  /// push consts
  sl->vk->addPushConsts(sizeof(PConsts), 0, VK_SHADER_STAGE_ALL);

  sl->vk->setDepthTest(true, true,  VK_COMPARE_OP_LESS, false);
  //sl->vk->setDepthTest(true, true,  VK_COMPARE_OP_GREATER, false);

  if(!sl->vk->build()) error.window(ix->vk.errorText, __FUNCTION__);

  // terrain memory cluster
  //ix->vki.addCluster(&cluster);
  //cluster->configure(TERRAIN_MEM_SIZE, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

  push.model.identity();


  for(uint a= 0; a< 2; a++) {

    pool[a]= ix->vk.objects.addCommandPool();
    pool[a]->configure(ix->vki.q1->family);
    pool[a]->build();

    cmd[a]= pool[a]->addCommandBuffer();
    cmd[a]->cfgLevel(VK_COMMAND_BUFFER_LEVEL_SECONDARY);
    cmd[a]->cfgUsage(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT| VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT);
    cmd[a]->cfgInheritance(*ix->vki.render.handle, 0, 0, false, 0, 0);
    cmd[a]->build();
  }


  //ix->camera= &ix->cameraPersp;
  ix->cameraPersp.move(defCamPos, vec3(0, 0, 0));
  computePerspective();
  ix->cameraPersp.computeViewMat();
  ix->cameraPersp.computeProjMat();
  ix->cameraPersp.computeCamMat();
  // material
  //mat= ix->res.mat.add();

}



void I3Deditor::computePerspective() {
  float aspect= (float)ix->win->dx/ (float)ix->win->dy;
  float dx, dy;
  dx= 0.5f;
  dy= 0.5f; /// aspect;
  dy/= aspect;

  ix->cameraPersp.setPerspective(ix->cameraPersp.pos, ix->cameraPersp.target, 30, aspect, 0.001f, 100.0f);
  
  //ix->camera->setPerspective(ix->camera->pos, ix->camera->target, 30, aspect, 0.001, 100);
}



void I3Deditor::update() {
  ui.update();
  input.update();
  //output.update();


  //mat->updateSet();
   
  /*
  shaders compiled;
  model is in push consts, that has to be updated
  many new vars in glb, must update them all

  phong is not done, but just diffuse will do for now;

  in theory this is all that's needed, and i need to really cement these things, cuz they're "standard" for a shader

    maybe make a func that i will just copy paste <<<<<<<<<<<<<< (for shaders)
    a streamlined thing must happen for sure.


    all these must happen in ix->camera
    https://learnopengl.com/Getting-started/Camera

  ;
  */

  ix->camera= &ix->cameraPersp;
  if(osi.flags.windowResized) {
    computePerspective();
  }

  //ix->camera->setPerspective(camPos, vec3(0, 0, 0), 30, aspect, 0.001, 100);

  ix->cameraPersp.computeProjMat();
  ix->cameraPersp.computeViewMat();

  //ix->camera->viewMat.scale(scale, scale, scale);

  /* old method with angles and distance
  ix->camera->viewMat*= mat4().translate(0, 0, dist);
  ix->camera->viewMat*= mat4().vkRotateX(angle.x);
  ix->camera->viewMat*= mat4().vkRotateY(angle.y);
  ix->camera->viewMat*= mat4().vkRotateZ(angle.z);
  */

  //ix->camera->viewMat*= mat4().scale(scale, scale, scale);

  ix->cameraPersp.computeCamMat();

  ix->vki.glb[ix->vki.fi]->data->sunColor=   sunColor;
  ix->vki.glb[ix->vki.fi]->data->sunPos=     sunPos;
  ix->vki.glb[ix->vki.fi]->data->sunAmbientStr= sunAmbientStr;
  ix->vki.glb[ix->vki.fi]->data->sunSpecularStr= sunSpecularStr;
  ix->vki.glb[ix->vki.fi]->data->cameraPos=  ix->cameraPersp.pos;
  ix->vki.clearValues->color= { backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a};
}


void I3Deditor::draw() {

  uint32 fi= ix->vki.fi;
  VkCommandBuffer c= cmd[fi]->buffer;
  VkDeviceSize offsets= 0;
  //vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
  //vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);
  //vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);



  ix->startPerspective();

  pool[fi]->reset();

  cmd[fi]->_inheritanceInfo.framebuffer= ix->vki.swap.framebuffer[ix->vki.swap.handle->currentIndex]->framebuffer;
  cmd[fi]->startRecording();
    ix->vk.CmdSetViewport(c, 0, 1, &ix->vki.render.viewport);
    ix->vk.CmdSetScissor(c, 0, 1, &ix->vki.render.scissor);

    /*
    sun has to happen; must research lightning, tho, never did it;
    so it's a new thing to do
      it has to be something real easy i think, based on normals & sun vector;

    then maybe specuar and other stuff, i think i must refine the materials and shaders, as a next big step;

    learnopengl has all the info i need;
    phong is per fragment - slower - way better
    gourand is per vertex - fast - not that great

    the camera position must be found, learnopengl has it, so i think i must start from there too, cuz it's fuzzy how i got things
      the 'world' 'model' 'view' stuff, is fuzzy, there has to be camera position (eye pos) , and learnopengl has everything there, more clearly
      */


    if(output.outMesh.buf) {
      ix->vk.CmdBindPipeline(c, VK_PIPELINE_BIND_POINT_GRAPHICS, sl->vk->pipeline);

      /// in/out

      ix->vk.CmdBindVertexBuffers(c, 0, 1, &output.outMesh.buf->handle->buffer, &offsets);
      //ix->vk.CmdBindVertexBuffers(c, 0, 1, &bufVert->handle->buffer, &offsets);
      //ix->vk.CmdBindIndexBuffer(c, *bufIndex, 0, VK_INDEX_TYPE_UINT16);
    
      /// sets

      ix->vk.CmdBindDescriptorSets(c, VK_PIPELINE_BIND_POINT_GRAPHICS, sl->vk->pipelineLayout, 0, 1, &ix->vki.glb[fi]->set->set, 0, null);

      // this is null vvv material sometimes works, but it mainly should crash all the time
      ix->vk.CmdBindDescriptorSets(c, VK_PIPELINE_BIND_POINT_GRAPHICS, sl->vk->pipelineLayout, 1, 1, &output.mat->set->set, 0, null);

      /// push consts

      push.flags= 0;
      push.flags.setDown(0x0001);
    
      ix->vk.CmdPushConstants(c, sl->vk->pipelineLayout, VK_SHADER_STAGE_ALL, 0, sizeof(PConsts), &push);
      
      
      ix->vk.CmdDraw(c, output.outMesh.nrVert, 1, 0, 0);
    }

  cmd[fi]->endRecording();
  ix->vk.CmdExecuteCommands(*ix->vki.render.cmdMain[fi], 1, &cmd[fi]->buffer);

  ix->endPerspective();

}
















