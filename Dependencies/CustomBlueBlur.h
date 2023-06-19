#pragma once

#include <Hedgehog/Universe/Engine/hhMessage.h>

namespace Sonic::Message
{

	class MsgSetGlobalLightDirection : public Hedgehog::Universe::MessageTypeSet
	{
	public:
		HH_FND_MSG_MAKE_TYPE(0x016814CC);

		Hedgehog::Math::CVector4 m_Direction;

		MsgSetGlobalLightDirection(const Hedgehog::Math::CVector4& rotation) : m_Direction(rotation) {}
	};

	BB_ASSERT_OFFSETOF(MsgSetGlobalLightDirection, m_Direction, 0x10);
	BB_ASSERT_SIZEOF(MsgSetGlobalLightDirection, 0x20);

}namespace Sonic::Message
{

	class MsgSetGlobalLightDiffuse : public Hedgehog::Universe::MessageTypeSet
	{
	public:
		HH_FND_MSG_MAKE_TYPE(0x016814E8);

		Hedgehog::Math::CVector4 m_Direction;

		MsgSetGlobalLightDiffuse(const Hedgehog::Math::CVector4& rotation) : m_Direction(rotation) {}
	};

	BB_ASSERT_OFFSETOF(MsgSetGlobalLightDiffuse, m_Direction, 0x10);
	BB_ASSERT_SIZEOF(MsgSetGlobalLightDiffuse, 0x20);

}
namespace Sonic::Message
{
	class MsgRequestChangeModule : public Hedgehog::Universe::MessageTypeSet
	{
	public:
		HH_FND_MSG_MAKE_TYPE(0x0168120C);

		int m_Mode;

		MsgRequestChangeModule(int visible) : m_Mode(visible) {}
	};

	BB_ASSERT_OFFSETOF(MsgRequestChangeModule, m_Mode, 0x10);
	BB_ASSERT_SIZEOF(MsgRequestChangeModule, 0x14);
}
//namespace Sonic::Message
//{
//
//	class MsgRequestChangeModule : public Hedgehog::Universe::MessageTypeSet
//	{
//	public:
//		HH_FND_MSG_MAKE_TYPE(0x0168120C);
//
//		int mode;
//
//		MsgRequestChangeModule(const int& val) : mode(val) {}
//	};
//
//	BB_ASSERT_OFFSETOF(MsgRequestChangeModule, mode, 0x1);
//	BB_ASSERT_SIZEOF(MsgRequestChangeModule, 0x2);
//
//}
namespace Sonic::Message
{

	class MsgSetBGColor : public Hedgehog::Universe::MessageTypeSet
	{
	public:
		HH_FND_MSG_MAKE_TYPE(0x016AA7B8);

		Hedgehog::Math::CVector m_Direction;

		MsgSetBGColor(const Hedgehog::Math::CVector& rotation) : m_Direction(rotation) {}
	};

	BB_ASSERT_OFFSETOF(MsgSetBGColor, m_Direction, 0x10);
	BB_ASSERT_SIZEOF(MsgSetBGColor, 0x20);

}