/*----------------------------------------------------------*/
//	Author: brianuuuSonic https://github.com/brianuuu
//	Year: 2021
//	Description: Inject new animations
/*----------------------------------------------------------*/

#pragma once
struct CAnimationStateInfo
{
    const char* m_Name;
    const char* m_FileName;
    float m_Speed;
    int32_t m_PlaybackType;
    int32_t field10;
    float field14;
    float field18;
    int32_t field1C;
    int32_t field20;
    int32_t field24;
    int32_t field28;
    int32_t field2C;
};

struct CAnimationStateSet
{
    CAnimationStateInfo* m_pEntries;
    size_t m_Count;
};

struct NewAnimationData
{
    char const* m_stateName;
    char const* m_fileName;
    float m_speed;
    bool m_isLoop;
    char const* m_destinationState;

    NewAnimationData
    (
        char const* _stateName,
        char const* _fileName,
        float _speed,
        bool _isLoop,
        char const* _destinationState
    )
        : m_stateName(_stateName)
        , m_fileName(_fileName)
        , m_speed(_speed)
        , m_isLoop(_isLoop)
        , m_destinationState(_destinationState)
    {}
};

class AnimationSetPatcher
{
public:
    typedef std::vector<NewAnimationData> NewAnimationDataList;
    static NewAnimationDataList m_newAnimationData;
    static NewAnimationDataList m_newAnimationDataSuper;
    static void applyPatches();

    static void initializeAnimationList(CAnimationStateInfo* pEntries, size_t const count, NewAnimationDataList const& dataList);
    static void createAnimationState(void* A2, NewAnimationDataList const& dataList);

    // New animation states
    static const char* volatile const CrawlLoop;
    static const char* volatile const CrawlEnter;
    static const char* volatile const CrawlExit;
    static const char* volatile const JumpBoardLoop;
};
