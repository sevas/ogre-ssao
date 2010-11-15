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
    :OgreApplication("ULB_Building_viz", CT_MOUSE)
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


    //_loadMesh("ulb_building_BATIMENTS_EST", Vector3(-1300, 0, 0));

    //_loadMesh("ulb_building_bat_C", Vector3(-1300, 0, 0));
    //_loadMesh("ulb_building_ASCENCEUR", Vector3(-1300, 0, 0));

 
    //_loadMesh("ulb_building_BAT_NEXT", Vector3(-1300, 0, 0));
    //_loadMesh("ulb_building_BAT_PRINCIPAL", Vector3(-1300, 0, 0));
    //_loadMesh("ulb_building_bat_projet_broullion", Vector3(-1300, 0, 0));
    //_loadMesh("ulb_building_bat_S_W", Vector3(-1300, 0, 0));
    //_loadMesh("ulb_building_batiment_plus", Vector3(-1300, 0, 0));


    //_loadMesh("ulb_building_BATIMENTS_PROJET", Vector3(-1300, 0, 0));
    //_loadMesh("ulb_building_BATIMENTS_TOUT", Vector3(-1300, 0, 0));
    //_loadMesh("ulb_building_Calque1", Vector3(-1300, 0, 0));
    //_loadMesh("ulb_building_Calque2", Vector3(-1300, 0, 0));
    //_loadMesh("ulb_building_COURBES", Vector3(-1300, 0, 0));

    //_loadMesh("ulb_building_galleries", Vector3(-1300, 0, 0));
    //_loadMesh("ulb_building_Layer0", Vector3(-1300, 0, 0));

    //_loadMesh("ulb_building_Le_Batiment", Vector3(-1300, 0, 0));
    ////_loadMesh("ulb_building_route_relief", Vector3(-1300, 0, 0));

    //_loadMesh("ulb_building_tour", Vector3(-1300, 0, 0));
    ////_loadMesh("ulb_building_Z_LAYOUT", Vector3(-1300, 0, 0));



    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0.0f);
    Ogre::MeshPtr planeMesh = 
        Ogre::MeshManager::getSingleton().createPlane("ground"
                                                     ,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
                                                     ,plane
                                                     , 1000, 1000
                                                     , 1, 1, true
                                                     , 1, 5, 5
                                                     , Ogre::Vector3::UNIT_Z );
    Ogre::Entity *ent = mSceneMgr->createEntity("ground", "ground");
    mSceneMgr->getRootSceneNode()->attachObject(ent);
    ent->setMaterialName("SSAO/DiffuseLight_GBuffer");

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


    //_loadMesh("cassini_10", Vector3(-200, 100, 0));
    //_loadMesh("cassini_11", Vector3(-200, 100, 0));
    //_loadMesh("cassini_13", Vector3(-200, 100, 0));
    //_loadMesh("cassini_14", Vector3(-200, 100, 0));
    //_loadMesh("cassini_15", Vector3(-200, 100, 0));
    //_loadMesh("cassini_a1", Vector3(-200, 100, 0));
    //_loadMesh("cassini_a2", Vector3(-200, 100, 0));
    //_loadMesh("cassini_a4", Vector3(-200, 100, 0));
    //_loadMesh("cassini_a5", Vector3(-200, 100, 0));
    //_loadMesh("cassini_a6", Vector3(-200, 100, 0));
    //_loadMesh("cassini_a7", Vector3(-200, 100, 0));
    //_loadMesh("cassini_a8", Vector3(-200, 100, 0));
    //_loadMesh("cassini_as", Vector3(-200, 100, 0));




    //_loadMesh("athene", Vector3(-150, 70, 0));
    
    //Ogre::SceneNode *node = _loadMesh("mikki", Vector3::ZERO);
    //node->scale(10, 10, 10);


}
//-----------------------------------------------------------------------------
Ogre::SceneNode* SSAOApp::_loadMesh(const Ogre::String &_name, const Ogre::Vector3 &_pos)
{
    std::string entityName = _name+Ogre::StringConverter::toString(mScenePairs.size());

    Ogre::Entity *ent = mSceneMgr->createEntity(entityName, _name+".mesh");
    Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode(ent->getName()+"Node", _pos);

    //ent->setMaterialName("blue");
    ent->setMaterialName("SSAO/DiffuseLight_GBuffer");
    node->attachObject(ent);

    mScenePairs.push_back(SSAOApp::ScenePair(ent, node));

    return node;
}
//-----------------------------------------------------------------------------
void SSAOApp::_createLight()
{
    //mBBset = mSceneMgr->createBillboardSet("Light BB");
    //mBBset->setMaterialName("Objects/Flare");
    //mLightFlare = mBBset->createBillboard(Ogre::Vector3::ZERO);

    mLight = mSceneMgr->createLight("main light");
    mLight->setType(Ogre::Light::LT_POINT);
    mLight->setDiffuseColour(Ogre::ColourValue::White);
    //mLight->setSpecularColour(Ogre::ColourValue::White);

    mLightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("light node");
    mLightNode->attachObject(mLight);
    //mLightNode->attachObject(mBBset);

    mLightNode->setPosition(Ogre::Vector3(200, 200, 0));

}
//-----------------------------------------------------------------------------
