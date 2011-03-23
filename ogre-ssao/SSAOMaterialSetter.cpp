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
    //entity->setMaterialName("SSAO/DiffuseLight_GBuffer");

    int n = entity->getNumSubEntities();
    for(int i=0 ; i<n ; i++)
    {
        Ogre::SubEntity *subEntity = entity->getSubEntity(i);
        Ogre::MaterialPtr material = subEntity->getMaterial();

        Ogre::Technique *geomTechnique = material->createTechnique();
        geomTechnique->setSchemeName("geom");
        geomTechnique->setName("geom");

        Ogre::Pass *geomBufferPass = geomTechnique->createPass();
        geomBufferPass->setVertexProgram("gbuffer_vs");
        geomBufferPass->setFragmentProgram("gbuffer_ps");

    }
}
