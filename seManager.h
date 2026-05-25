#pragma once
#include "audio.h"
enum class SE
{
    Beam,
    Click,
    CoolDown,
    Damage,
    EnemyShot,
    Hit,
    KnockOut,
    Meteor,
    MeteorBurn,
    OverHeat,
    Shot,
    Count//構造内の数計測用
};

class SoundManager
{
public:
    static void Init()
    {
        // 配列確保
        for (int i = 0; i < (int)SE::Count; i++) m_SE[i] = nullptr;

        LoadSE(SE::Beam, "asset\\sound\\beam.wav");
        LoadSE(SE::Click, "asset\\sound\\click.wav");
        LoadSE(SE::CoolDown, "asset\\sound\\coolbullet.wav");
        LoadSE(SE::Damage, "asset\\sound\\damage.wav");
        LoadSE(SE::EnemyShot, "asset\\sound\\Enemyshot.wav");
        LoadSE(SE::Hit, "asset\\sound\\hit.wav");
        LoadSE(SE::KnockOut, "asset\\sound\\knockout.wav");
        LoadSE(SE::Meteor, "asset\\sound\\meteor.wav");
        LoadSE(SE::MeteorBurn, "asset\\sound\\meteorBurn.wav");
        LoadSE(SE::OverHeat, "asset\\sound\\overheat.wav");
        LoadSE(SE::Shot, "asset\\sound\\shot.wav");
    }

    static void Uninit()
    {
        for (int i = 0; i < (int)SE::Count; i++)
        {
            if (m_SE[i])
            {
                m_SE[i]->Uninit();
                delete m_SE[i];
                m_SE[i] = nullptr;
            }
        }
    }

    static void PlaySE(SE id, float volume = 1.0f)
    {
        if (m_SE[(int)id]) m_SE[(int)id]->Play(false, volume);
    }

private:
    static void LoadSE(SE id, const char* path)
    {
        m_SE[(int)id] = new Audio();
        m_SE[(int)id]->Load(path);
    }

    static Audio* m_SE[(int)SE::Count];
};