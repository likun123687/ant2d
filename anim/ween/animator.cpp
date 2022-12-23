#include <anim/ween/animator.h>
#include <anim/ween/anim.h>

namespace ant2d {
namespace ween {
    Animator::Animator(TweenEngine* en_, int index_)
        : en(en_)
        , index(index_)
    {
    }

    Animator* Animator::SetDuration(float d_)
    {
        en->SetDuration(index, d_);
        return this;
    }

    Animator* Animator::SetRepeat(int count_, LoopType loop_)
    {
        en->SetRepeat(index, count_, loop_);
        return this;
    }

    Animator* Animator::SetFunction(math::ease::Function function_)
    {
        en->SetFunction(index, function_);
        return this;
    }

    Animator* Animator::OnUpdate(UpdateCallback cb_)
    {
        en->SetUpdateCallback(index, cb_);
        return this;
    }

    Animator* Animator::OnComplete(EndCallback cb_)
    {
        en->SetCompleteCallback(index, cb_);
        return this;
    }

    float Animator::GetValue()
    {
        return en->GetValue(index);
    }

    bool Animator::IsValid()
    {
        if (en == nullptr) {
            return false;
        }

        auto iter = en->loopup.find(index);
        if (iter == en->loopup.end()) {
            return false;
        }

        return true;
    }

    void Animator::Forward()
    {
        en->Forward(index);
    }

    void Animator::Reverse()
    {
        en->Reverse(index);
    }

    void Animator::Stop()
    {
        en->Stop(index);
    }

    void Animator::Dispose()
    {
        en->Delete(index);
    }
}
} // namespace ant2d
