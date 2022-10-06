﻿#pragma once

#include <Hedgehog/Yggdrasill/hhYggScheduler.h>

namespace Hedgehog::Mirage
{
    class CRenderingInfrastructure;
    class CVertexShaderData;
    class CPixelShaderData;
}

namespace Hedgehog::Yggdrasill
{
    class CYggAllocator;
    class CYggDevice;
    class CYggScheduler;
    class CYggSurface;
    class CYggTexture;

    struct SYggClearParams
    {
        DWORD Flags;
        D3DCOLOR Color;
        float Z;
        DWORD Stencil;
    };

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCYggDeviceSetSamplerFilter, 0x411CF0, 
        CYggDevice* This, uint32_t index, D3DTEXTUREFILTERTYPE migFilter, D3DTEXTUREFILTERTYPE magFilter, D3DTEXTUREFILTERTYPE mipFilter);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCYggDeviceSetShader, 0x415EE0, 
        CYggDevice* This, const boost::shared_ptr<Hedgehog::Mirage::CVertexShaderData>& spVertexShaderData, const boost::shared_ptr<Hedgehog::Mirage::CPixelShaderData>& spPixelShaderData);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCYggDeviceCreateTexture0, 0x7884C0, 
        CYggDevice* This, boost::shared_ptr<CYggTexture>& spTexture, float widthRatio, float heightRatio, uint32_t levels, uint32_t usage, D3DFORMAT format, D3DPOOL pool, HANDLE* pSharedHandle);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCYggDeviceCreateTexture1, 0x788450, 
        CYggDevice* This, boost::shared_ptr<CYggTexture>& spTexture, uint32_t width, uint32_t height, uint32_t levels, uint32_t usage, D3DFORMAT format, D3DPOOL pool, HANDLE* pSharedHandle);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCYggDeviceDrawQuad2D, 0x786620, 
        CYggDevice* This, float* bounds, float x, float y);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCYggDeviceUnsetSampler, 0x412120, 
        CYggDevice* This, uint32_t index);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCYggDeviceSetDepthStencil, 0x785D10, 
        CYggDevice* This, const boost::shared_ptr<CYggSurface>& spSurface);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCYggDeviceGetRenderTarget, 0x786030,
        CYggDevice* This, boost::shared_ptr<CYggSurface>& spSurface, uint32_t index);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCYggDeviceSetRenderTarget, 0x785D00, 
        CYggDevice* This, uint32_t index, const boost::shared_ptr<CYggSurface>& spSurface);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCYggDeviceSetSamplerAddressMode, 0x411CA0, 
        CYggDevice* This, uint32_t index, uint32_t value);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCYggDeviceSetTexture, 0x7865A0, 
        CYggDevice* This, uint32_t index, const boost::shared_ptr<CYggTexture>& spTexture);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCYggDeviceClear0, 0x785EE0, 
        CYggDevice* This, const SYggClearParams& params);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCYggDeviceClear1, 0x416650,
        CYggDevice* This, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCYggDeviceGetResolution, 0x785A90,
        CYggDevice* This, float widthRatio, float heightRatio, uint32_t& width, uint32_t& height);

    static inline BB_FUNCTION_PTR(void, __thiscall, fpCYggDeviceFlush, 0x786070,
        CYggDevice* This);

    class CYggDevice
    {
    public:
        Mirage::CRenderingInfrastructure* m_pRenderingInfrastructure;
        DX_PATCH::IDirect3DDevice9* m_pD3DDevice;
        CYggScheduler* m_pScheduler;
        CYggAllocator* m_pAllocator;
        BB_INSERT_PADDING(0xB8);
        uint32_t m_InternalResolutionWidth;
        uint32_t m_InternalResolutionHeight;

        void SetSamplerFilter(uint32_t index, D3DTEXTUREFILTERTYPE migFilter, D3DTEXTUREFILTERTYPE magFilter, D3DTEXTUREFILTERTYPE mipFilter)
        {
            fpCYggDeviceSetSamplerFilter(this, index, migFilter, magFilter, mipFilter);
        }

        void SetShader(const boost::shared_ptr<Mirage::CVertexShaderData>& spVertexShaderData, const boost::shared_ptr<Mirage::CPixelShaderData>& spPixelShaderData)
        {
            fpCYggDeviceSetShader(this, spVertexShaderData, spPixelShaderData);
        }

        void SetShader(const Mirage::SShaderPair& shader)
        {
            SetShader(shader.m_spVertexShader, shader.m_spPixelShader);
        }

        void CreateTexture(boost::shared_ptr<CYggTexture>& spTexture, float widthRatio, float heightRatio, uint32_t levels, uint32_t usage, D3DFORMAT format, D3DPOOL pool, HANDLE* pSharedHandle)
        {
            fpCYggDeviceCreateTexture0(this, spTexture, widthRatio, heightRatio, levels, usage, format, pool, pSharedHandle);
        }

        void CreateTexture(boost::shared_ptr<CYggTexture>& spTexture, uint32_t width, uint32_t height, uint32_t levels, uint32_t usage, D3DFORMAT format, D3DPOOL pool, HANDLE* pSharedHandle)
        {
            fpCYggDeviceCreateTexture1(this, spTexture, width, height, levels, usage, format, pool, pSharedHandle);
        }

        void DrawQuad2D(float* bounds, float x, float y)
        {
            fpCYggDeviceDrawQuad2D(this, bounds, x, y);
        }

        void UnsetSampler(uint32_t index)
        {
            fpCYggDeviceUnsetSampler(this, index);
        }

        void SetDepthStencil(const boost::shared_ptr<CYggSurface>& spSurface)
        {
            fpCYggDeviceSetDepthStencil(this, spSurface);
        }

        void UnsetDepthStencil()
        {
            SetDepthStencil(boost::shared_ptr<CYggSurface>());
        }

        void GetRenderTarget(boost::shared_ptr<CYggSurface>& spSurface, uint32_t index)
        {
            fpCYggDeviceGetRenderTarget(this, spSurface, index);
        }

        void SetRenderTarget(uint32_t index, const boost::shared_ptr<CYggSurface>& spSurface)
        {
            fpCYggDeviceSetRenderTarget(this, index, spSurface);
        }

        void UnsetRenderTarget(uint32_t index)
        {
            SetRenderTarget(index, boost::shared_ptr<CYggSurface>());
        }

        void SetSamplerAddressMode(uint32_t index, uint32_t value)
        {
            fpCYggDeviceSetSamplerAddressMode(this, index, value);
        }

        void SetTexture(uint32_t index, const boost::shared_ptr<CYggTexture>& spTexture)
        {
            fpCYggDeviceSetTexture(this, index, spTexture);
        }

        void Clear(const SYggClearParams& params)
        {
            fpCYggDeviceClear0(this, params);
        }

        void Clear(DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
        {
            fpCYggDeviceClear1(this, Flags, Color, Z, Stencil);
        }

        void GetResolution(float widthRatio, float heightRatio, uint32_t& width, uint32_t& height)
        {
            fpCYggDeviceGetResolution(this, widthRatio, heightRatio, width, height);
        }

        void Flush()
        {
            fpCYggDeviceFlush(this);
        }
    };

    BB_ASSERT_OFFSETOF(CYggDevice, m_pRenderingInfrastructure, 0x0);
    BB_ASSERT_OFFSETOF(CYggDevice, m_pD3DDevice, 0x4);
    BB_ASSERT_OFFSETOF(CYggDevice, m_pScheduler, 0x8);
    BB_ASSERT_OFFSETOF(CYggDevice, m_pAllocator, 0xC);
    BB_ASSERT_OFFSETOF(CYggDevice, m_InternalResolutionWidth, 0xC8);
    BB_ASSERT_OFFSETOF(CYggDevice, m_InternalResolutionHeight, 0xCC);
}
