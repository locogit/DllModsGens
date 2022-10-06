#pragma once
class CSDCommon
{
public:

	static void PlayAnimation(Chao::CSD::CScene* pScene, const char* name, Chao::CSD::EMotionRepeatType repeatType, float motionSpeed, float startFrame, float endFrame = 0.0f, bool reEnable = false, bool reverse = false)
	{
		if (!pScene) return;
		if (reEnable)
			pScene->SetHideFlag(false);

		pScene->SetMotion(name);
		pScene->SetMotionFrame(startFrame);
		float speed = 1;
		if (startFrame == endFrame)
			endFrame = pScene->m_MotionEndFrame;
		if (reverse)
		{
			float temp = startFrame;
			startFrame = endFrame;
			endFrame = startFrame;
			speed = -speed;
		}
		if (endFrame > startFrame)
		{
			pScene->m_MotionEndFrame = endFrame;
		}
		pScene->m_MotionDisableFlag = false;
		pScene->m_MotionSpeed = speed;
		pScene->m_MotionRepeatType = repeatType;
		pScene->Update();
	}

	static void FreezeMotion(Chao::CSD::CScene* pScene)
	{
		pScene->SetMotionFrame(pScene->m_MotionEndFrame);
		pScene->m_MotionSpeed = 0.0f;
		pScene->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	}

	static void IntroAnim(Chao::CSD::RCPtr<Chao::CSD::CScene> scene)
	{
		scene->SetMotion("Intro_Anim");
		scene->SetMotionFrame(0.0f);
		scene->m_MotionSpeed = 1;
		scene->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scene->Update(0.0f);
	}

	static float lerp(float a, float b, float f)
	{
		return (a * (1.0 - f)) + (b * f);
	}
};
