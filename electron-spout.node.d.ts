declare module "electron-spout" {
    export class SpoutOutput {
        constructor(name: string);

        /**
         * Updates the frame with the given buffer and size.
         * @param buffer A Uint8Array containing the frame data.
         * @param size An object with width and height properties specifying the size of the frame.
         */
        updateFrame(buffer: Uint8Array, size: { width: number; height: number }): void;

        /**
         * Updates the texture with the given size, widget type, pixel format, and shared texture handle.
         * @param size An object containing the widgetType, pixelFormat, and sharedTextureHandle.
         */
        updateTexture(size: { widgetType: string; pixelFormat: string; sharedTextureHandle: string }): void;

        /**
         * The name of spout source.
         */
        name: string;
    }
}