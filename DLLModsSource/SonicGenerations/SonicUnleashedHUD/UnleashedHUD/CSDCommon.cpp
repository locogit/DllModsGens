bool IsAnimDone(Chao::CSD::CScene* scene)
{
	if (scene->m_MotionSpeed < 0.0f)
	{
		if ((scene->m_MotionStartFrame + 0.001000000047497451f) >= scene->m_MotionFrame)
			return true;
	}
	else
	{
		float v9 = scene->m_MotionFrame;
		if (v9 >= (scene->m_MotionEndFrame - 0.001000000047497451f))
			return true;
	}
	return false;
}

void CSDCommon::CheckSceneAnimation(int i, Chao::CSD::CScene* scene)
{
	Chao::CSD::CScene* sceneB;
	if (i != -1)
		sceneB = CSDCommon::scenesPlayingBack[i];
	else
		sceneB = scene;
	if (!sceneB)
	{
		auto it = std::find(CSDCommon::scenesPlayingBack.begin(), CSDCommon::scenesPlayingBack.end(), scene);
		if (it != CSDCommon::scenesPlayingBack.end())CSDCommon::scenesPlayingBack.erase(it);
		return;
	}
	else
	{
		Common::ClampFloat(CSDCommon::scenesPlayingBack[i]->m_MotionFrame, 0, CSDCommon::scenesPlayingBack[i]->m_MotionEndFrame);

		if (IsAnimDone(CSDCommon::scenesPlayingBack[i]))
		{
			CSDCommon::FreezeMotion(CSDCommon::scenesPlayingBack[i], 0);
			auto it = std::find(CSDCommon::scenesPlayingBack.begin(), CSDCommon::scenesPlayingBack.end(), scene);
			if (it != CSDCommon::scenesPlayingBack.end())CSDCommon::scenesPlayingBack.erase(it);
			return;
		}
	}
}
std::vector<Chao::CSD::CScene*> CSDCommon::scenesPlayingBack; //idk why i need to do this, symbol errors-
void CSDCommon::update()
{
	if (CSDCommon::scenesPlayingBack.size() != 0)
	{
		for (size_t i = 0; i < CSDCommon::scenesPlayingBack.size(); i += 4)
		{
			if (i < CSDCommon::scenesPlayingBack.size())
				CSDCommon::CheckSceneAnimation(i);
			if (i + 1 < CSDCommon::scenesPlayingBack.size())
				CSDCommon::CheckSceneAnimation(i + 1);
			if (i + 2 < CSDCommon::scenesPlayingBack.size())
				CSDCommon::CheckSceneAnimation(i + 2);
			if (i + 3 < CSDCommon::scenesPlayingBack.size())
				CSDCommon::CheckSceneAnimation(i + 3);

		}
	}
}

void CSDCommon::SplitTextToSeparateCasts(Chao::CSD::CScene* scene, const char* formatCastName, const char* text, int maxCharacterPerLine, int maxLines)
{
	const std::string input = std::string(text);
	std::stringstream ss(input);
	std::string line;
	std::vector<std::string> lines;
	for (std::string line; std::getline(ss, line);)
	{
		lines.push_back(line);
	}

	if (lines.empty())
		lines.emplace_back();

	char buff[32];
	for (size_t i = 0; i < maxLines; i++)
	{
		sprintf(buff, formatCastName, i);
		if (i <= lines.size() - 1)
		{
			scene->GetNode(buff)->SetText(lines.at(i).c_str());
		}
	}
}