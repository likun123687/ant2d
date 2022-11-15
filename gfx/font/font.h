#pragma once
#include <gfx/font/config.h>
#include <gfx/bk/texture.h>
#include <unordered_map>
#include <gfx/bk/res_manager.h>
#include <utils/debug.h>
#include <third_party/utfcpp/utf8.h>

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

    void AddGlyphs(Rune rune, Glyph g)
    {
        glyphs_[rune] = g;
    }

    void LoadTex(const ImageData &img)
    {           
        Texture2D* tex = nullptr;
        if (math::IsPow2(img.width_) && math::IsPow2(img.height_)) {
            tex_width_ = img.width_;
            tex_height_ = img.height_;
            std::tie(id_, tex) = SharedResManager.AllocTexture(img);
        } else {
            ImageData pow2_img = ImageData::Pow2Image(img);
            tex_width_ = pow2_img.width_;
            tex_height_ = pow2_img.height_;
            std::tie(id_, tex) = SharedResManager.AllocTexture(pow2_img);
        }
        if (!tex) {
            Error("load texture error");
        }        
    }

    void SetGWidth(float gw)
    {
        g_width_ = gw;
    }

    void SetGHeight(float gh)
    {
        g_height_ = gh;
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

std::tuple<int, std::string> Wrap(IFont* font, const std::string& text,
    float wrap, float font_size)
{
    auto size = text.size();
    auto line = std::vector<uint8_t> {};
    line.reserve(size);

    auto buff = std::vector<uint8_t> {};
    buff.reserve(size * 2);

    float gh = 0;
    std::tie(std::ignore, gh) = font->Bounds();
    auto scale = font_size / gh;

    auto is_valid = ::utf8::is_valid(iter, text.end());
    if (!is_valid) {
        Error("invalid utf-8 encoding string {}", std::string { text.begin(), text.end() });
    }

    for (int i = 0, w = 0; i < size;) {
        float line_size = 0;
        int last_space = -1;

        for (int j = 0; i < size; i = i + w, j = j + w) {
            auto iter = text.begin() + i;
            auto old_iter = iter;

            Rune r = ::utf8::unchecked::next(iter, text.end());
            int width = iter - old_iter;

            if (r == '\n') {
                i += w;
                j += w;
                goto NEW_LINE;
            }

            std::copy(text.begin() + i, text.begin() + i + w, line.end());
            Glyph* g = font->Glyph(r);
            if (g) {
                line_size += float(g->advance) * scale;
            } else {
                Error("error rune {}", r);
            }

            if (r == ' ' || r == '\t') {
                last_space = j;
            }

            if (line_size > wrap) {
                i += w;
                j += w;
                break;
            }
        }
        // reach the end
        if (line_size < wrap) {
            std::copy(line.begin(), line.end(), buff.end());
            n += 1;
            break;
        }

        // if has space, break! or remove last char to fit line-width
        if (last_space > 0) {
            i -= (line.size() - last_space);
            line.resize(last_space + 1);
        } else {
            i -= w;
            line.resize(line.size() - w + 1);
        }

    NEW_LINE:
        line.push_back('\n');
        std::copy(line.begin(), line.end(), buff.end());
        line.resize(0);
        n += 1;
    }

    return std::make_tuple(n, std::string(buff.begin(), buff.end()));
}

math::Vec2 CalculateTextSize(const std::string& text, IFont* font, float font_size)
{
    float gh = 0;
    std::tie(std::ignore, gh) = font->Bounds();

    auto scale = font_size / gh;
    auto size = math::Vec2 { 0, font_size };

    auto is_valid = ::utf8::is_valid(iter, text.end());
    if (!is_valid) {
        Error("invalid utf-8 encoding string {}", std::string { text.begin(), text.end() });
    }

    for (int i = 0, w = 0; i < text.size(); i += w) {
        auto iter = text.begin() + i;
        auto old_iter = iter;

        Rune r = ::utf8::unchecked::next(iter, text.end());
        int width = iter - old_iter;

        Glyph* g = font->Glyph(r);
        if (r >= 32) {
            size[0] += float(g->advance) * scale;
        }
    }
    return size;
}

}