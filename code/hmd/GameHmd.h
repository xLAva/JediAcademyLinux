#ifndef GAMEHMD_H
#define GAMEHMD_H

class GameHmd
{
public:

    GameHmd();

    static GameHmd* Get();
    static void Destroy();

    void UpdateHmd(float* pitch, float* yaw, float* roll);
    bool GetOrientation(float& pitch, float& yaw, float& roll);

private:

    // disable copy constructor
    GameHmd(const GameHmd&);
    GameHmd& operator=(const GameHmd&);
    
    float mPitch;
    float mYaw;
    float mRoll;

    static GameHmd* sGameHmd;



};

#endif
