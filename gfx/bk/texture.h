
class Texture2D
{
    public:
        void create(ImageData data)
        {
            width_ = data.width_;
            height_ = data.height_;
            id_ = newTexture(data);
        }

        sg_image newTexture(ImageData data)
        {
            /* initialize the sokol-gfx texture */
            sg_image image_id = sg_alloc_image();
            sg_init_image(image_id, &(sg_image_desc){
                .width = data.width_,
                .height = data.height_,
                /* set pixel_format to RGBA8 for WebGL */
                .pixel_format = SG_PIXELFORMAT_RGBA8,
                .wrap_u = SG_WRAP_REPEAT,
                .wrap_v = SG_WRAP_REPEAT,
                .min_filter = SG_FILTER_LINEAR,
                .mag_filter = SG_FILTER_LINEAR,
                .data.subimage[0][0] = {
                    .ptr = data.pixels_,
                    .size = img_width * img_height * 4,
                }
            });
            return image_id;
        }

        void update(ImageData img)
        {
            sg_image_data image_data = {};
            image_data.data.subimage[0][0] = {
                .ptr = pixels,
                .size = img_width * img_height,
            }
            sg_update_image(id_, &image_data);
        }

    private:
        float width_;
        float height_;
        sg_image id_;
}

