#ifndef UTIL_H
#define UTIL_H

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <d3d11_1.h>
//#include <D3Dcompiler.h>
//#include <xnamath.h>
#include <DirectXMath.h>
//#include <dxerr.h>			// To enable DXTrace(...)

#include <cassert>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

// A helper function to release a COM component
template <typename T> 
void safe_release(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        (*ppT) = NULL;
    }
}

// A debug macro for any Direct3D function that returns an HRESULT value.
//#if defined(DEBUG) || defined(_DEBUG)
#if 0
    #ifndef HR
    #define HR(x)												\
    {															\
        HRESULT hr = (x);										\
        if(FAILED(hr))											\
        DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true);	    \	
    }
    #endif
#else
    #ifndef HR
    #define HR(x) (x)
    #endif
#endif

// #define XMGLOBALCONST extern CONST __declspec(selectany)
//   1. extern so there is only one copy of the variable, and not a separate
//      private copy in each .obj.
//   2. __declspec(selectany) so that the compiler does not complain about
//      multiple definitions in a .cpp file (it can pick anyone and discard 
//      the rest because they are constant--all the same).
#ifndef XMGLOBALCONST
#define XMGLOBALCONST extern const __declspec(selectany)
#endif
namespace Colors
{
    XMGLOBALCONST DirectX::XMVECTORF32 White     = {1.0f, 1.0f, 1.0f, 1.0f};
    XMGLOBALCONST DirectX::XMVECTORF32 Black     = {0.0f, 0.0f, 0.0f, 1.0f};
    XMGLOBALCONST DirectX::XMVECTORF32 Red       = {1.0f, 0.0f, 0.0f, 1.0f};
    XMGLOBALCONST DirectX::XMVECTORF32 Green     = {0.0f, 1.0f, 0.0f, 1.0f};
    XMGLOBALCONST DirectX::XMVECTORF32 Blue      = {0.0f, 0.0f, 1.0f, 1.0f};
    XMGLOBALCONST DirectX::XMVECTORF32 Yellow    = {1.0f, 1.0f, 0.0f, 1.0f};
    XMGLOBALCONST DirectX::XMVECTORF32 Cyan      = {0.0f, 1.0f, 1.0f, 1.0f};
    XMGLOBALCONST DirectX::XMVECTORF32 Magenta   = {1.0f, 0.0f, 1.0f, 1.0f};
    XMGLOBALCONST DirectX::XMVECTORF32 Silver    = {0.75f, 0.75f, 0.75f, 1.0f};
    XMGLOBALCONST DirectX::XMVECTORF32 LightSteelBlue = {0.69f, 0.77f, 0.87f, 1.0f};
}


#endif