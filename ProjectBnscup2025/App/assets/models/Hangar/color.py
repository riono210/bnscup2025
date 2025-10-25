from PIL import Image

# PNGファイルを読み込む
img = Image.open('output.png')

# 画像がパレットモードかチェック
if img.mode != 'P':
    print('この画像はパレットモードではありません。')
    exit()

# パレット情報を取得
palette = img.getpalette()

# 3要素ずつ走査して [0,0,0] を [1,1,1] に変更
for i in range(0, len(palette), 3):
    r, g, b = palette[i], palette[i+1], palette[i+2]
    if (r, g, b) == (0, 0, 0):
        palette[i]   = 255
        palette[i+1] = 255
        palette[i+2] = 255

# 変更したパレットを画像に適用
img.putpalette(palette)

# 結果を保存
img.save('output2.png')
