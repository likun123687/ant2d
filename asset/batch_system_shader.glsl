@vs batch_vs
uniform batch_vs_params{
    mat4 proj;
};

in vec4 xyuv;
in vec4 rgba;

out vec4 outColor;
out vec2 outTexCoord;

void main() {
    outColor = rgba;
    outTexCoord = xyuv.zw;
    gl_Position = proj * vec4(xyuv.xy, 1, 1);
}
@end

@fs batch_fs

uniform sampler2D tex;

in vec4 outColor;
in vec2 outTexCoord;

out vec4 outputColor;
void main() {
    outputColor = texture(tex, outTexCoord) * outColor;
}
@end

@program batch batch_vs batch_fs
