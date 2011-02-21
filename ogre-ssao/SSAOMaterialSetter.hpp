#pragma once

#include <OgreMaxScene.hpp>

class SSAOMaterialSetter :
    public OgreMax::OgreMaxSceneCallback
{
public:
    SSAOMaterialSetter(void);
    ~SSAOMaterialSetter(void);

    void CreatedEntity(const OgreMax::OgreMaxScene *scene, Ogre::Entity *entity);
};
