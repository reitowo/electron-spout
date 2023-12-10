# Electron Spout

Share [Electron](https://github.com/electron/electron)'s offscreen window's frame to [Spout](https://github.com/leadedge/Spout2) output.

It listens to `paint` event and copies the frame data, sends to native module, copies to D3D11Texture2D and share.

> It is sad, that currently there's no way to directly copy browser's GPU texture (without heavily modifying chromium). But the performance is acceptable. The `Map - Copy - Unmap` process takes about `2~4 ms` on `RTX 3070 + 13900K`

## Build

First, update the info in `package.json` according to your electron:

```json
{ 
  "cmake-js": {
    "runtime": "electron",
    "runtimeVersion": "25.8.0",
    "arch": "x64"
  }
}
```

Then, use `cmake-js print-configure` to get configuration, and set these variables to CMake.

> You'll need to update path accordingly to your own computer.

```
-G "Visual Studio 17 2022" -A x64
-DCMAKE_JS_VERSION=7.2.1
-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded$<$<CONFIG:Debug>:Debug>DLL
-DCMAKE_JS_INC=C:/Users/reito/.cmake-js/electron-x64/v25.8.0/include/node
-DCMAKE_JS_SRC=C:/Users/reito/AppData/Roaming/npm/node_modules/cmake-js/lib/cpp/win_delay_load_hook.cc
-DNODE_RUNTIME=electron
-DNODE_RUNTIMEVERSION=25.8.0
-DNODE_ARCH=x64
-DCMAKE_JS_LIB=C:/Users/reito/.cmake-js/electron-x64/v25.8.0/x64/node.lib
-DCMAKE_SHARED_LINKER_FLAGS=/DELAYLOAD:NODE.EXE
```

Build.

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
