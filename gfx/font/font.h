#pragma once
#include <gfx/bk/texture.h>
#include <unordered_map>
#include <gfx/bk/res_manager.h>
#include <utils/debug.h>
#include <tuple>
#include <gfx/font/glpy.h>

namespace ant2d {
namespace font {
    // Font provides all the information needed to render a uint32_t.
    //
    // Tex2D returns the low-level bk-texture.
    // Glyph returns the matrix of rune.
    // Bounds returns the largest width and height for any of the glyphs
    // in the fontAtlas.
    // SizeOf measure the text and returns the width and height.

    // struct IFont {
    //     virtual std::tuple<uint16_t, Texture2D*> Tex2D() = 0;
    //     virtual Glyph* GetGlyph(uint32_t rune) = 0;
    //     virtual std::tuple<float, float> Bounds() = 0;
    //     virtual std::tuple<float, float, float, float> Frame(uint32_t rune) = 0;
    //     virtual void Dispose() = 0;
    // };

    // A fontAtlas allows rendering ofg text to an OpenGL context
    class FontAtlas {
    public:
        std::tuple<uint16_t, Texture2D*> Tex2D();

        // implement fontAtlas-system
        Glyph* GetGlyph(uint32_t rune);
        std::tuple<float, float> Bounds();
        std::tuple<float, float, float, float> Frame(uint32_t rune);

        void Dispose();
        void AddGlyphs(uint32_t rune, Glyph g);
        void LoadTex(const ImageData& img);

        void SetGWidth(float gw);
        void SetGHeight(float gh);
        void SetGlyphs(std::unordered_map<uint32_t, Glyph> glyphs);

    private:
        uint16_t id_;

        float tex_width_;
        float tex_height_;

        float g_width_; // Largest glyph width.
        float g_height_; // Largest glyph height.

        std::unordered_map<uint32_t, Glyph> glyphs_;
        uint16_t w_;
        uint16_t h_;

        std::vector<float> regions_;
    };

    std::tuple<int, std::string> Wrap(FontAtlas* font, const std::string& text,
        float wrap, float font_size);

    math::Vec2 CalculateTextSize(const std::string& text, FontAtlas* font, float font_size);

}
}