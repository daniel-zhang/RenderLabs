#include "ShaderLab0.h"
#include "d3dcompiler.h"

ShaderLab0::ShaderLab0()
{

}

ShaderLab0::~ShaderLab0()
{

}

bool ShaderLab0::init()
{
    if (!RenderCore::init()) return false;

    //loadPrecompiledShaders();

    mVertShader.setPipelineRef(md3dImmediateContext, md3dDevice);
    mVertShader.createFromCompiledFile(std::string("TestVS.cso"));
    mVertShader.createConstantBuffers();

    mVertShader.beginUpdate();
    mVertShader.endUpdate();


    return true;
}

void ShaderLab0::onResize()
{
    RenderCore::onResize();
}

void ShaderLab0::updateScene(float dt)
{

}

void ShaderLab0::drawScene()
{

}

void ShaderLab0::onMouseDown(WPARAM btnState, int x, int y)
{
    SetCapture(mHwnd);
}

void ShaderLab0::onMouseUp(WPARAM btnState, int x, int y)
{
    ReleaseCapture();
}

void ShaderLab0::onMouseMove(WPARAM btnState, int x, int y)
{

}

void ShaderLab0::loadPrecompiledShaders()
{
    std::vector<char> shaderCode;
    int size = 0;
    std::ifstream fin("../debug/TestVS.cso", std::ios::binary);
    if (fin)
    {
        fin.seekg(0, std::ios_base::end);
        size = (int)fin.tellg();
        fin.seekg(0, std::ios_base::beg);

        shaderCode.resize(size);
        fin.read(&shaderCode[0], size);

        fin.close();
    }
    ID3D11VertexShader* vs;
    HRESULT hr = md3dDevice->CreateVertexShader(&shaderCode[0], size, NULL, &vs);
    if (FAILED(hr))
    {
        //
    }

    ID3D11ShaderReflection* pReflector = NULL;
    D3DReflect(&shaderCode[0], size, IID_ID3D11ShaderReflection, (void**)&pReflector);

    D3D11_SHADER_DESC desc;
    pReflector->GetDesc(&desc);

    md3dImmediateContext->VSSetShader(vs, NULL, NULL);

    safe_release(&vs);
}

/*

*/
void ShaderProgram::setPipelineRef(ID3D11DeviceContext* ctx, ID3D11Device* dev)
{
    mPipeline.mCtx = ctx;
    mPipeline.mDev = dev;
}

ShaderProgram::ShaderProgram():mShaderFilePath("../debug/")
{
    mEShaderType = SHADER_TYPE_INVALID;
}

std::string& ShaderProgram::getShaderName()
{
    return mShaderFileName;
}

ShaderProgram::EShaderType ShaderProgram::getShaderType()
{
    return mEShaderType;
}

bool ShaderProgram::loadFromFile(std::string& filename)
{
    //std::ifstream fin("../debug/TestVS.cso", std::ios::binary);
    std::ifstream fin(mShaderFilePath + filename, std::ios::binary);
    if (fin)
    {
        fin.seekg(0, std::ios_base::end);
        int size = (int)fin.tellg();
        fin.seekg(0, std::ios_base::beg);

        mShaderByteCode.clear();
        mShaderByteCode.resize(size);
        fin.read(&mShaderByteCode[0], size);
        fin.close();

        mShaderFileName = filename;
        return true;
    }
    return false;
}

VertexShaderProgram::VertexShaderProgram()
{
    mEShaderType = SHADER_TYPE_VERTEX_SHADER;
    mVS = NULL;
}

VertexShaderProgram::~VertexShaderProgram()
{
    safe_release(&mVS);
}

void VertexShaderProgram::createFromCompiledFile(std::string& filename)
{
    if (mPipeline.mDev == NULL)
    {
        // throw invalid device
        return;
    }
    if (loadFromFile(filename))
    {
        HRESULT hr = mPipeline.mDev->CreateVertexShader(&mShaderByteCode[0], mShaderByteCode.size(), NULL, &mVS);
        if (FAILED(hr))
        {
            // throw failed to create vertex shader
        }
    }
    else
    {
        // throw invalid shader file
    }
}

PixelShaderProgram::PixelShaderProgram()
{
    mEShaderType = SHADER_TYPE_PIXEL_SHADER;
    mPS = NULL;
}

PixelShaderProgram::~PixelShaderProgram()
{
    safe_release(&mPS);
}

void PixelShaderProgram::createFromCompiledFile(std::string& filename)
{
    if (mPipeline.mDev == NULL) return;

    if (loadFromFile(filename))
    {
        HRESULT hr = mPipeline.mDev->CreatePixelShader(&mShaderByteCode[0], mShaderByteCode.size(), NULL, &mPS);
        if (FAILED(hr))
        {
            // throw
        }
    }
}
