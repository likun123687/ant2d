
@ctype mat4 ::ant2d::math::Mat4

@vs debug_vs
uniform debug_vs_proj {
    mat4 projection;
};

in vec4 xyuv;
in vec4 rgba;

out vec4 outColor;
out vec2 outTexCoord;

void main() {
    outColor = rgba;
	outTexCoord = xyuv.zw;

    gl_Position = projection * vec4(xyuv.xy, 0, 1);
}
@end


@fs debug_fs
uniform sampler2D tex;

in vec4 outColor;
in vec2 outTexCoord;

out vec4 outputColor;
void main() {
	if (outTexCoord.x == 2.0) {
		outputColor = outColor;
	} else {
	    outputColor = outColor * texture(tex, outTexCoord);
	}
}
@end

@program debug debug_vs debug_fs
