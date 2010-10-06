#pragma once
#ifndef __041010_EDGEGEOMETRYBUILDER_HPP__
#define __041010_EDGEGEOMETRYBUILDER_HPP__

#include <Ogre.h>

class EdgeGeometryBuilder
{
public:
    EdgeGeometryBuilder(const Ogre::String &_name, Ogre::Log *_log, Ogre::SceneManager *_sceneMgr);
    ~EdgeGeometryBuilder();
    void begin();
    void addEdgesForEntity(Ogre::Entity *_ent);
    void end();
    void attachToSceneNode(Ogre::SceneNode*);
    void setVisible(bool _visible);
    bool isVisible(){return mEdges->isVisible();};


protected:
    void _addEdgesForEdgeGroup(const Ogre::EdgeData::EdgeGroup &_edgeGroup
                              ,const Ogre::EdgeData &edgeData);
    void _buildEdgeQuad(const Ogre::Vector3 &_v0, const Ogre::Vector3 &_v1
                       ,const Ogre::Vector4 &_nA, const Ogre::Vector4 &_nB
                       ,const Ogre::Real _markedEdge, unsigned int _idx);

    inline bool _isEdgeARidge(const Ogre::Vector3&, const Ogre::Vector3&, const Ogre::Real&);
    inline bool _isEdgeAValley(const Ogre::Vector3&, const Ogre::Vector3&, const Ogre::Real&);


private:
    EdgeGeometryBuilder(){};


protected:
    Ogre::String mName;
    bool mIsBuilding;
    Ogre::ManualObject *mEdges;
    Ogre::Log *mLog;
    Ogre::SceneManager *mSceneMgr;
    Ogre::Real mRidgeThreshold;

    int mIndex;
};

#endif 