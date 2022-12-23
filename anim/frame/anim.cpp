#include <anim/frame/anim.h>

namespace ant2d {
namespace frame {

    void SpriteEngine::RequireTable(TableList* table_list)
    {
        for (auto& t : *table_list) {
            if (t->GetTableType() == TableType::kFlipbook) {
                flipbook_table_ = reinterpret_cast<FlipbookTable*>(t.get());
            } else if (t->GetTableType() == TableType::kSprite) {
                sprite_table_ = reinterpret_cast<SpriteTable*>(t.get());
            }
        }
    }

    void SpriteEngine::NewAnimation(const std::string& name, std::vector<ITexture2D*>& frames, bool loop)
    {
        auto start = frames_.size();
        auto size = frames.size();
        frames_.insert(frames_.end(), frames.begin(), frames.end());
        data_.emplace_back(Animation { name, int(start), int(size), loop });
        names_[name] = frames_.size() - 1;
    }

    std::tuple<Animation*, std::vector<ITexture2D*>::iterator, std::vector<ITexture2D*>::iterator> SpriteEngine::GetAnimation(const std::string& name)
    {
        auto it = names_.find(name);
        if (it != names_.end()) {
            auto anim = &data_[it->second];
            return { anim, frames_.begin() + anim->start, frames_.begin() + anim->start + anim->len };
        }

        return { nullptr, frames_.end(), frames_.end() };
    }

    void SpriteEngine::Update(float dt)
    {
        for (int i = 0; i < flipbook_table_->GetSize(); i++) {
            auto flipbook = flipbook_table_->GetComp(i);
            if (flipbook->IsRunning()) {
                auto id = names_[flipbook->define_];
                auto data = data_[id];

                flipbook->gfi_ = (data.start + flipbook->frame_index_);
                flipbook->dt_ += dt;
                if (flipbook->dt_ > flipbook->rate_) {
                    flipbook->ii_ += 1;
                    flipbook->dt_ = 0;
                    auto frame = flipbook->ii_ % data.len;
                    // frame end
                    if (frame == 0) {
                        if (flipbook->loop_ && flipbook->typ_ == FlipbookComp::kPingPong) {
                            flipbook->reverse_ = !flipbook->reverse_;
                            flipbook->ii_ += 1; // skip one frame, or it'll repeat last frame
                            frame += 1;
                        }

                        if (!flipbook->loop_) {
                            flipbook->running_ = false;
                            break;
                        }
                    }

                    if (flipbook->reverse_) {
                        frame = data.len - frame - 1;
                    }

                    // update frame index
                    flipbook->last_frame_index_ = flipbook->frame_index_;
                    flipbook->frame_index_ = frame;
                    flipbook->gfi_ = data.start + frame;
                }

                // update sprite component
                auto comp = sprite_table_->GetComp(flipbook->GetEntity());
                auto frame = frames_[flipbook->gfi_];
                comp->SetSprite(frame);
            }
        }
    }

}
} // namespace ant2d
