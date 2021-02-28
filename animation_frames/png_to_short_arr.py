"""
representation of image will be saved as a short Array, where the value itself is the index of the pixel (128*64=8192). 
But an unsigned short is 2^16=65536 large so the first three bits are free for use. (2^13=8192).
The first saves, if the pixel should be on or off. If there is nothing saved for a pixel it means it is transparent. 1: on 0: off. 
On top of that the diffs for the bongocat frames are calculated and saved compared to the first: idle_0
"""

from PIL import Image

# interpret black as OLED on and white as off
invert_colors = True

ON = 0 if invert_colors else 255
OFF = 255 if invert_colors else 0

# add to set first bit
first_bit = 2**15

# set to true to show the diffs graphically, that were generated. White: on, Grey: ignore, Black: off
show_diffs = False

path_to_images = "./"

frame_filenames = [
    "idle_0_right.png",
    "idle_1_right.png",
    "idle_2_right.png",
    "idle_3_right.png",
    "prep_0_right.png",
    "tap_0_right.png",
    "tap_1_right.png"
    ]

base_frame = "idle_1_right.png"
base_path = path_to_images + base_frame


def print_as_c_arr(arr):
    if isinstance(arr, list):
        print("{", end="")
        for idx, i in enumerate(arr):
            print_as_c_arr(i)
            if idx != len(arr)-1:
                print(",", end=" ")
        print("}", end="")
    else:
        print(arr, end="")


print("Pixel values without diff: ")
for frame_filename in frame_filenames:
    frame_path = path_to_images + frame_filename
    frame_name = frame_filename.replace(".png", "")

    im = Image.open(frame_path, 'r')

    # 255 white, 0 black
    pixel_values_bw = list(im.getdata(0))
    # 255 NOT transparent, 0 transparent
    pixel_values_trans = list(im.getdata(3))

    width, height = im.size

    oled_pixels = []
    for idx, px in enumerate(pixel_values_bw):
        # transparency means just ignore
        if pixel_values_trans[idx] == 0:
            continue

        tmp = idx
        if px == ON:
            tmp += first_bit
        oled_pixels.append(tmp)

    print(frame_name)
    oled_pixels_len = len(oled_pixels)
    oled_pixels.insert(0, oled_pixels_len)
    print_as_c_arr(oled_pixels)
    print("\n")


print("\n\n")
print("Pixel diffs with " + base_frame + " as the base frame: ")

base_im = Image.open(base_path, 'r')
# 255 white, 0 black
pixel_values_bw_base = list(base_im.getdata(0))
# 255 NOT transparent, 0 transparent
pixel_values_trans_base = list(base_im.getdata(3))

width_base, height_base = base_im.size

for frame_filename in frame_filenames:
    frame_path = path_to_images + frame_filename
    frame_name = frame_filename.replace(".png", "")

    im = Image.open(frame_path, 'r')
    # 255 white, 0 black
    pixel_values_bw = list(im.getdata(0))
    # 255 NOT transparent, 0 transparent
    pixel_values_trans = list(im.getdata(3))

    width, height = im.size
    if width_base != width or height_base != height:
        continue

    oled_pixels_diff = []
    for idx, px in enumerate(pixel_values_bw):
        # if it is equal, no redraw required
        if pixel_values_bw_base[idx] == px and pixel_values_trans_base[idx] == pixel_values_trans[idx]:
            continue
        if pixel_values_trans_base[idx] == pixel_values_trans[idx]:
            continue

        tmp = idx
        if px == ON and pixel_values_trans[idx] == 255:
            tmp += first_bit
        oled_pixels_diff.append(tmp)

    print(frame_name)
    oled_pixels_diff_len = len(oled_pixels_diff)
    oled_pixels_diff.insert(0, oled_pixels_diff_len)
    print_as_c_arr(oled_pixels_diff)
    print("\n")

    if show_diffs:
        img = Image.new('L', (width, height), color=125)
        for i in oled_pixels_diff:
            on = 0
            if i > first_bit:
                i = i - first_bit
                on = 255
            img.putpixel((i%width, int(i/width)), on)
        img.show(frame_name)


