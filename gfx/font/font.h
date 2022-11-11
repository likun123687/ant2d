#pragma once
#include <gfx/font/config.h>
#include <gfx/bk/texture.h>
#include <unordered_map>
#include <gfx/bk/res_manager.h>

namespace ant2d {
// Font provides all the information needed to render a Rune.
//
// Tex2D returns the low-level bk-texture.
// Glyph returns the matrix of rune.
// Bounds returns the largest width and height for any of the glyphs
// in the fontAtlas.
// SizeOf measure the text and returns the width and height.
struct IFont {
    virtual std::tuple<uint16_t, Texture2D*> Tex2D() = 0;
    virtual Glyph* Glyph(Rune rune) = 0;
    virtual std::tuple<float, float> Bounds() = 0;
    virtual std::tuple<float, float, float, float> Frame(Rune rune) = 0;
    virtual void Dispose() = 0;
};

// A fontAtlas allows rendering ofg text to an OpenGL context
class FontAtlas : public IFont {
public:
    std::tuple<uint16_t, Texture2D*> Tex2D()
    {
        Texture2D* tex = nullptr;
        auto tex = SharedResManager.GetTexture(id_);
        return std::make_tuple(id_, tex);
    }

    // implement fontAtlas-system
    Glyph* Glyph(Rune rune)
    {
        Glyph* ret = nullptr;
        auto iter = glyphs_.find(rune);
        if (iter != glyphs_.end()) {
            ret = &(it->second);
        }
        return ret;
    }

    std::tuple<float, float> Bounds()
    {
        return std::make_tuple(g_width_, g_height_);
    }

    std::tuple<float, float, float, float> Frame(Rune rune)
    {
        auto& g = glyphs_[rune];
        auto u1 = float(g.x) / tex_width_;
        auto v1 = float(g.y) / tex_height_;
        auto u2 = float(g.x + g.width) / tex_width_;
        auto v2 = float(g.y + g.height) / tex_height_;
        return std::make_tuple(u1, v1, u2, v2);
    }

    void Dispose()
    {
        SharedResManager.Free(id_);
    }

    void  AddGlyphs(Rune rune, Glyph g)
    {
        glyphs_[rune] = g;
    }

private:
    uint16_t id_;

    float tex_width_;
    float tex_height_;

    float g_width_; // Largest glyph width.
    float g_height_; // Largest glyph height.

    std::unordered_map<Rune, Glyph> glyphs_;
    uint16_t w_;
    uint16_t h_;

    std::vector<float> regions_;
};

}