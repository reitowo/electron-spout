# Electron Spout

Share electron's offscreen window's frame to spout output.

It uses `paint` event and copies the frame data and send to native module, copies to D3D11Texture2D and share.

## Compile

Use `cmake-js print-configure` to get configuration, and replace the related output in:

```
-G "Visual Studio 17 2022"
-DCMAKE_JS_VERSION=7.2.1
-DCMAKE_BUILD_TYPE=Release
-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded$<$<CONFIG:Debug>:Debug>DLL
-DCMAKE_JS_INC=C:/Users/reito/.cmake-js/electron-x64/v25.2.0/include/node
-DCMAKE_JS_SRC=C:/Users/reito/AppData/Roaming/npm/node_modules/cmake-js/lib/cpp/win_delay_load_hook.cc
-DNODE_RUNTIME=electron
-DNODE_RUNTIMEVERSION=25.2.0
-DNODE_ARCH=x64
-DCMAKE_JS_LIB=C:/Users/reito/.cmake-js/electron-x64/v25.2.0/x64/node.lib
-DCMAKE_SHARED_LINKER_FLAGS=/DELAYLOAD:NODE.EXE
```

Then set these variables to CMake.
