#include "gfx/font/truetype/truetype.h"
#include "third_party/maxRectsBinPack/MaxRectsBinPack.h"
#include "utils/freeType/FtFont.h"
#include <string>
#include <iostream>
#include <set>

namespace ant2d {
namespace font {
namespace truetype {
    static std::vector<rbp::RectSize> GetGlyphRectangles(const Glyphs& glyphs, const std::uint32_t additionalWidth, const std::uint32_t additionalHeight, const Config& config)
    {
        std::vector<rbp::RectSize> result;
        for (const auto& kv : glyphs) {
            const auto& glyphInfo = kv.second;
            if (!glyphInfo.IsEmpty()) {
                auto width = glyphInfo.width + additionalWidth;
                auto height = glyphInfo.height + additionalHeight;
                width = ((width + config.alignment.hor - 1) / config.alignment.hor) * config.alignment.hor;
                height = ((height + config.alignment.ver - 1) / config.alignment.ver) * config.alignment.ver;
                result.emplace_back(width, height, kv.first);
            }
        }
        return result;
    }

    Glyphs CollectGlyphInfo(const ft::Font& font, const std::set<std::uint32_t>& codes)
    {
        Glyphs result;

        for (const auto& id : codes) {
            Glyph glyphInfo;

            if (font.isGlyphProvided(id)) {
                ft::Font::GlyphMetrics glyphMetrics = font.renderGlyph(nullptr, 0, 0, 0, 0, id, 0);
                glyphInfo.width = glyphMetrics.width;
                glyphInfo.height = glyphMetrics.height;
                glyphInfo.advance = glyphMetrics.horiAdvance;
                glyphInfo.xoffset = glyphMetrics.horiBearingX;
                glyphInfo.yoffset = font.ascent - glyphMetrics.horiBearingY;
                result[id] = glyphInfo;
            } else {
                std::cout << "warning: glyph " << id << " not found";
                if (id == 65279)
                    std::cout << " (it looks like Unicode byte order mark (BOM))";
                std::cout << "." << std::endl;
            }
        }

        return result;
    }

    static std::vector<Config::Size> ArrangeGlyphs(Glyphs& glyphs, const Config& config)
    {
        const auto additionalWidth = config.spacing.hor + config.padding.left + config.padding.right;
        const auto additionalHeight = config.spacing.ver + config.padding.up + config.padding.down;
        std::vector<Config::Size> result;

        auto glyphRectangles = GetGlyphRectangles(glyphs, additionalWidth, additionalHeight, config);

        rbp::MaxRectsBinPack mrbp;

        for (;;) {
            std::vector<rbp::Rect> arrangedRectangles;
            auto glyphRectanglesCopy = glyphRectangles;
            Config::Size lastSize;

            uint64_t allGlyphSquare = 0;
            for (const auto& i : glyphRectangles)
                allGlyphSquare += static_cast<uint64_t>(i.width) * i.height;
            // std::cout << "allGlyphSquare: " << allGlyphSquare << "\n";

            // int tryCount = 0;
            for (size_t i = 0; i < config.textureSizeList.size(); ++i) {
                const auto& ss = config.textureSizeList[i];
                uint64_t textureSquare = static_cast<uint64_t>(ss.w) * ss.h;
                if (textureSquare < allGlyphSquare && i + 1 < config.textureSizeList.size())
                    continue;

                // std::cout << "size: " << ss.w << " " << ss.h << ", allGlyphSquare: " << textureSquare << "\n";

                //++tryCount;

                lastSize = ss;
                glyphRectangles = glyphRectanglesCopy;

                // TODO: check workAreaW,H
                const auto workAreaW = ss.w - config.spacing.hor;
                const auto workAreaH = ss.h - config.spacing.ver;

                mrbp.Init(workAreaW, workAreaH);
                mrbp.Insert(glyphRectangles, arrangedRectangles, rbp::MaxRectsBinPack::RectBestAreaFit);

                if (glyphRectangles.empty())
                    break;
            }

            // std::cout << "tryCount: " << tryCount << "\n";

            if (arrangedRectangles.empty()) {
                if (!glyphRectangles.empty()) {
                    Error("can not fit glyphs into texture");
                }
                break;
            }

            std::uint32_t maxX = 0;
            std::uint32_t maxY = 0;
            for (const auto& r : arrangedRectangles) {
                std::uint32_t x = r.x + config.spacing.hor;
                std::uint32_t y = r.y + config.spacing.ver;

                glyphs[r.tag].x = x;
                glyphs[r.tag].y = y;

                if (maxX < x + r.width)
                    maxX = x + r.width;
                if (maxY < y + r.height)
                    maxY = y + r.height;
            }
            if (config.cropTexturesWidth)
                lastSize.w = maxX;
            if (config.cropTexturesHeight)
                lastSize.h = maxY;

            result.push_back(lastSize);
        }

        return result;
    }

    std::tuple<ImageData, uint32_t, uint32_t> RenderTextures(const Glyphs& glyphs, const Config& config, const ft::Font& font, const Config::Size& s)
    {
        // TODO:用一个不用复制的方案，提高性能
        std::vector<std::uint32_t> surface(s.w * s.h, config.color.getBGR());
        uint32_t max_width = 0;
        uint32_t max_height = 0;

        // Render every glyph
        // TODO: do not repeat same glyphs (with same index)
        for (const auto& kv : glyphs) {
            const auto& glyph = kv.second;

            if (!glyph.IsEmpty()) {
                const auto x = glyph.x + config.padding.left;
                const auto y = glyph.y + config.padding.up;

                font.renderGlyph(&surface[0], s.w, s.h, x, y,
                    kv.first, config.color.getBGR());
            }

            if (glyph.width > max_width) {
                max_width = glyph.width;
            }

            if (glyph.height > max_height) {
                max_height = glyph.height;
            }
        }

        if (!config.backgroundTransparent) {
            auto cur = surface.data();
            const auto end = &surface.back();

            const auto fgColor = config.color.getBGR();
            const auto bgColor = config.backgroundColor.getBGR();

            while (cur <= end) {
                const std::uint32_t a0 = (*cur) >> 24u;
                const std::uint32_t a1 = 256 - a0;
                const std::uint32_t rb1 = (a1 * (bgColor & 0xFF00FFu)) >> 8u;
                const std::uint32_t rb2 = (a0 * (fgColor & 0xFF00FFu)) >> 8u;
                const std::uint32_t g1 = (a1 * (bgColor & 0x00FF00u)) >> 8u;
                const std::uint32_t g2 = (a0 * (fgColor & 0x00FF00u)) >> 8u;
                *cur = ((rb1 | rb2) & 0xFF00FFu) + ((g1 | g2) & 0x00FF00u);
                ++cur;
            }
        }
        auto image_data = ImageData { (uint8_t*)surface.data(), static_cast<size_t>(s.w * s.h * 4) };
        return std::make_tuple(std::move(image_data), max_width, max_height);
    }

    std::unique_ptr<FontAtlas> LoadTrueType(const Config& config)
    {
        std::unique_ptr<FontAtlas> f { new FontAtlas };

        ft::Library library;
        if (config.verbose)
            std::cout << "freetype " << library.getVersionString() << "\n";

        ft::Font font(library, config.fontFile, config.fontSize, 0, config.monochrome);

        auto glyphs = CollectGlyphInfo(font, config.chars);
        const auto pages = ArrangeGlyphs(glyphs, config);
        if (config.useMaxTextuerCount && pages.size() > config.maxTextureCount) {
            Error("too many generated textures (more than --max-texture-count)");
        }

        f->SetGlyphs(std::move(glyphs));

        auto [image_data, width, height] = RenderTextures(glyphs, config, font, pages[0]);
        f->LoadTex(image_data);
        f->SetGWidth(width);
        f->SetGHeight(height);
        return std::move(f);
    }

}
}
}