#include "ix3do.h"


Viewer::Viewer() {
  sl= null;
  cmd[0]= cmd[1]= null;
  pool[0]= pool[1]= null;

}


Viewer::~Viewer() {
}


void Viewer::delData() {
}




void Viewer::init() {
    // shader
  sl= new ixShader(&ix);
  sl->vk->loadModuleVert("shaders/viewer.vert.spv");
  sl->vk->loadModuleFrag("shaders/viewer.frag.spv");

  sl->vk->setInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
  //sl->vk->setInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP, VK_FALSE);
  //sl->vk->setFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
  sl->vk->setFrontFace(VK_FRONT_FACE_CLOCKWISE);
  sl->vk->setCullModeFlags(VK_CULL_MODE_BACK_BIT);
  //sl->vk->setCullModeFlags(VK_CULL_MODE_NONE);
  sl->vk->setRenderPass(*ix.vki.render.handle);
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
  sl->vk->addDescriptorSetFromExisting(ix.vki.glb[ix.vki.fi]->layout);       /// set 0, binding 0 - global buffer
  sl->vk->addDescriptorSetFromExisting(ix.res.tex.vkd.standard4mapsLayout);  /// set 1, binding 0 - material
  /// push consts
  sl->vk->addPushConsts(sizeof(PConsts), 0, VK_SHADER_STAGE_ALL);

  sl->vk->setDepthTest(true, true,  VK_COMPARE_OP_LESS, false);
  //sl->vk->setDepthTest(true, true,  VK_COMPARE_OP_GREATER, false);

  if(!sl->vk->build()) error.window(ix.vk.errorText, __FUNCTION__);

  // terrain memory cluster
  //ix.vki.addCluster(&cluster);
  //cluster->configure(TERRAIN_MEM_SIZE, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);


  for(uint a= 0; a< 2; a++) {
    pool[a]= ix.vk.objects.addCommandPool();
    pool[a]->configure(ix.vki.q1->family);
    pool[a]->build();

    cmd[a]= pool[a]->addCommandBuffer();
    cmd[a]->cfgLevel(VK_COMMAND_BUFFER_LEVEL_SECONDARY);
    cmd[a]->cfgUsage(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT| VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT);
    cmd[a]->cfgInheritance(*ix.vki.render.handle, 0, 0, false, 0, 0);
    cmd[a]->build();
  }

}




void Viewer::update() {


}


void Viewer::draw() {

  uint32 fi= ix.vki.fi;
  VkCommandBuffer c= cmd[fi]->buffer;

  //vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
  //vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);
  //vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);



  /*
  there has to be buffers, an array, growing, shrinking, same size
    these buffers will hold the block shapes, the number to draw, and a matrix for each, tho this matrix could be smaller, depends how i can do the rotates

    so a number and a bunch of matrices

    also another chain of buffers for the actual meshes, but this shoudl be fixed in size
    still maybe not all will fit in 1 buffer... tho... maybe they will
    */




  pool[fi]->reset();

  cmd[fi]->_inheritanceInfo.framebuffer= ix.vki.swap.framebuffer[ix.vki.swap.handle->currentIndex]->framebuffer;
  cmd[fi]->startRecording();
    ix.vk.CmdSetViewport(c, 0, 1, &ix.vki.render.viewport);
    ix.vk.CmdSetScissor(c, 0, 1, &ix.vki.render.scissor);

    // terrain draw

    ix.vk.CmdBindPipeline(c, VK_PIPELINE_BIND_POINT_GRAPHICS, sl->vk->pipeline);

    // in/out

    //ix.vk.CmdBindVertexBuffers(c, 0, 1, &bufVert->handle->buffer, &offsets);
    //ix.vk.CmdBindIndexBuffer(c, *bufIndex, 0, VK_INDEX_TYPE_UINT16);
    
    // sets

    ix.vk.CmdBindDescriptorSets(c, VK_PIPELINE_BIND_POINT_GRAPHICS, sl->vk->pipelineLayout, 0, 1, &ix.vki.glb[fi]->set->set, 0, null);

    //push.blockID= 0;
    push.flags= 0;
    push.flags.setDown(0x0001);
    
    ix.vk.CmdPushConstants(c, sl->vk->pipelineLayout, VK_SHADER_STAGE_ALL, 0, sizeof(PConsts), &push);

    //Segment *seg= camera.seg;
    //seg->draw(c, (int32)camera.coord.z- 3);
    //seg->draw(c, (int32)camera.coord.z- 2);
    //seg->draw(c, (int32)camera.coord.z- 1);
    //seg->draw(c, (int32)camera.coord.z);
    //seg->draw(c, (int32)camera.coord.z+ 1);
    

    //ui.drawTerrainSel(c);

  cmd[fi]->endRecording();
  ix.vk.CmdExecuteCommands(*ix.vki.render.cmdMain[fi], 1, &cmd[fi]->buffer);
  while(0);
}
















