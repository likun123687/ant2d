namespace bk {
class Shader {
    private:
    sg_shader sg_shd_id_;
    std::array<AttribBind, 32> AttrBinds_;
    uint32_t numAttr_;
    std::string program_name_;

    public:
    Shader()
    {
        if () {
             sg_shd_id_ = sg_make_shader(simple_shader_desc());
        } else if () {
        } else {
        }

    }

    void BindAttributes(ResManager *R,  const &std::array<Stream, 2> streams)
    {
        uint16_t bindStream = 0;
        uint16_t bindStride = 0;
        for (uint32_t i = 0; i < numAttr_; i++) {
            auto bind = AttrBinds_[i];
            auto stream = streams[bind.stream];
            if (bind.stream != bindStream) {
                auto buffer = R.vertexBuffers[stream.vertexBuffer&IdMask];
            }
        }
    }
}
}
