#include <gfx/bk/uniform.h>

using namespace bk;

UniformBlock::UniformBlock(std::string program_name, std::string name)
{
    slot_ = prog_mod_uniformblock_slot(SG_SHADERSTAGE_VS, "vs_params");
    name_ = name;
    size_ = prog_mod_uniformblock_size(SG_SHADERSTAGE_VS, "vs_params");
}

UniformBlockBuffer::UniformBlockBuffer():buffer_()
{
}

uint32_t  UniformBlockBuffer::GetPos()
{
    return pos_;
}

void UniformBlockBuffer::Reset()
{
    pos_ = 0;
}

void UniformBlockBuffer::Seek(uint32_t pos)
{
    pos_ = pos;
}

bool UniformBlockBuffer::IsEmpty()
{
    return pos_ == 0;
}

void UniformBlockBuffer::WriteUInt32(uint32_t value)
{
    auto u32 = static_cast<const uint8_t*>(&value);
    auto len = sizeof(value);
    Copy(u32, len);
}

void UniformBlockBuffer::Copy(const void *ptr, uint32_t size)
{
    std::copy(ptr, prt+size, &*buffer_.begin() + pos_);
    pos_ += len;
}

void *UniformBlockBuffer::ReadPointer(uint32_t size)
{
    auto ptr = &buffer_[size];
    pos_ += size;
    return ptr;
}
