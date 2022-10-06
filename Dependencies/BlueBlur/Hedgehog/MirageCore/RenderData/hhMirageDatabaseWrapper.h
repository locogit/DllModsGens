﻿#pragma once

#include <Hedgehog/Base/hhObject.h>
#include <Hedgehog/Base/Type/hhSharedString.h>

namespace Hedgehog::Database
{
    class CDatabase;
}

namespace Hedgehog::Mirage
{
    class CMirageDatabaseWrapper;
    class CPictureData;
    class CLightListData;
    class CVertexShaderData;
    class CPixelShaderData;
    class CModelData;

    static inline BB_FUNCTION_PTR(CMirageDatabaseWrapper*, __thiscall, fpCMirageDatabaseWrapperCtor, 0x72EE30,
        CMirageDatabaseWrapper* This, Hedgehog::Database::CDatabase* pDatabase);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCDatabaseGetPictureData, 0x72EFE0,
        CMirageDatabaseWrapper* This, boost::shared_ptr<CPictureData>& spPictureData, const Hedgehog::Base::CSharedString& name, uint32_t unknown);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCDatabaseGetLightListData, 0x72F6F0,
        CMirageDatabaseWrapper* This, boost::shared_ptr<CLightListData>& spLightListData, const Hedgehog::Base::CSharedString& name, uint32_t unknown);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCDatabaseGetVertexShaderData, 0x72F560,
        CMirageDatabaseWrapper* This, boost::shared_ptr<CVertexShaderData>& spVertexShaderData, const Hedgehog::Base::CSharedString& name, uint32_t unknown);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCDatabaseGetPixelShaderData, 0x72F4F0,
        CMirageDatabaseWrapper* This, boost::shared_ptr<CPixelShaderData>& spPixelShaderData, const Hedgehog::Base::CSharedString& name, uint32_t unknown);
   
    static inline BB_FUNCTION_PTR(void, __thiscall, fpCDatabaseGetModelData, 0x72F810,
        CMirageDatabaseWrapper* This, boost::shared_ptr<CModelData>& spModelData, const Hedgehog::Base::CSharedString& name, uint32_t unknown);

    class CMirageDatabaseWrapper : public Hedgehog::Base::CObject
    {
    public:
        Hedgehog::Database::CDatabase* m_pDatabase;
        bool m_Flag;

        CMirageDatabaseWrapper(Hedgehog::Database::CDatabase* pDatabase)
        {
            fpCMirageDatabaseWrapperCtor(this, pDatabase);
        }

        virtual ~CMirageDatabaseWrapper() = default;

        void GetPictureData(boost::shared_ptr<CPictureData>& spPictureData, const Base::CSharedString& name, uint32_t unknown)
        {
            fpCDatabaseGetPictureData(this, spPictureData, name, unknown);
        }

        void GetLightListData(boost::shared_ptr<CLightListData>& spLightListData, const Base::CSharedString& name, uint32_t unknown)
        {
            fpCDatabaseGetLightListData(this, spLightListData, name, unknown);
        }

        void GetVertexShaderData(boost::shared_ptr<CVertexShaderData>& spVertexShaderData, const Base::CSharedString& name, uint32_t unknown)
        {
            fpCDatabaseGetVertexShaderData(this, spVertexShaderData, name, unknown);
        }

        void GetPixelShaderData(boost::shared_ptr<CPixelShaderData>& spPixelShaderData, const Base::CSharedString& name, uint32_t unknown)
        {
            fpCDatabaseGetPixelShaderData(this, spPixelShaderData, name, unknown);
        }     
        
        void GetModelData(boost::shared_ptr<CModelData>& spModelData, const Base::CSharedString& name, uint32_t unknown)
        {
            fpCDatabaseGetModelData(this, spModelData, name, unknown);
        }
    };

    BB_ASSERT_OFFSETOF(CMirageDatabaseWrapper, m_pDatabase, 0x4);
    BB_ASSERT_OFFSETOF(CMirageDatabaseWrapper, m_Flag, 0x8);
    BB_ASSERT_SIZEOF(CMirageDatabaseWrapper, 0xC);
}