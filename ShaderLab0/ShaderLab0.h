#ifndef SHADER_LAB_0_H
#define SHADER_LAB_0_H

#include "RenderCore.h"
#include <fstream>
#include <vector>

class PipelineRef
{
public:
    PipelineRef()
    {
        mCtx = NULL;
        mDev = NULL;
    }
    PipelineRef(ID3D11Device* dev, ID3D11DeviceContext* ctx)
    {
        mCtx = ctx;
        mDev = dev;
    }

    ID3D11DeviceContext* mCtx;
    ID3D11Device* mDev;
};

class ShaderProgram
{
public:
    enum EShaderType
    {
        SHADER_TYPE_VERTEX_SHADER = 0,
        SHADER_TYPE_PIXEL_SHADER,
        SHADER_TYEP_COMPUTE_SHADER,
        SHADER_TYPE_INVALID,
    };

    ShaderProgram();

    void setPipelineRef(ID3D11DeviceContext* ctx, ID3D11Device* dev);
    virtual void createFromCompiledFile(std::string& filename) = 0;
    std::string& getShaderName();
    EShaderType getShaderType();

protected:
    bool loadFromFile(std::string& filename);

protected:
    EShaderType mEShaderType;
    std::vector<char> mShaderByteCode;
    const std::string mShaderFilePath;
    std::string mShaderFileName;

    PipelineRef mPipeline;
};

class VertexShaderProgram : public ShaderProgram
{
public:
    VertexShaderProgram();
    ~VertexShaderProgram();
    virtual void createFromCompiledFile(std::string& filename);

protected:
    ID3D11VertexShader* mVS;
};

class PixelShaderProgram : public ShaderProgram
{
public:
    PixelShaderProgram();
    ~PixelShaderProgram();
    virtual void createFromCompiledFile(std::string& filename);

protected:
    ID3D11PixelShader* mPS;
};

class DirLight
{
public:
    DirectX::XMFLOAT4 mAmbient;
    DirectX::XMFLOAT4 mDiffuse;
    DirectX::XMFLOAT4 mSpecular;
    DirectX::XMFLOAT3 mDirection;
    float padding;
};

class Material
{
public:
    DirectX::XMFLOAT4 mAmbient;
    DirectX::XMFLOAT4 mDiffuse;
    DirectX::XMFLOAT4 mSpecular;
    DirectX::XMFLOAT4 mReflect;
};

/*
A pipeline stage (i.e. vs, ps, etc) has 4 kinds of inputs:
- constant buffer
- sampler
- shader resource
- shader program
*/

struct PerFrameCB
{
    DirectX::XMFLOAT3 mEyePosW;
    float padding;
    DirLight mLight;
};

struct PerObjCB
{
    DirectX::XMFLOAT4X4 mLocal;
    DirectX::XMFLOAT4X4 mWorld;
    DirectX::XMFLOAT4X4 mView;
    Material mMat;
};

class DefaultVS : public VertexShaderProgram
{
public:
    bool createConstantBuffers()
    {
        //
        //
        //
        D3D11_BUFFER_DESC bd;
        bd.ByteWidth = sizeof(PerObjCB);
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&mPerObjCB, sizeof(PerObjCB));
        InitData.pSysMem = &mPerObjCB;

        mpCBuffer0 = NULL;
        HRESULT hr = mPipeline.mDev->CreateBuffer(&bd, &InitData, &mpCBuffer0);
        if (FAILED(hr))
        {
            return false;
        }

        //
        //
        //
        mPerFrameCB.mEyePosW          = DirectX::XMFLOAT3(1.f, 2.f, 3.f);
        mPerFrameCB.mLight.mAmbient   = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.f);
        mPerFrameCB.mLight.mDiffuse   = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.f);
        mPerFrameCB.mLight.mSpecular  = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.f );
        mPerFrameCB.mLight.mDirection = DirectX::XMFLOAT3(0.f, 0.f, 0.f);

        bd.ByteWidth = sizeof(PerFrameCB);
        InitData.pSysMem = &mPerFrameCB;
        hr = mPipeline.mDev->CreateBuffer(&bd, &InitData, &mpCBuffer1);
        if (FAILED(hr))
        {
            return false;
        }

        mPipeline.mCtx->VSSetConstantBuffers(0, 1, &(mpCBuffer0));
        mPipeline.mCtx->VSSetConstantBuffers(1, 1, &(mpCBuffer1));

        return true;
    }
    void beginUpdate()
    {

    }
    void endUpdate()
    {
        D3D11_MAPPED_SUBRESOURCE mappedData;
        HRESULT hr = mPipeline.mCtx->Map(mpCBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

        PerObjCB* p = reinterpret_cast<PerObjCB*>(mappedData.pData);

        mPipeline.mCtx->Unmap(mpCBuffer0, 0);
    }

public:
    PerFrameCB mPerFrameCB;
    PerObjCB mPerObjCB;

protected:
    ID3D11Buffer* mpCBuffer0;
    ID3D11Buffer* mpCBuffer1;
};


class ShaderLab0 : public RenderCore
{
public:
    ShaderLab0();
    ~ShaderLab0();

    virtual PCWSTR getClassName() const { return L"ShaderLab0"; }
    virtual PCWSTR getWindowCaption() const { return L"ShaderLab0 MainWindow"; }

    virtual bool init();
    virtual void onResize();
    virtual void updateScene(float dt);
    virtual void drawScene();

    virtual void onMouseDown(WPARAM btnState, int x, int y);
    virtual void onMouseUp(WPARAM btnState, int x, int y);
    virtual void onMouseMove(WPARAM btnState, int x, int y);

protected:
    void loadPrecompiledShaders();

protected:
    DefaultVS mVertShader;


};

#endif