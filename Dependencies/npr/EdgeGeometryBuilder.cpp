#include "precompiled.h"
#include "EdgeGeometryBuilder.hpp"


//-----------------------------------------------------------------------------
EdgeGeometryBuilder::EdgeGeometryBuilder(const Ogre::String &_name, Ogre::Log *_log, Ogre::SceneManager *_sceneMgr)
    :mName(_name)
    ,mLog(_log)
    ,mSceneMgr(_sceneMgr)
    ,mIsBuilding(false)
    ,mEdges(NULL)
    ,mRidgeThreshold(40.0f)
{
}
//-----------------------------------------------------------------------------
EdgeGeometryBuilder::~EdgeGeometryBuilder()
{
}
//-----------------------------------------------------------------------------
void EdgeGeometryBuilder::attachToSceneNode(Ogre::SceneNode *_node)
{
    _node->attachObject(mEdges);
}
//-----------------------------------------------------------------------------
void EdgeGeometryBuilder::setVisible(bool _visible)
{
#ifdef _DEBUG
    assert(mEdges);
#endif

    mEdges->setVisible(_visible);
}
//-----------------------------------------------------------------------------
void EdgeGeometryBuilder::begin()
{
    mIsBuilding = true;

    mEdges = mSceneMgr->createManualObject(mName);
    mEdges->begin("NPR/EdgeOutliner", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    mIndex = 0;

}
//-----------------------------------------------------------------------------
void EdgeGeometryBuilder::end()
{
    mEdges->end();
    mIsBuilding = false;
}
//-----------------------------------------------------------------------------
void EdgeGeometryBuilder::addEdgesForEntity(Ogre::Entity *_ent)
{
    if(mIsBuilding)
    {

        mLog->logMessage("Creating edges for entity : " + _ent->getName());
        Ogre::MeshPtr mesh = _ent->getMesh();

        {
            boost::format fmt("Mesh has %d submeshes");
            fmt % mesh->getNumSubMeshes();
            mLog->logMessage(fmt.str());
        }

        mesh->buildEdgeList();

        Ogre::EdgeData *edgeData = mesh->getEdgeList();

        {
            boost::format fmt("Generating %d edgegroups");
            fmt % edgeData->edgeGroups.size();
            mLog->logMessage(fmt.str());
        }
    
        for(int edgeGroupIdx = 0 ; edgeGroupIdx < edgeData->edgeGroups.size() ; edgeGroupIdx++)
        {
            {
                boost::format fmt("Adding edges for edgegroup[%d]");
                fmt % edgeGroupIdx ;
                mLog->logMessage(fmt.str());
            }

            Ogre::EdgeData::EdgeGroup &currentEdgeGroup = edgeData->edgeGroups[edgeGroupIdx];
            _addEdgesForEdgeGroup(currentEdgeGroup, *edgeData);

            {
                boost::format fmt("%s : Added %d edges (%d tris, %d vertices)");
                fmt % mEdges->getName() % (mIndex/6) % (mIndex/3) % (mIndex);
                mLog->logMessage(fmt.str());
            }

        }
    }
    else
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "Your card does not support vertex and fragment programs, so cannot "
            "run this application. Sorry!", 
            "createScene");


        OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED
            , "EdgeGeometryBuilder::addEdgesForEntity() was called before EdgeGeometryBuilder::begin() was"
            , "EdgeGeometryBuilder::addEdgesForEntity()");
    }
}
//-----------------------------------------------------------------------------
void EdgeGeometryBuilder::_addEdgesForEdgeGroup(const Ogre::EdgeData::EdgeGroup &_edgeGroup, const Ogre::EdgeData &edgeData)
{
    std::vector<Ogre::Vector3> vertices;
    vertices.resize(_edgeGroup.vertexData->vertexCount);

    const Ogre::VertexElement* posElem =
        _edgeGroup.vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

    Ogre::HardwareVertexBufferSharedPtr vbuf =
        _edgeGroup.vertexData->vertexBufferBinding->getBuffer(posElem->getSource());

    unsigned char* vertex =
        static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

    // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
    //  as second argument. So make it float, to avoid trouble when Ogre::Real will
    //  be comiled/typedefed as double:
    //      Ogre::Real* pReal;
    float* pReal;

    for( size_t j = 0; j < _edgeGroup.vertexData->vertexCount; ++j, vertex += vbuf->getVertexSize())
    {
        posElem->baseVertexPointerToElement(vertex, &pReal);

        Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

        vertices[j] = pt;
    }
    vbuf->unlock();






    int edgeCount = _edgeGroup.edges.size();
                                                            
    for(int i=0; i<edgeCount ; i++)
    {
        Ogre::EdgeData::Edge e = _edgeGroup.edges[i];

        Ogre::Vector3 v0, v1;
        Ogre::Vector4 nA, nB ;
        Ogre::Real markedEdge;

        if(e.degenerate)
        {
            
            v0 =  vertices[e.vertIndex[0]];
            v1 =  vertices[e.vertIndex[1]];

            nA = edgeData.triangleFaceNormals[e.triIndex[0]];
            nB = -nA;
            markedEdge = 2.0f;
        }
        else
        {
            v0 =  vertices[e.vertIndex[0]];
            v1 =  vertices[e.vertIndex[1]];

            nA = edgeData.triangleFaceNormals[e.triIndex[0]];
            nB = edgeData.triangleFaceNormals[e.triIndex[1]];

            Ogre::Real ridgeThreshold = Ogre::Degree(mRidgeThreshold).valueRadians();
            Ogre::Real valleyThreshold = Ogre::Degree(mRidgeThreshold).valueRadians();


            bool isRidge = _isEdgeARidge(Ogre::Vector3(nA.x, nA.y, nA.z)
                                        ,Ogre::Vector3(nB.x, nB.y, nB.z)
                                        ,ridgeThreshold);

            bool isValley = _isEdgeAValley(Ogre::Vector3(nA.x, nA.y, nA.z)
                                          ,Ogre::Vector3(nB.x, nB.y, nB.z)
                                          ,valleyThreshold);

            markedEdge = (isRidge || isValley) ? 1.0f : 0.0f;
        }

        {
            boost::format fmt("Adding new edge for position : (%.2f, %.2f, %.2f)  (%.2f, %.2f, %.2f)");
            fmt % v0.x % v0.y % v0.z % v1.x % v1.y % v0.z; 
            mLog->logMessage(fmt.str());
        }

        _buildEdgeQuad(v0, v1, nA, nB, markedEdge, mIndex);
        mIndex += 6;
    }
}
//-----------------------------------------------------------------------------
void EdgeGeometryBuilder::_buildEdgeQuad(  const Ogre::Vector3 &_v0, const Ogre::Vector3 &_v1
                                         , const Ogre::Vector4 &_nA,const Ogre::Vector4 &_nB
                                         , const Ogre::Real _markedEdge,unsigned int _idx)
{
    Ogre::Vector3 nA = Ogre::Vector3(_nA.x, _nA.y, _nA.z).normalisedCopy();
    Ogre::Vector3 nB = Ogre::Vector3(_nB.x, _nB.y, _nB.z).normalisedCopy();

    /* 1st tri
    1 __ 2
    | /
    |/
    3
    */
    mEdges->position(_v0);
    mEdges->normal(nA);
    mEdges->textureCoord(nB);
    mEdges->textureCoord(_markedEdge);

    mEdges->index(_idx++);

    mEdges->position(_v0);
    mEdges->normal(nB);
    mEdges->textureCoord(nA);
    mEdges->textureCoord(_markedEdge);

    mEdges->index(_idx++);

    mEdges->position(_v1);
    mEdges->normal(nA);
    mEdges->textureCoord(nB);
    mEdges->textureCoord(_markedEdge);

    mEdges->index(_idx++);

    /* 2nd tri
       6
      /|
     /_|
    4   5
    */
    mEdges->position(_v1);
    mEdges->normal(nA);
    mEdges->textureCoord(nB);
    mEdges->textureCoord(_markedEdge);

    mEdges->index(_idx++);

    mEdges->position(_v0);
    mEdges->normal(nB);
    mEdges->textureCoord(nA);
    mEdges->textureCoord(_markedEdge);

    mEdges->index(_idx++);

    mEdges->position(_v1);
    mEdges->normal(nB);
    mEdges->textureCoord(nA);
    mEdges->textureCoord(_markedEdge);

    mEdges->index(_idx++);
}

//-----------------------------------------------------------------------------
bool EdgeGeometryBuilder::_isEdgeARidge(const Ogre::Vector3 &_nA, const Ogre::Vector3 &_nB, const Ogre::Real &_threshold)
{
    Ogre::Vector3 nA_norm = Ogre::Vector3(_nA.x, _nA.y, _nA.z).normalisedCopy();
    Ogre::Vector3 nB_norm = Ogre::Vector3(_nB.x, _nB.y, _nB.z).normalisedCopy();

    return nA_norm.dotProduct(nB_norm) < Ogre::Math::Cos(_threshold);
}
//-----------------------------------------------------------------------------
bool EdgeGeometryBuilder::_isEdgeAValley(const Ogre::Vector3 &_nA, const Ogre::Vector3 &_nB, const Ogre::Real &_threshold)
{
    Ogre::Vector3 nA_norm = Ogre::Vector3(_nA.x, _nA.y, _nA.z).normalisedCopy();
    Ogre::Vector3 nB_norm = Ogre::Vector3(_nB.x, _nB.y, _nB.z).normalisedCopy();

    return nA_norm.dotProduct(nB_norm) < Ogre::Math::Cos(_threshold);
}
