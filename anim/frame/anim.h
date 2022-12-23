#pragma once
#include <string>
#include <gfx/bk_texture.h>
#include <gfx/sprite/sprite_table.h>
#include <anim/frame/flipbook_table.h>

namespace ant2d {
namespace frame {
    // Frames data of Sprite Animation
    struct Animation {
        std::string name;
        int start;
        int len;
        bool loop;
    };

    // Sprite Animation System
    class SpriteEngine {
    private:
        std::vector<ITexture2D*> frames_;
        std::vector<Animation> data_;
        std::unordered_map<std::string, int> names_;

        SpriteTable* sprite_table_;
        FlipbookTable* flipbook_table_;

    public:
        void RequireTable(TableList* table_list);
        
        // 创建新的动画数据
        // 现在 subText 还是指针，稍后会全部用 id 来索引。
        // 动画资源全部存储在一个大的buffer里面，在外部使用索引引用即可.
        // 采用这种设计，删除动画将会变得麻烦..
        // 或者说无法删除动画，只能全部删除或者完全重新加载...
        // 如何动画以组的形式存在，那么便可以避免很多问题
        void NewAnimation(const std::string& name, std::vector<ITexture2D*>& frames, bool loop);
        std::tuple<Animation*, std::vector<ITexture2D*>::iterator, std::vector<ITexture2D*>::iterator> GetAnimation(const std::string& name);
        void Update(float dt);
    };

}
} // namespace ant2d
