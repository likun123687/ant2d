#pragma once
#include <array>
#include <asset/shader_utils.h>
#include <string>

namespace ant2d {
const uint16_t kUniformblockBufferSize = 16 << 10;

class Uniformblock {
private:
    uint8_t slot_;
    uint8_t size_;
    sg_shader_stage stage_;
    ShaderType shader_type_;
    std::string name_;

public:
    uint8_t Create(ShaderType type, sg_shader_stage stage, const std::string& name);
    static void decode(uint32_t code, uint8_t* stage, uint8_t* slot, uint8_t* size);
    static uint32_t encode(uint8_t stage, uint8_t slot, uint8_t size);

    uint8_t GetSlot() const
    {
        return slot_;
    }

    uint8_t GetSize() const
    {
        return size_;
    }

    sg_shader_stage GetStage() const
    {
        return stage_;
    }

    ShaderType GetShaderType() const
    {
        return shader_type_;
    }

    std::string GetName() const
    {
        return name_;
    }
};

class UniformblockBuffer {
public:
    UniformblockBuffer();
    uint32_t GetPos();
    void Reset();
    void Seek(uint32_t pos);
    bool IsEmpty();
    void WriteUInt32(uint32_t value);
    void WriteUInt16(uint16_t value);
    void Copy(const void* ptr, uint32_t size);
    void* ReadPointer(uint32_t size);
    uint32_t ReadUInt32();

private:
    std::array<uint8_t, kUniformblockBufferSize> buffer_;
    uint32_t size_;
    uint32_t pos_;
};
}
