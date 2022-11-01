#pragma once
#include <effect/chan.h>
#include <string>
#include <vector>
#include <variant>

namespace ant2d {
struct ChanField {
    ChanType type;
    std::string name;

    bool operator==(const ChanField& cf) const
    {
        return name == cf.name && cf.type == type;
    }
};

class ChanField_hash {
public:
    size_t operator()(const ChanField& cf) const
    {
        return std::hash<std::string>()(cf.name) ^ std::hash<int>()((int)cf.type);
    }
};

class Block {
public:
    Block(ChanField field);

    ChanField GetChanField();

    void SetData(uint8_t* data, int size);
    std::tuple<uint8_t*, int> GetData();
    void SetStride(int stride);

private:
    ChanField chan_field_;
    int stride_;
    uint8_t* data_;
    int size_;
};

// A Pool represent a particle-pool.
class Pool {
    using FieldUnion = std::variant<std::monostate, Channel_f32, Channel_v2, Channel_v4>;

public:
    Pool();

    static int GetChanTypeSize(ChanType t);
    void Initialize();
    void AddChan(std::vector<ChanField> fields);
    int Size();

    // Field returns pointer of the filed in the pool.
    FieldUnion GetField(ChanField t);

    // Swap swap all the field defined in the pool.
    void Swap(int dst, int src);

private:
    std::vector<Block> blocks_;
    std::unordered_map<ChanField, int, ChanField_hash> chans_;
    int cap_;
    std::vector<uint8_t> data_;
};

}