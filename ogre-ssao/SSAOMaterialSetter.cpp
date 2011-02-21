#include "precompiled.h"
#include "SSAOMaterialSetter.hpp"

SSAOMaterialSetter::SSAOMaterialSetter(void)
{
}

SSAOMaterialSetter::~SSAOMaterialSetter(void)
{
}



void SSAOMaterialSetter::CreatedEntity(const OgreMax::OgreMaxScene *scene, Ogre::Entity *entity)
{
    entity->setMaterialName("SSAO/DiffuseLight_GBuffer");

}