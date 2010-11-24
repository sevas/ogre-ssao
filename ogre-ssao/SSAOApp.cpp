/* This program is free software. It comes without any warranty, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://sam.zoy.org/wtfpl/COPYING for more details. */

#include "precompiled.h"

#include "SSAOApp.h"

#include <sstream>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

//-----------------------------------------------------------------------------
SSAOApp::SSAOApp()
    :OgreApplication("SSAO sandbox", CT_MOUSE)
    ,mBBset(NULL)
    ,mLightFlare(NULL)
    ,mLight(NULL)
    ,mLightNode(NULL)
{
}
//-----------------------------------------------------------------------------
void SSAOApp::createScene()
{
    mLog  = Ogre::LogManager::getSingleton().createLog("SSAO.log");


    const Ogre::RenderSystemCapabilities* caps = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities();
    if (!caps->hasCapability(Ogre::RSC_VERTEX_PROGRAM) || !(caps->hasCapability(Ogre::RSC_FRAGMENT_PROGRAM)))
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "Your card does not support vertex and fragment programs, so cannot "
            "run this application. Sorry!", 
            "createScene");
    }


    mSceneMgr->setNormaliseNormalsOnScale(true);
    //_createGrid(500);
    _createLight();
    _populate();


    mCamera->setPosition(-200, 100, 500);
    mCamera->lookAt(Ogre::Vector3::ZERO);


    _createDebugOverlay();
}
//-----------------------------------------------------------------------------
bool SSAOApp::frameStarted(const Ogre::FrameEvent& evt)
{
    bool cont = OgreApplication::frameStarted(evt);


    return cont;
}
//-----------------------------------------------------------------------------
void SSAOApp::_populate()
{   using namespace Ogre;


    _buildRoom();

    const int n=3;
    const int spacing = 30;
    for(int i=0 ; i<n ; i++)
    {
        for (int j=0 ; j<n ; j++)
        {
            _loadMesh("Box01", Vector3(i*spacing, 0.1, j*spacing));
        }
    }

    _loadMesh("ogrehead", Vector3(-50, 20, 0));


    Vector3 cassiniPosition = Vector3(-300, 100, 0);
    _loadMesh("cassini_10", cassiniPosition);
    _loadMesh("cassini_11", cassiniPosition);
    _loadMesh("cassini_13", cassiniPosition);
    _loadMesh("cassini_14", cassiniPosition);
    _loadMesh("cassini_15", cassiniPosition);
    _loadMesh("cassini_a1", cassiniPosition);
    _loadMesh("cassini_a2", cassiniPosition);
    _loadMesh("cassini_a4", cassiniPosition);
    _loadMesh("cassini_a5", cassiniPosition);
    _loadMesh("cassini_a6", cassiniPosition);
    _loadMesh("cassini_a7", cassiniPosition);
    _loadMesh("cassini_a8", cassiniPosition);
    _loadMesh("cassini_as", cassiniPosition);


    Ogre::SceneNode *node= _loadMesh("hebemissin", Vector3(0, 0, 200));
    node->scale(20, 20, 20);

}
//-----------------------------------------------------------------------------
void SSAOApp::_buildRoom()
{  
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0.0f);
    Ogre::MeshPtr planeMesh = 
        Ogre::MeshManager::getSingleton().createPlane("Plane.mesh"
        ,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
        ,plane
        , 1000, 1000
        , 1, 1, true
        , 1, 5, 5
        , Ogre::Vector3::UNIT_Z );


    Ogre::SceneNode *wallNode;
    wallNode = _makeWall("Plane.mesh", "ground");

    wallNode = _makeWall("Plane.mesh", "left wall");
    wallNode->translate(-500, 500, 0);
    wallNode->roll(Ogre::Radian(-Ogre::Math::PI / 2));

    wallNode = _makeWall("Plane.mesh", "back wall");
    wallNode->translate(0, 500, -500);
    wallNode->pitch(Ogre::Radian(Ogre::Math::PI / 2));

    wallNode = _makeWall("Plane.mesh", "right wall");
    wallNode->translate(500, 500, 0);
    wallNode->roll(Ogre::Radian(Ogre::Math::PI / 2));



}
//-----------------------------------------------------------------------------
Ogre::SceneNode* SSAOApp::_makeWall(const std::string &_meshName, const std::string &_name)
{
    Ogre::Entity *wall;
    Ogre::SceneNode *wallNode;
    wall = mSceneMgr->createEntity(_name, _meshName);
    wall->setMaterialName("SSAO/DiffuseLight_GBuffer");
    wallNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(_name+"_node");
    wallNode->attachObject(wall);
    return wallNode;
}
//-----------------------------------------------------------------------------
Ogre::SceneNode* SSAOApp::_loadMesh(const Ogre::String &_name, const Ogre::Vector3 &_pos)
{
    std::string entityName = _name+Ogre::StringConverter::toString(mScenePairs.size());

    Ogre::Entity *ent = mSceneMgr->createEntity(entityName, _name+".mesh");
    Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode(ent->getName()+"Node", _pos);


    ent->setMaterialName("SSAO/DiffuseLight_GBuffer");
    node->attachObject(ent);

    mScenePairs.push_back(SSAOApp::ScenePair(ent, node));

    return node;
}
//-----------------------------------------------------------------------------
void SSAOApp::_createLight()
{
    mBBset = mSceneMgr->createBillboardSet("Light BB");
    mBBset->setMaterialName("Objects/Flare");
    mLightFlare = mBBset->createBillboard(Ogre::Vector3::ZERO);

    mLight = mSceneMgr->createLight("main light");
    mLight->setType(Ogre::Light::LT_POINT);
    mLight->setDiffuseColour(Ogre::ColourValue::White);
    mLight->setSpecularColour(Ogre::ColourValue::White);

    mLightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("light node");
    mLightNode->attachObject(mLight);
    mLightNode->attachObject(mBBset);

    mLightNode->setPosition(Ogre::Vector3(0, 200, 500));

}
//-----------------------------------------------------------------------------
