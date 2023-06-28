//
// Created by reito on 2023/6/26.
//

#include "spout_output.h"
#include <dxgi1_2.h>

void SpoutOutput::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func =
            DefineClass(env, "SpoutOutput",
                        {InstanceAccessor("name", &SpoutOutput::NameGetter, &SpoutOutput::NameSetter),
                         InstanceMethod("updateFrame", &SpoutOutput::UpdateFrame)});

    Napi::FunctionReference *constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("SpoutOutput", func);
}

Napi::Object SpoutOutput::NewInstance(Napi::Env env, Napi::Value arg) {
    Napi::Object obj = env.GetInstanceData<Napi::FunctionReference>()->New({arg});
    return obj;
}

SpoutOutput::SpoutOutput(const Napi::CallbackInfo &info) : ObjectWrap(info) {
    auto name = info[0].As<Napi::String>();
    InitializeDevice();
    output.SetSenderName(name.Utf8Value().c_str());
    output.OpenDirectX11(device);
}

void SpoutOutput::UpdateFrame(const Napi::CallbackInfo &info) {
    auto buffer = info[0].As<Napi::Uint8Array>();
    auto size = info[1].As<Napi::Object>();
    auto width = size.Get("width").As<Napi::Number>().Uint32Value();
    auto height = size.Get("height").As<Napi::Number>().Uint32Value();

    EnsureTexture(width, height);

    if (!texture)
        return;

    D3D11_MAPPED_SUBRESOURCE mapped;
    context->Map(texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    memcpy((uint8_t *) mapped.pData, buffer.Data(), buffer.ByteLength());
    context->Unmap(texture, 0);

    output.SendTexture(texture);
}

Napi::Value SpoutOutput::NameGetter(const Napi::CallbackInfo &info) {
    return Napi::String::New(info.Env(), output.GetSenderName());
}
void SpoutOutput::NameSetter(const Napi::CallbackInfo &info, const Napi::Value &value) {
    auto name = value.As<Napi::String>();
    output.SetSenderName(name.Utf8Value().c_str());
}

SpoutOutput::~SpoutOutput() {
    output.ReleaseSender();
    output.CloseDirectX11();
    if (texture)
        texture->Release();
    context->Release();
    device->Release();
}
void SpoutOutput::EnsureTexture(int width, int height) {
    if (texWidth == width && texHeight == height)
        return;
    texWidth = width;
    texHeight = height;

    if (texture)
        texture->Release();

    D3D11_TEXTURE2D_DESC texDesc_rgba;
    ZeroMemory(&texDesc_rgba, sizeof(texDesc_rgba));
    texDesc_rgba.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    texDesc_rgba.Width = width;
    texDesc_rgba.Height = height;
    texDesc_rgba.ArraySize = 1;
    texDesc_rgba.MipLevels = 1;
    texDesc_rgba.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc_rgba.Usage = D3D11_USAGE_DYNAMIC;
    texDesc_rgba.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    texDesc_rgba.SampleDesc.Count = 1;
    texDesc_rgba.SampleDesc.Quality = 0;
    texDesc_rgba.MiscFlags = 0;

    auto hr = device->CreateTexture2D(&texDesc_rgba, nullptr, &texture);
    if (FAILED(hr)) {
        return;
    }
}

void SpoutOutput::InitializeDevice() {
    HRESULT hr;

    // Feature levels supported
    D3D_FEATURE_LEVEL FeatureLevels[] = {
            D3D_FEATURE_LEVEL_11_0,
    };
    UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);
    D3D_FEATURE_LEVEL FeatureLevel;
    // This flag adds support for surfaces with a different color channel ordering
    // than the default. It is required for compatibility with Direct2D.
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;

    // We need dxgi to share texture
    IDXGIFactory2 *pDXGIFactory;
    IDXGIAdapter *pAdapter = NULL;
    hr = CreateDXGIFactory(IID_IDXGIFactory2, (void **) &pDXGIFactory);
    if (FAILED(hr))
        return;

    hr = pDXGIFactory->EnumAdapters(0, &pAdapter);
    if (FAILED(hr))
        return;

    hr = D3D11CreateDevice(pAdapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr, creationFlags, FeatureLevels, NumFeatureLevels,
                           D3D11_SDK_VERSION, &device, &FeatureLevel, &context);
    if (FAILED(hr))
        return;

    pDXGIFactory->Release();
    pAdapter->Release();
}
