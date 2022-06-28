#pragma once
#
#include "BaseMesh.h"
#include "gameobject.h"
#include "lodepng.h"
#include "gamegrid.h"
#include <vector>
#include <iostream>

typedef unsigned int GLuint;
class MobObject : public GameObject
{
private:
    GLuint texture, textSpecular, textDiffuse;
    void importMesh();

    // Struct made for holding position on GamePath
    // This model allows for easy calculation of feature positions 
    
    using GamePosition = glm::vec3;
    using PathIndex = int;
    // Segment is a triple (i - 1, i, i + 1) where i is a PathIndex
    using SegmentIndex = int;

    struct MobPosition { 
        SegmentIndex segment;
        float timeLeft;
    };

    static constexpr float EPS = 1e-9;

    GameGrid * gameGrid;
    MobPosition currentPos;
    float timePerSegment = 1.0f;
    bool active = true;
    
    // Vector in model space 
    static constexpr float modelHeight = 1.0f;
    static constexpr MobPosition startingPosition = { 0, 0.5f };
    
public:

    inline void restart() {
        currentPos = startingPosition;
    }

    inline bool finished() const {
        return reachedPathExit(currentPos);
    }

    inline bool activate() { active  = true; }
    inline bool deactivate() { active = false; } 
    
    inline PathIndex translateToPathIndex(SegmentIndex idx) const { return idx * 2 + 1; }
    inline SegmentIndex translateToSegmentIndex(PathIndex idx) const { return (idx - 1) / 2; }

    inline int getPathSegments() const { return translateToSegmentIndex(gameGrid->gamePathSize()); }
    bool reachedPathExit(MobPosition pos) const { return pos.segment >= getPathSegments() || (pos.segment + 1 == getPathSegments() && pos.timeLeft > 0.5f); }
    
    MobPosition advanceInTime(MobPosition curr, float time);
    GamePosition getModelHitCoordinates(float afterTime);
    GamePosition translateToGamePosition(MobPosition pos);

    std::vector<BaseMesh> meshes;
    MobObject(GameGrid * gameGridPointer);
    ~MobObject();
    void Draw(const Camera& camera) const override;
    void Update(double deltaTime)  override;
    void loadModel(std::string filename);
    GLuint readTexture(const char* filename);
};

