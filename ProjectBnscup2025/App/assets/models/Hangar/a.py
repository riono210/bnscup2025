from PIL import Image

img = Image.open('T_Gimmick_Hangar01_D_index.png')

if img.mode != 'P':
    print('この画像はパレットモードではありません。')
    exit()

# パレットは変更しない場合このまま
# palette = img.getpalette()
# img.putpalette(palette)

# 透過情報が存在すれば上書き
if 'transparency' in img.info:
    # transparencyはbytesまたはlistまたはint
    transparency = img.info['transparency']

    if isinstance(transparency, bytes):
        transparency = list(transparency)
    elif isinstance(transparency, int):
        # 単色透過（例えばインデックス0だけ透明）の場合
        tmp = [255] * 256
        tmp[transparency] = 0
        transparency = tmp

    # すべて不透明化（255）
    for i in range(len(transparency)):
        transparency[i] = 255

    img.info['transparency'] = bytes(transparency)

# 透過情報がなければ何もしなくてOK（すでに不透明）

img.save('output.png')
