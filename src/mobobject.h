#pragma once

#include "BaseMesh.h"
#include "gameobject.h"
#include "lodepng.h"
#include "gamegrid.h"
#include <vector>
#include <iostream>

typedef unsigned int GLuint;

class MobManager;

class MobObject : public GameObject
{
    friend MobManager;

private:

    using GamePosition = glm::vec3;
    using PathIndex = int;
    // Segment is a triple (i - 1, i, i + 1) where i is a PathIndex
    using SegmentIndex = int;

    struct MobPosition { 
        SegmentIndex segment;
        float timeLeft;
    };

    static constexpr float EPS = 1e-9;
    static constexpr float timePerSegment = 0.2f;
    static constexpr float modelHeight = 1.0f;
    static constexpr MobPosition startingPosition = { 0, 0.5f };

    GameGrid * gameGrid = nullptr;
    MobPosition currentPos = startingPosition;
    bool active = false;
    int m_id = -1;
    BaseMesh *mesh = nullptr;
    GLuint tex, texSpecular;
    
public:

    inline void restart() {
        currentPos = startingPosition;
        active = true;
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
    
    inline int id() const { return m_id; }

    MobObject(GameGrid * gameGridPointer, int id);
    void Draw(const Camera& camera) const override;
    void Update(double deltaTime)  override;
};

