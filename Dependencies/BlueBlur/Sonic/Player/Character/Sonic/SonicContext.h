#pragma once

#include <Sonic/Player/Character/Speed/PlayerSpeedContext.h>

namespace Sonic::Player
{
    class CSonicContext : public CPlayerSpeedContext
    {
    public:
        static constexpr CSonicContext** ms_pInstance = (CSonicContext**)0x1E5E2F8;

        static CSonicContext* GetInstance()
        {
            return *ms_pInstance;
        }

        BB_INSERT_PADDING(0x2A0);
    };

    BB_ASSERT_SIZEOF(CSonicContext, 0x14D0);
}