#include <effect/pool.h>

namespace ant2d {

Block::Block(ChanField field)
    : chan_field_(field)
{
}

ChanField Block::GetChanField()
{
    return chan_field_;
}

void Block::SetData(uint8_t* data, int size)
{
    data_ = data;
    size_ = size;
}

std::tuple<uint8_t*, int> Block::GetData()
{
    return std::make_tuple(data_, size_);
}

void Block::SetStride(int stride)
{
    stride_ = stride;
}

Pool::Pool(int cap)
    : cap_ { cap }
{
}

int Pool::GetChanTypeSize(ChanType t)
{
    int size = 0;
    switch (t) {
    case ChanType::ChanF32:
        size = 4;
        break;
    case ChanType::ChanV2:
        size = 8;
        break;
    case ChanType::ChanV4:
        size = 16;
        break;
    }
    return size;
}

void Pool::Initialize()
{
    auto size = Size();
    data_.resize(size);
    int offset = 0;

    for (int i = 0; i < blocks_.size(); i++) {
        auto stride = GetChanTypeSize(blocks_[i].GetChanField().type);
        blocks_[i].SetStride(stride);

        blocks_[i].SetData(&(*(data_.begin() + offset)), cap_ * stride);
        offset += cap_ * stride;
        chans_[blocks_[i].GetChanField()] = i;
    }
}

void Pool::AddChan(std::initializer_list<ChanField> fields)
{
    for (auto& f : fields) {
        blocks_.push_back(Block { f });
    }
}

int Pool::Size()
{
    int size = 0;
    for (auto& b : blocks_) {
        size += int(GetChanTypeSize(b.GetChanField().type)) * cap_;
    }
    return size;
}

// Field returns pointer of the filed in the pool.
Pool::FieldUnion Pool::GetField(ChanField t)
{
    auto block = blocks_[chans_[t]];
    uint8_t* data = nullptr;
    std::tie(data, std::ignore) = block.GetData();
    FieldUnion ret {};
    switch (t.type) {
    case ChanType::ChanF32:
        ret.emplace<1>(Channel_f32 { (float*)data, cap_ });
        break;
    case ChanType::ChanV2:
        ret.emplace<2>(Channel_v2 { (math::Vec2*)data, cap_ });
        break;
    case ChanType::ChanV4:
        ret.emplace<3>(Channel_v4 { (math::Vec4*)data, cap_ });
        break;
    }
    return ret;
}

// Swap swap all the field defined in the pool.
void Pool::Swap(int dst, int src)
{
    for (auto& b : blocks_) {
        auto stride = 4 << uint16_t(b.GetChanField().type);
        int i = dst * stride;
        int j = src * stride;
        auto [data, size] = b.GetData();

        std::copy(data + j, data + j + stride, data + i);
    }
}

}