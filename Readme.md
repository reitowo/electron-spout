# Electron Spout

Share [Electron](https://github.com/electron/electron)'s offscreen window's frame to [Spout](https://github.com/leadedge/Spout2) output.

It listens to `paint` event and copies the frame data, sends to native module, copies to D3D11Texture2D and share.

It is sad, that there's no way to directly copy browser's GPU texture (without heavily modifying chromium). But the performance is just right. The `Map - Copy - Unmap` process takes about `2~4 ms` on my `RTX 3070 + 13900K`

## Build

First, update the info in `package.json` according to your electron:

```json
{ 
  "cmake-js": {
    "runtime": "electron",
    "runtimeVersion": "25.2.0",
    "arch": "x64"
  }
}
```

Then, use `cmake-js print-configure` to get configuration, and replace the related output with yours:

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

Then set these variables to CMake, and build.

## Usage

```js
let win = new BrowserWindow({
    title: "MyWindow",
    webPreferences: {
        preload,
        offscreen: true,
    }, 
    show: false,
    transparent: true
});

const spout = require("electron_spout.node");
const osr = new spout.SpoutOutput("Electron Output");

win.webContents.setFrameRate(60);
win.webContents.on("paint", (event, dirty, image) => {
    osr.updateFrame(image.getBitmap(), image.getSize());
});
```
