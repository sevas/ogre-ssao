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

#include "SSAOMaterialSetter.hpp"



#define TAU 2*Ogre::Math::PI

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


    mCamera->setPosition(-500, 100, 0);
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
{   
    //_loadSponzaAtrium();
    _loadHebeCassini();
    //_loadULBCampus();
    //_loadOgreMaxScene("../../media/scenes/amphimath/", "amphimath.scene");
    //_loadOgreMaxScene("../../media/scenes/pleyel/", "amphi.scene");
}
//-----------------------------------------------------------------------------
void SSAOApp::_loadHebeCassini()
{
    _buildRoom();

    const int n=3;
    const int spacing = 30;
    for(int i=0 ; i<n ; i++)
    {
        for (int j=0 ; j<n ; j++)
        {
            _loadMesh("Box01", Ogre::Vector3(i*spacing, 0.1, j*spacing));
        }
    }

    _loadMesh("ogrehead", Ogre::Vector3(-50, 20, 0));


    Ogre::Vector3 cassiniPosition = Ogre::Vector3(-300, 100, 0);
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


    Ogre::SceneNode *node= _loadMesh("hebemissin", Ogre::Vector3(0, -0.19, 200));
    node->scale(20, 20, 20);

    node = _loadMesh("dragon", Ogre::Vector3(200, 35, 200));
    node->roll(Ogre::Radian(TAU / 2));
    node->scale(50, 50, 50);

                                             
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
void SSAOApp::_loadULBCampus()
{ using namespace Ogre;

    _loadMesh("ulb_building_BATIMENTS_EST", Vector3(-1300, 0, 0));

    _loadMesh("ulb_building_bat_C", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_ASCENCEUR", Vector3(-1300, 0, 0));


    _loadMesh("ulb_building_BAT_NEXT", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_BAT_PRINCIPAL", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_bat_projet_broullion", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_bat_S_W", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_batiment_plus", Vector3(-1300, 0, 0));


    _loadMesh("ulb_building_BATIMENTS_PROJET", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_BATIMENTS_TOUT", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_Calque1", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_Calque2", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_COURBES", Vector3(-1300, 0, 0));

    _loadMesh("ulb_building_galleries", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_Layer0", Vector3(-1300, 0, 0));

    _loadMesh("ulb_building_Le_Batiment", Vector3(-1300, 0, 0));
    //_loadMesh("ulb_building_route_relief", Vector3(-1300, 0, 0));

    _loadMesh("ulb_building_tour", Vector3(-1300, 0, 0));
    //_loadMesh("ulb_building_Z_LAYOUT", Vector3(-1300, 0, 0));
}
//-----------------------------------------------------------------------------
void SSAOApp::_loadSponzaAtrium()
{

    _loadSponzaMesh("arcs_01");
    _loadSponzaMesh("arcs_02");
    _loadSponzaMesh("arcs_03");
    _loadSponzaMesh("arcs_04");
    _loadSponzaMesh("arcs_floo0");
    _loadSponzaMesh("arcs_floor");
    _loadSponzaMesh("arcs_long");
    _loadSponzaMesh("arcs_small");
    _loadSponzaMesh("ceiling");
    _loadSponzaMesh("doors");
    _loadSponzaMesh("floors");
    _loadSponzaMesh("holes");
    _loadSponzaMesh("object19");
    _loadSponzaMesh("object21");
    _loadSponzaMesh("object23");
    _loadSponzaMesh("object27");
    _loadSponzaMesh("object28");
    _loadSponzaMesh("object3");
    _loadSponzaMesh("object31");
    _loadSponzaMesh("object32");
    _loadSponzaMesh("object4");
    _loadSponzaMesh("object5");
    _loadSponzaMesh("object6");
    _loadSponzaMesh("outside01");
    _loadSponzaMesh("parapet");
    _loadSponzaMesh("pillar_cor");
    _loadSponzaMesh("pillar_flo");
    _loadSponzaMesh("pillar_qua");
    _loadSponzaMesh("pillar_rou");
    _loadSponzaMesh("puillar_fl");
    _loadSponzaMesh("relief");
    _loadSponzaMesh("round_hole");
    _loadSponzaMesh("walls");
    _loadSponzaMesh("windows");
}
//-----------------------------------------------------------------------------
void SSAOApp::_loadSponzaMesh(const std::string&_meshName)
{
    Ogre::SceneNode *node = _loadMesh(_meshName, Ogre::Vector3::ZERO);
    node->scale(50, 50, 50);
    node->pitch(Ogre::Radian(Ogre::Math::PI / 2));    
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
void SSAOApp::_loadOgreMaxScene(const std::string &_path, const std::string &_filename)
{
    OgreMax::OgreMaxScene ogreMaxScene;
    SSAOMaterialSetter materialSetter;


    ogreMaxScene.Load(
        _path + _filename
        , mWindow
        , OgreMax::OgreMaxScene::SKIP_ENVIRONMENT 
        | OgreMax::OgreMaxScene::SKIP_SCENE_CAMERA
        | OgreMax::OgreMaxScene::SKIP_SCENE_LIGHT 
        | OgreMax::OgreMaxScene::ALL_NAME_PREFIXES
        , mSceneMgr
        , mSceneMgr->getRootSceneNode()
        , &materialSetter
        , "ogremax_amphi");

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
