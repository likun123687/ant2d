#pragma once
#include <unordered_map>
#include <utils/debug.h>
#include <gfx/font/font.h>
#include <third_party/utfcpp/utf8/unchecked.h>

namespace ant2d {
namespace font {

    std::tuple<uint16_t, Texture2D*> FontAtlas::Tex2D()
    {
        Texture2D* tex = nullptr;
        tex = SharedResManager.GetTexture(id_);
        return std::make_tuple(id_, tex);
    }

    // implement fontAtlas-system
    Glyph* FontAtlas::GetGlyph(uint32_t rune)
    {
        Glyph* ret = nullptr;
        auto iter = glyphs_.find(rune);
        if (iter != glyphs_.end()) {
            ret = &(iter->second);
        }
        return ret;
    }

    std::tuple<float, float> FontAtlas::Bounds()
    {
        return std::make_tuple(g_width_, g_height_);
    }

    std::tuple<float, float, float, float> FontAtlas::Frame(uint32_t rune)
    {
        auto& g = glyphs_[rune];
        auto u1 = float(g.x) / tex_width_;
        auto v1 = float(g.y) / tex_height_;
        auto u2 = float(g.x + g.width) / tex_width_;
        auto v2 = float(g.y + g.height) / tex_height_;
        return std::make_tuple(u1, v1, u2, v2);
    }

    void FontAtlas::Dispose()
    {
        SharedResManager.Free(id_);
    }

    void FontAtlas::AddGlyphs(uint32_t rune, Glyph g)
    {
        glyphs_[rune] = g;
    }

    void FontAtlas::LoadTex(const ImageData& img)
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

    void FontAtlas::SetGWidth(float gw)
    {
        g_width_ = gw;
    }

    void FontAtlas::SetGHeight(float gh)
    {
        g_height_ = gh;
    }

    void FontAtlas::SetGlyphs(std::unordered_map<uint32_t, Glyph> glyphs)
    {
        glyphs_ = std::move(glyphs);
    }

    std::tuple<int, std::string> Wrap(FontAtlas* font, const std::string& text,
        float wrap, float font_size)
    {
        int n = 0;
        auto size = text.size();
        auto line = std::vector<uint8_t> {};
        line.reserve(size);

        auto buff = std::vector<uint8_t> {};
        buff.reserve(size * 2);

        float gh = 0;
        std::tie(std::ignore, gh) = font->Bounds();
        auto scale = font_size / gh;

        auto is_valid = ::utf8::is_valid(text.begin(), text.end());
        if (!is_valid) {
            Error("invalid utf-8 encoding string {}", std::string { text.begin(), text.end() });
        }

        for (int i = 0, w = 0; i < size;) {
            float line_size = 0;
            int last_space = -1;

            for (int j = 0; i < size; i += w, j += w) {
                auto iter = text.begin() + i;
                auto old_iter = iter;

                uint32_t r = ::utf8::unchecked::next(iter);
                int width = iter - old_iter;
                w = width;

                if (r == '\n') {
                    i += w;
                    j += w;
                    goto NEW_LINE;
                }

                line.insert(line.end(), text.begin() + i, text.begin() + i + w);
                Glyph* g = font->GetGlyph(r);
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
                // std::copy(line.begin(), line.end(), buff.end());
                buff.insert(buff.end(), line.begin(), line.end());
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
            buff.insert(buff.end(), line.begin(), line.end());
            line.resize(0);
            n += 1;
        }

        return { n, std::string(buff.begin(), buff.end()) };
    }

    math::Vec2 CalculateTextSize(const std::string& text, FontAtlas* font, float font_size)
    {
        float gh = 0;
        std::tie(std::ignore, gh) = font->Bounds();

        auto scale = font_size / gh;
        auto size = math::Vec2 { 0, font_size };

        auto is_valid = ::utf8::is_valid(text.begin(), text.end());
        if (!is_valid) {
            Error("invalid utf-8 encoding string {}", std::string { text.begin(), text.end() });
        }

        auto iter = text.begin();
        while (iter != text.end()) {
            uint32_t r = ::utf8::unchecked::next(iter);
            Glyph* g = font->GetGlyph(r);
            if (r >= 32) {
                size[0] += float(g->advance) * scale; 
            }
        }
        return size;
    }

}
}