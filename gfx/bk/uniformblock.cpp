#include <gfx/bk/uniformblock.h>

using namespace ant2d;

uint8_t Uniformblock::Create(ShaderType type, sg_shader_stage stage, const std::string& name)
{
    stage_ = stage;
    name_ = name;
    auto slot_func = GetUniformblockSlotFunc(type);
    slot_ = slot_func(stage, name.c_str());

    auto size_func = GetUniformblockSizeFunc(type);
    size_ = size_func(stage, name.c_str());

    shader_type_ = type;
    return slot_;
}

void Uniformblock::decode(uint32_t code, uint8_t* stage, uint8_t* slot, uint8_t* size)
{
    *stage = static_cast<uint8_t>((code >> 16) & 0xFF);
    *slot = static_cast<uint8_t>((code >> 8) & 0xFF);
    *size = static_cast<uint8_t>((code >> 0) & 0xFF);
}

uint32_t Uniformblock::encode(uint8_t stage, uint8_t slot, uint8_t size)
{
    return static_cast<uint32_t>(stage) << 16 | static_cast<uint32_t>(slot) << 8 | static_cast<uint32_t>(size) << 0;
}

UniformblockBuffer::UniformblockBuffer()
    : buffer_()
{
}

uint32_t UniformblockBuffer::GetPos()
{
    return pos_;
}

void UniformblockBuffer::Reset()
{
    pos_ = 0;
}

void UniformblockBuffer::Seek(uint32_t pos)
{
    pos_ = pos;
}

bool UniformblockBuffer::IsEmpty()
{
    return pos_ == 0;
}

void UniformblockBuffer::WriteUInt32(uint32_t value)
{
    // auto u32 = reinterpret_cast<const uint8_t*>(&value);
    Copy(&value, sizeof(value));
}

void UniformblockBuffer::WriteUInt16(uint16_t value)
{
    // auto u16 = reinterpret_cast<const uint8_t*>(&value);
    Copy(&value, sizeof(value));
}

void UniformblockBuffer::Copy(const void* ptr, uint32_t size)
{
    const char* d = static_cast<const char*>(ptr);
    // std::copy(ptr, ptr+size, &*buffer_.begin() + pos_);
    std::copy(d, d + size, &*buffer_.begin() + pos_);
    pos_ += size;
}

void* UniformblockBuffer::ReadPointer(uint32_t size)
{
    auto ptr = &buffer_[pos_];
    pos_ += size;
    return ptr;
}

uint32_t UniformblockBuffer::ReadUInt32()
{
    auto u32 = (uint32_t*)(&buffer_[pos_]);
    pos_ += 4;
    return *u32;
}
