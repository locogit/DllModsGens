#pragma once

#include <Hedgehog/MirageCore/MatrixNode/hhMatrixNodeListener.h>
#include <Sonic/System/GameObject.h>

namespace Havok
{
    class Shape;
}

namespace Hedgehog::Database
{
    class CDatabase;
}

namespace Sonic
{
    class CGameObject3D;
    class CMatrixNodeTransform;
    class CRigidBody;
    class CEventCollisionHolder;

    static inline BB_FUNCTION_PTR(CGameObject3D*, __stdcall, fpCGameObject3DCtor, 0xD5DAC0, CGameObject3D* This);

    static inline BB_FUNCTION_PTR(void, __stdcall, fpCGameObject3DSetPosition, 0xD5CE10,
        CGameObject3D* This, const Hedgehog::Math::CVector& position);

    static inline BB_FUNCTION_PTR(bool, __stdcall, fpCGameObject3DAddRigidBody, 0xE98BD0,
        const boost::shared_ptr<Sonic::CRigidBody>&, Sonic::CGameObject3D* This, Havok::Shape*, int, boost::shared_ptr<Hedgehog::Mirage::CMatrixNode>);

    static inline BB_FUNCTION_PTR(void*, __stdcall, fpCGameObject3DAddEventColliderA, 0xD5E090,
        CGameObject3D* This, const Hedgehog::Base::CStringSymbol& symbol, Havok::Shape** ppShape, int mask, bool isOffset, bool isTrigger);

    static inline BB_FUNCTION_PTR(void*, __stdcall, fpCGameObject3DAddEventColliderB, 0xD5DC90,
        CGameObject3D* This, const Hedgehog::Base::CStringSymbol& symbol, Havok::Shape** ppShape, int mask, bool isTrigger, boost::shared_ptr<Hedgehog::Mirage::CMatrixNode> node);

    // An optimized function takes shared pointers on the stack, so we have to be a bit... tricky.

    static constexpr int pAddRigidBodyFromDatabase = 0x00E98EB0;
    static void __declspec(naked) __declspec(noinline) asmCGameObject3DAddRigidBodyFromDatabase()
    {
        __asm
        {
            mov eax, ecx
            jmp pAddRigidBodyFromDatabase
        }
    }

    static inline BB_FUNCTION_PTR(bool, __thiscall, fpCGameObject3DAddRigidBodyFromDatabase, asmCGameObject3DAddRigidBodyFromDatabase,
        const char* containerName, const boost::shared_ptr<CRigidBody>& spRigidBody, Sonic::CGameObject3D* This,
        const char* shapeName, int colID, boost::shared_ptr<Hedgehog::Mirage::CMatrixNode> spMatrixNodeSingleElement,
        boost::shared_ptr<Hedgehog::Database::CDatabase> spDatabase);


    // Skyth's method that doesn't actually work, to-be explored
    /*
    static uint32_t pCGameObject3DAddRigidBodyFromDatabase = 0xE98EB0;

    static bool __declspec(naked) __declspec(noinline) __fastcall
        fCGameObject3DAddRigidBodyFromDatabase(const char* containerName, void* unused_edx, const boost::shared_ptr<Sonic::CRigidBody>& spRigidBody, Sonic::CGameObject3D* This,
        const char* shapeName, int colID, boost::shared_ptr<Hedgehog::Mirage::CMatrixNode> spMatrixNodeSingleElement,
        boost::shared_ptr<Hedgehog::Database::CDatabase> spDatabase) noexcept
    {
        __asm
        {
            mov eax, ecx
            jmp[fCGameObject3DAddRigidBodyFromDatabase]
        }
    }
    */

    class CGameObject3D : public CGameObject, public Hedgehog::Mirage::CMatrixNodeListener
    {
    public:
        BB_INSERT_PADDING(0x8);
        boost::shared_ptr<CMatrixNodeTransform> m_spMatrixNodeTransform;
        boost::shared_ptr<CEventCollisionHolder> m_spEventCollisionHolder;
        BB_INSERT_PADDING(0x2C);

        CGameObject3D(const bb_null_ctor&) : CGameObject(bb_null_ctor{}), CMatrixNodeListener(bb_null_ctor{}) {}

        CGameObject3D() : CGameObject3D(bb_null_ctor{})
        {
            fpCGameObject3DCtor(this);
        }

        virtual ~CGameObject3D();

        BB_OVERRIDE_FUNCTION_PTR(void, CGameObject, AddCallback, 0xD5CB80, (const Hedgehog::Base::THolder<CWorld>&, worldHolder),
            (Sonic::CGameDocument*, pGameDocument), (const boost::shared_ptr<Hedgehog::Database::CDatabase>&, spDatabase))

            BB_OVERRIDE_FUNCTION_PTR(void, CGameObject, RemoveCallback, 0xD5C770, (Sonic::CGameDocument*, pGameDocument))

            BB_OVERRIDE_FUNCTION_PTR(void, CGameObject, CGameObject24, 0xD5D540, (void*, gameplayFlowManager))

            BB_OVERRIDE_FUNCTION_PTR(bool, CMatrixNodeListener, MatrixNodeChangedCallback, 0xD5C780,
                (const Hedgehog::Math::CMatrix&, matrix), (size_t, flags))

            BB_VIRTUAL_FUNCTION_PTR(void, SetModelVisibility, 0xD5D6D0, (bool, isVisible))
            BB_VIRTUAL_FUNCTION_PTR(void*, CGameObject3DVtable38, 0xD5D200, (void*, A1), (void*, A2), (void*, A3), (void*, A4))
            BB_VIRTUAL_FUNCTION_PTR(void*, CGameObject3DVtable3C, 0xD5CCB0, (void*, A1), (void*, A2), (void*, A3))
            BB_VIRTUAL_FUNCTION_PTR(void*, CGameObject3DVtable40, 0xD5CCE0, (void*, A1), (void*, A2))
            BB_VIRTUAL_FUNCTION_PTR(void*, CGameObject3DVtable44, 0xD5D3A0, (void*, A1), (void*, A2), (void*, A3))
            BB_VIRTUAL_FUNCTION_PTR(void*, CGameObject3DVtable48, 0xD5CD10, (void*, A1), (void*, A2))
            BB_VIRTUAL_FUNCTION_PTR(void*, CGameObject3DVtable4C, 0xD5CBF0, (void*, A1), (void*, A2), (void*, A3))
            BB_VIRTUAL_FUNCTION_PTR(void*, CGameObject3DVtable50, 0xD5CC20, (void*, A1), (void*, A2))
            BB_VIRTUAL_FUNCTION_PTR(void*, CGameObject3DVtable54, 0xD5CC50, (void*, A1), (void*, A2), (void*, A3))
            BB_VIRTUAL_FUNCTION_PTR(void*, CGameObject3DVtable58, 0xD5CC80, (void*, A1), (void*, A2))

            void SetPosition(const Hedgehog::Math::CVector& position)
        {
            fpCGameObject3DSetPosition(this, position);
        }

        // Adds a CRigidBody to this game, attached to a MatrixNode.
        // Requires a hkShape to be made first, and collision mask ID--see the function 0x1255FA0 for examples.
        void AddRigidBody(const boost::shared_ptr<CRigidBody>& rigidBody, Havok::Shape* shape, int collisionID, const boost::shared_ptr<Hedgehog::Mirage::CMatrixNode>& matrixNode)
        {
            fpCGameObject3DAddRigidBody(rigidBody, this, shape, collisionID, matrixNode);
        }

        // Adds a CRigidBody to this game, attached to a MatrixNode.
        // Takes in the name of a ".phy.hkx" file, which is the container for rigidbodies in the game.
        // Also takes in the name of the rigidbody from the container to use.
        bool AddRigidBody(const boost::shared_ptr<CRigidBody>& rigidBody, const char* containerName,
            const char* shapeName, int colID,
            const boost::shared_ptr<Hedgehog::Mirage::CMatrixNode>& spMatrixNode,
            const boost::shared_ptr<Hedgehog::Database::CDatabase>& spDatabase)
        {
            //return fCGameObject3DAddRigidBodyFromDatabase(containerName, rigidBody, this, shapeName, colID, spMatrixNode, spDatabase);
            return fpCGameObject3DAddRigidBodyFromDatabase(containerName, rigidBody, this, shapeName, colID, spMatrixNode, spDatabase);
        }

        void AddEventCollision(const Hedgehog::Base::CStringSymbol& symbol, Havok::Shape* shape, int collisionMask, bool isOffset, bool isContactPhantom)
        {
            fpCGameObject3DAddEventColliderA(this, symbol, &shape, collisionMask, isOffset, isContactPhantom);
        }

        void AddEventCollision(const Hedgehog::Base::CStringSymbol& symbol, Havok::Shape* shape, int collisionMask, bool isContactPhantom, const boost::shared_ptr<Hedgehog::Mirage::CMatrixNode>& node)
        {
            fpCGameObject3DAddEventColliderB(this, symbol, &shape, collisionMask, isContactPhantom, node);
        }

    };

    BB_ASSERT_OFFSETOF(CGameObject3D, m_spMatrixNodeTransform, 0xB8);
    BB_ASSERT_SIZEOF(CGameObject3D, 0xF4);
}
