#pragma once

#include "BaseMesh.h"
#include "gameobject.h"
#include "lodepng.h"
#include "gamegrid.h"
#include <vector>
#include <iostream>

typedef unsigned int GLuint;

class MobManager;
class Animator;
class AnimatedMesh;

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
    static constexpr float timePerSegment = 1.f;
    static constexpr float modelHeight = 1.0f;
    static constexpr MobPosition startingPosition = { 0, 0.5f };
    static constexpr float animSpeed = 0.5f;

    GameGrid * gameGrid = nullptr;
    MobPosition currentPos = startingPosition;
    bool active = false;
    int m_id = -1;
    AnimatedMesh *mesh = nullptr;
    Animator * animator = nullptr;
    GLuint tex, texSpecular;
    bool markOfDead = false;
    float timeToDie = -1.0f;
public:
    
    inline void markDead(float time) {
        markOfDead = true;
        timeToDie = time;
    }

    inline bool shootable(float time) const {
        if (!active || died() || willDie()) return false;
        else return !reachedPathExit(advanceInTime(currentPos, time));
    }
    
    inline bool willDie() const {
        return markOfDead && timeToDie > 0.0f;
    }
    
    inline bool died() const {
        return markOfDead && timeToDie <= 0.0f;
    }
    inline bool isActive() const {
        return active;
    }

    inline void restart() {
        currentPos = startingPosition;
        active = true;
        markOfDead = false;
        timeToDie = -1.0f;
    }

    inline bool finished() const {
        return reachedPathExit(currentPos);
    }

    inline void activate() { active  = true; }
    inline void deactivate() { active = false; } 
    
    inline PathIndex translateToPathIndex(SegmentIndex idx) const { return idx * 2 + 1; }
    inline SegmentIndex translateToSegmentIndex(PathIndex idx) const { return (idx - 1) / 2; }

    inline int getPathSegments() const { return translateToSegmentIndex(gameGrid->gamePathSize()); }
    bool reachedPathExit(MobPosition pos) const { return pos.segment >= getPathSegments() || (pos.segment + 1 == getPathSegments() && pos.timeLeft > 0.5f); }
    
    MobPosition advanceInTime(MobPosition curr, float time) const;
    glm::vec3 getModelHitCoordinates(float afterTime);
    GamePosition translateToGamePosition(MobPosition pos);
    
    inline int id() const { return m_id; }

    MobObject(GameGrid * gameGridPointer, int id);
    void Draw(const Camera& camera) const override;
    void Update(double deltaTime)  override;
};

