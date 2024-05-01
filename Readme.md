# Electron Spout 

[English](Readme_EN.md)

通过 OSR 输出 [Electron](https://github.com/electron/electron) 的画面至 [Spout](https://github.com/leadedge/Spout2)

其监听 `paint` 事件，并发送图像数据至本模块，随后复制进入 Spout 的 D3D11Texture2D 以分享

## 编译

1. 更新 `package.json` 中的信息，以符合你的 Electron 版本以及架构 

    ```json
    { 
      "cmake-js": {
        "runtime": "electron",
        "runtimeVersion": "30.0.1",
        "arch": "x64"
      }
    }
    ```

2. 运行 `cmake-js print-configure` 获取 CMake 选项

    ![img.png](img.png)

3. 复制这些选项并设置到 IDE 中的 CMake 选项中
   > 注意: 需要将 `-DCMAKE_MSVC_RUNTIME_LIBRARY` 设置为 `MultiThreaded$<$<CONFIG:Debug>:Debug>DLL`，因为 Spout 是静态链接的
   
   ```
   -G "Visual Studio 17 2022" -A x64
   -DCMAKE_BUILD_TYPE=Release
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
   
   > There are environment dependent values, do not directly copy the text above, and always use the value provided by the command.
   
4. 编译并复制 .node 文件使用

## 用法

```js
let win = new BrowserWindow({
   title: "MyWindow",
   webPreferences: {
      preload,
      offscreen: true,
      offscreenUseSharedTexture: true
   },
   show: false,
   transparent: true
});

const spout = require("electron_spout.node");
const osr = new spout.SpoutOutput("Electron Output");

win.webContents.setFrameRate(60);
win.webContents.on("paint", (event, dirty, image, texture) => {
   if (texture) {
      // when offscreenUseSharedTexture = true
      osr.updateTexture(texture)
   } else {
      osr.updateFrame(image.getBitmap(), image.getSize());
   }
});
```
