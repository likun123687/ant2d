@vs mesh_vs
uniform mesh_vs_params {
    mat4 proj;
    mat4 model;
};

in vec4 xyuv;
in vec4 rgba;

out vec4 outColor;
out vec2 outTexCoord;

void main() {
    outColor = rgba;
    outTexCoord = xyuv.zw;

    gl_Position = proj * model * vec4(xyuv.xy, 1, 1);
}
@end


@fs mesh_fs
uniform sampler2D tex;

in vec4 outColor;
in vec2 outTexCoord;

out vec4 outputColor;
void main() {
    outputColor = texture(tex, outTexCoord) * outColor;
}
@end

@program mesh mesh_vs mesh_fs
