# pragma once
#include <memory>
#include "Common.hpp"
#include "PhiController.hpp"
#include "ObjectController.hpp"
#include "Collision.hpp"
#include "AnimeDraw.hpp"

// ポイントライトVer.2
struct PSLighting
{
	static constexpr uint32 MaxPointLights = 4;

	struct Light
	{
		Float4 position{ 0, 0, 0, 0 };
		Float4 diffuseColor{ 0, 0, 0, 0 };
		Float4 attenuation{ 1.0f, 2.0f, 1.0f, 0 };
	};

	std::array<Light, MaxPointLights> pointLights;

	/// @brief 点光源を設定します。
	/// @param i 光源のインデックス。0 以上 MaxPointLights 未満である必要があります。
	/// @param pos 光源の位置
	/// @param diffuse 色
	/// @param r 強さ
	void setPointLight(uint32 i, const Vec3& pos, const ColorF& diffuse, double r)
	{
		pointLights[i].position = Float4{ pos, 1.0f };
		pointLights[i].diffuseColor = diffuse.toFloat4();
		pointLights[i].attenuation = Float4{ 1.0, (2.0 / r), (1.0 / (r * r)), 0.0 };
	}

	/// @brief 点光源を球として描画します。
	/// @param i 光源のインデックス。0 以上 MaxPointLights 未満である必要があります。
	/// @param r 球の半径
	void drawPointLightAsEmissiveSphere(uint32 i, double r)
	{
		const Vec3 pos = pointLights[i].position.xyz();
		const ColorF diffuse{ pointLights[i].diffuseColor };

		PhongMaterial phong;
		phong.ambientColor = ColorF{ 0.0 };
		phong.diffuseColor = ColorF{ 0.0 };
		phong.emissionColor = diffuse;
#ifdef _DEBUG
	//	Sphere{ pos, r }.draw(phong);
#endif
	}
};

// アイテムID
enum ItemID
{
	PrideRelic = 0,	// 0 傲慢のレリック
	GreedRelic,	// 1 強欲のレリック
	EnvyRelic,	// 2 嫉妬のレリック
	WrathRelic,	// 3 憤怒のレリック
	LustRelic,	// 4 色欲のレリック
	GluttonyRelic,	// 5 暴食のレリック
	SlothRelic,	// 6 怠惰のレリック
	ItemIdMAX = 99,
};

// 椅子の状態
enum ChairState
{
	Empty = 0b00,
	Placed = 0b01,
	Collect = 0b10,
};

// カメラの実験
class CameraTest : public App::Scene
{
public:

	// コンストラクタ
	CameraTest(const InitData& init);

	// 処理
	void update() override;

	// 描画
	void draw() const override;

private:

	// リソースを読み込む
	void loadResources() const;

	// ポイントライトVer.2
	ConstantBuffer<PSLighting> constantBuffer;

	// デバッグ表示
	void debug();

	// マウスポインタのRay
	Ray getMouseRay() const;

	// インベントリ：アイテムのミニ画像
	void drawMiniItem(
		int itemId,
		int x,
		int y
	) const;

	// インベントリ：アイテムの拡大画像
	void drawBigItem(
		int itemId,
		int x,
		int y
	) const;

	// カメラ
	BasicCamera3D camera;

	// カメラスピード
	const double cameraSpeed = 2.0f;

	// HDR（ハイダイナミックレンジ）レンダリング
	const MSRenderTexture renderTexture{ Scene::Size(), TextureFormat::R16G16B16A16_Float, HasDepth::Yes };

	// 背景色
	ColorF backgroundColor;

	// カメラの視点（現在）
	Vec3 curCameraPosition = Vec3{ 0, 1.5, -2 };

	// カメラの視点（次）
	Vec3 toCameraPos = curCameraPosition;

	// カメラの視点（1フレーム前）
	Vec3 lastCameraPosition = Vec3{ 0, 1.5, -2 };

	// カメラの注視点（カメラが「いま見ている」場所のこと）
	Vec3 m_focusPosition = Vec3{ 0, 0, -1 };

	// カメラの上方向ベクトル
	Vec3 m_upDirection = Vec3{ 0, 1, 0 };

	//Size m_sceneSize = Scene::Size();

	// 視野角
	double m_verticalFOV = 55_deg;
	
	// 近クリッピング面（Near Clipping Plane）
	//double m_nearClip = 0.2;
	double m_nearClip = 0.01;

	// 開始時の目の角度
	double m_focusY = 0;
	double to_m_focusY = 0;

	// カメラの角度
	PhiController phiController = PhiController(curCameraPosition, m_focusPosition);

	//==============================
	// モデル
	//==============================


	// モデルのpath
	const String roomModelPath = U"assets/obj/EV_Map01.obj";
	const String relicPrideModelPath = U"assets/obj/Relic_Pride01.obj";
	const String relicGluttonyModelPath = U"assets/obj/Relic_Gluttony01.obj";
	const String relicEnvyModelPath = U"assets/obj/Relic_Envy01.obj";
	const String relicWrathModelPath = U"assets/obj/Relic_Wrath01.obj";
	const String relicLustModelPath = U"assets/obj/Relic_Lust01.obj";
	const String relicGreedModelPath = U"assets/obj/Relic_Greed01.obj";
	const String relicSlothModelPath = U"assets/obj/Relic_Sloth01.obj";
	const String modelExclamationMarkPath = U"assets/models/ExclamationMark/ExclamationMark.obj";

	// modelの遅延ロード用ポインタ
	mutable std::unique_ptr<Model> roomModel;
	mutable std::unique_ptr<Model> prideRelicModel;
	mutable std::unique_ptr<Model> greedRelicModel;
	mutable std::unique_ptr<Model> envyRelicModel;
	mutable std::unique_ptr<Model> wrathRelicModel;
	mutable std::unique_ptr<Model> lustRelicModel;
	mutable std::unique_ptr<Model> gluttonyRelicModel;
	mutable std::unique_ptr<Model> slothRelicModel;
	mutable std::unique_ptr<Model> modelExclamationMark;
	
	mutable int loadCount = 0;	// リソースの読み込みカウント
	mutable bool bLoaded = false;	// リソースが読み込まれたかどうかのフラグ

	//==============================
	// スケーリング
	//==============================

	// スケーリングの倍率
	const Vec3 roomScale{ 0.01, 0.01, 0.01 };


	//==============================
	// BGM
	//==============================

	// BGMがストップしている時間
	double bgmStopCount = 0.0f;

	//==============================
	// シェーダー
	//==============================

	// 頂点シェーダ
	const VertexShader vs3D = GLSL{
		U"example/shader/glsl/default3d_forward.vert",
		{
			{ U"VSPerView", 1 },
			{ U"VSPerObject", 2 },
			{ U"VSPerMaterial", 3 }
		}
	};

	// ピクセルシェーダ
	const PixelShader ps3D = GLSL{
		U"example/shader/glsl/PointLight.Frag",
		{
			{ U"PSPerFrame", 0 },
			{ U"PSPerView", 1 },
			{ U"PSPointLightPos", 2 },
			{ U"PSPerMaterial", 3 },
			{ U"PSLighting", 4 }
		}
	};

	// ストップウォッチ
	Stopwatch stopwatch{ StartImmediately::Yes };

	// 回転をスムーズにするときに使っている変数
	double sss = 0;

	// 回転をスムーズにするときに使っている変数
	double ccc = 0;

	// 動きをスムーズにする値
	double smooth = 0.1f;
	//double lightSmooth = 0.025f;

	// XINPUT：プレイヤーインデックス (0 - 3)
	size_t playerIndex = 0;

	// m_focusY
	double focusY_max = 2.0;

	// ライトの設定
	Vec3 lightPos;
	float lightStrong = 4.0;

	// 暖炉の明るさ
	float fireplaceStrong = 2.6;
	float fireplaceSin = 0.6;
	float fireplaceR = 1.0;
	float fireplaceG = 0.4;
	float fireplaceB = 0;

	// 環境光
	float globalAmbientColor = 0.75;

	// 太陽光
	float sunColor = 0.0;


	//==============================
	// モデルの座標
	//==============================

	// ルームの位置
	const Vec3 roomPos{ 0, 0, 0 };

	// 大罪
	float chairRelicPosY = 0.45f;
	Vec3 prideChairPos = { 0.04, 0.8, 11.3 };
	Vec3 prideRelicOnChairPos = { prideChairPos.x, chairRelicPosY, prideChairPos.z };
	Vec3 pridePicturePos = { 0, 1.45, 11.6 };

	Vec3 greedChairPos = { 4.7, 0.9, 6.37 };
	Vec3 greedRelicOnChairPos = { greedChairPos.x, chairRelicPosY, greedChairPos.z };
	Vec3 greedPicturePos = { 5, 1.45, 6.38 };

	Vec3 envyChairPos = { -4.7, 0.9, 6.37 };
	Vec3 envyRelicOnChairPos = { envyChairPos.x, chairRelicPosY, envyChairPos.z };
	Vec3 envyPicturePos = { -5, 1.45, 6.37 };

	Vec3 wrathChairPos = { 4.7, 0.9, 1.31 };
	Vec3 wrathRelicOnChairPos = { wrathChairPos.x, chairRelicPosY, wrathChairPos.z };
	Vec3 wrathPicturePos = { 5, 1.45, 1.31 };

	Vec3 lustChairPos = { -4.7, 0.9, 1.30 };
	Vec3 lustRelicOnChairPos = { lustChairPos.x, chairRelicPosY, lustChairPos.z };
	Vec3 lustPicturePos = { -5, 1.45, 1.32 };

	Vec3 gluttonyChairPos = { 4.7, 0.9, -3.65 };
	Vec3 gluttonyRelicOnChairPos = { gluttonyChairPos.x, chairRelicPosY, gluttonyChairPos.z };
	Vec3 gluttonyPicturePos = { 5, 1.45, -3.64 };

	Vec3 slothChairPos = { -4.7, 0.9, -3.65 };
	Vec3 slothRelicOnChairPos = { slothChairPos.x, chairRelicPosY, slothChairPos.z };
	Vec3 slothPicturePos = { -5, 1.45, -3.65 };

	Array<Vec3> relicOnChairPos = {
		prideRelicOnChairPos,
		greedRelicOnChairPos,
		envyRelicOnChairPos,
		wrathRelicOnChairPos,
		lustRelicOnChairPos,
		gluttonyRelicOnChairPos,
		slothRelicOnChairPos
	};

	// レリック
	float relicBaseXPos = 4.7f;
	float relicBaseYPos = 0.8f;
	Vec3 prideRelicCurrentPos = { relicBaseXPos, relicBaseYPos, -10.8 };
	Vec3 greedRelicCurrentPos = { relicBaseXPos, relicBaseYPos, -11.3 };
	Vec3 envyRelicCurrentPos = { relicBaseXPos, relicBaseYPos, -11.8 };
	Vec3 wrathRelicCurrentPos = { -relicBaseXPos, relicBaseYPos, -10.8 };
	Vec3 lustRelicCurrentPos = { -relicBaseXPos, relicBaseYPos, -11.2 };
	Vec3 gluttonyRelicCurrentPos = { -relicBaseXPos, relicBaseYPos, -11.6 };
	Vec3 slothRelicCurrentPos = { -relicBaseXPos, relicBaseYPos, -12 };

	// レリックの回転
	Array<double> relicRotY = {
		90_deg,
		0_deg,
		0_deg,
		0_deg,
		0_deg,
		0_deg,
		0_deg
	};

	Array<double> chairRotY = {
		270_deg,
		0_deg,
		180_deg,
		0_deg,
		180_deg,
		0_deg,
		180_deg
	};

	// 脱出の扉
	Vec3 exitDoorPos = { 0, 1, -13.5 };
	Vec3 exitDoorPlatePos = { -1.88, 1.2, -13.7 };

	// オブジェクトクラス
	ObjectController prideChairController;
	ObjectController pridePictureController;
	ObjectController greedChairController;
	ObjectController greedPictureController;
	ObjectController envyChairController;
	ObjectController envyPictureController;
	ObjectController wrathChairController;
	ObjectController wrathPictureController;
	ObjectController lustChairController;
	ObjectController lustPictureController;
	ObjectController gluttonyChairController;
	ObjectController gluttonyPictureController;
	ObjectController slothChairController;
	ObjectController slothPictureController;
	ObjectController exitDoorController;
	ObjectController exitDoorPlateController;

	ObjectController prideRelicController;
	ObjectController greedRelicController;
	ObjectController envyRelicController;
	ObjectController wrathRelicController;
	ObjectController lustRelicController;
	ObjectController gluttonyRelicController;
	ObjectController slothRelicController;

	// // old
	// ObjectController breadController;
	// ObjectController rustedKeyController;
	// ObjectController ironkeyController;
	// ObjectController pokerController;
	// ObjectController doorController;
	// ObjectController door2Controller;
	// ObjectController door3Controller;
	// ObjectController parchmentController;
	// ObjectController bedController;
	// ObjectController bed2Controller;
	// ObjectController bed3Controller;
	// ObjectController bed4Controller;
	// ObjectController oldBedController;
	// ObjectController toiletController;
	// ObjectController shelfController;
	// ObjectController wallController;
	// ObjectController fireplaceController;
	// ObjectController fireplaceStrongController;
	// ObjectController hangerController;
	// ObjectController barrelController;
	// ObjectController dirtyClothController;
	// ObjectController memoController;
	// ObjectController drawerController;
	// ObjectController stoneController;
	// ObjectController chairController;
	// ObjectController handcuffsController;
	// ObjectController paintingController;
	// ObjectController smallShelfController;
	// ObjectController boardController;

	double GlobalAmbientColorR = 0.4;
	double GlobalAmbientColorG = 0.4;
	double GlobalAmbientColorB = 0.5;

	double toGlobalAmbientColorR = GlobalAmbientColorR;
	double toGlobalAmbientColorG = GlobalAmbientColorG;
	double toGlobalAmbientColorB = GlobalAmbientColorB;

	double mousePosX = 0.0f;
	double mousePosY = 0.0f;
	double toMousePosX = 0.0f;
	double toMousePosY = 0.0f;

	int mouseDirectionX = 1;
	int mouseDirectionY = 1;

	// コリジョンの有効フラグ
	bool bCollision = true;
	bool bCollisionDoor = true;

	// 60FPS対応
	const double targetDeltaTime = 1.0 / 60.0; // 約16.666ms

	// コリジョンリスト
	double collisionList[12][2] = {
	   { 0, 1 },
	   { 1, 3 },
	   { 3, 2 },
	   { 2, 0 },

	   { 0, 4 },
	   { 1, 5 },
	   { 2, 6 },
	   { 3, 7 },

	   { 4, 5 },
	   { 5, 7 },
	   { 7, 6 },
	   { 6, 4 },
	};

	// コリジョンリスト
	double collisionTriangle[12][3] = {
	   { 0, 1, 2 },
	   { 1, 2, 3 },

	   { 0, 2, 4 },
	   { 2, 4, 6 },

	   { 2, 3, 6 },
	   { 3, 6, 7 },

	   { 1, 3, 5 },
	   { 3, 5, 7 },

	   { 1, 3, 5 },
	   { 3, 5, 7 },

	   { 4, 5, 6 },
	   { 5, 6, 7 },
	};

	// デバッグ用のフラグ
	bool bDebugViewFrame = true;
	bool bDebugViewCollision = false;
	bool bDebugviewModel = true;
	bool bDebugFlashingLight = true;

	// 自分自身のコリジョンの半径
	double myRadius = 0.3;

	// ウィンドウの中心座標
	const Vec2 center = Scene::Center();
	const int32 height = Scene::Height();

	// 仮想マウス座標
	Vec2 virtualCursorPos = Vec2{ 0, 0 };

	// インベントリアイテムアイコンのサイズ
	const int inventoryMini = 60;
	const int inventoryBig = 140;

	// スプライト
	const Texture prideRelicUiSprite{ U"assets/sprites/UI_Relic_Pride.png"};
	const Texture greedRelicUiSprite{ U"assets/sprites/UI_Relic_Greed.png" };
	const Texture envyRelicUiSprite{ U"assets/sprites/UI_Relic_Envy.png" };
	const Texture wrathRelicUiSprite{ U"assets/sprites/UI_Relic_Wrath.png" };
	const Texture lustRelicUiSprite{ U"assets/sprites/UI_Relic_Lust.png" };
	const Texture gluttonyRelicUiSprite{ U"assets/sprites/UI_Relic_Gluttony.png" };
	const Texture slothRelicUiSprite{ U"assets/sprites/UI_Relic_Sloth.png" };

	const Texture inventorySprite{ U"assets/sprites/BG_Inventory01.png" };
	const Texture nowLoadingSprite{ U"assets/sprites/UI_Now_Loading01.png" };

	// ビルボード用
	const Texture uvChecker{ U"assets/sprites/ExclamationMark.png", TextureDesc::MippedSRGB };

	// ビルボード表示する板
	const Mesh billboard{ MeshData::Billboard() };

	// インベントリの表示
	bool bInventory = false;

	// インベントリの大きさ
	const int inventoryWidth = 512;
	const int inventoryHeight = 512;

	// インベントリのON／OFF
	void inventoryOnOff();

	// アイテムアイコンの大きさ
	const int itemMiniWidth = 60;
	const int itemMiniHeight = 60;
	const int itemBigWidth = 140;
	const int itemBigHeight = 140;

	// 選択中のアイテム番号
	int itemIndex = 0;

	// ビックリマーク
	Vec3 markPosition{ 0.0, 0.0, 0.0 };
	float markSize = 0.1;
	float markHigh = 0.3;

	// 取得しているアイテム一覧
	Array<int> items;

	// メッセージ番号
	int message = 3;
	int messagePattern = 0;
	int itemMessage = -1;

	int debugItemNameX = 805;
	int debugItemNameY = 236;

	// シナリオ番号
	int scenario = 0;

	// 絵画を見ているかどうかのフラグ
	bool bWatchPicture = false;
	// 見ている絵画の番号
	int pictureIndex = -1;
	// 絵画を開いてからのフレーム数
	int pictureOpenFrameCount = 0;
 	// 脱出扉のプレートを見ているかどうかのフラグ
	bool bWatchExitPlate = false;

	// 大罪の遺物を描画するかどうかのフラグ
	Array<bool> bShowRelicArray = { true, true, true, true, true, true, true };

	// レリックを持っているかどうかのフラグ
	Array<bool> bRelicHaveArray = { false, false, false, false, false, false, false };

	// レリックを椅子に配置したかどうかのフラグ
	Array<bool> bRelicPlacedArray = { false, false, false, false, false, false, false };

	bool bFrontOfChair = false;
	int frontOfChairIndex = -1;

	// 各要素のビット数
	const int BITS_PER_CHAIR_CELL = 2;

	// クリア判定用のマスク
	const unsigned int ALL_COLLECTED_MASK = 0b10101010101010;

	// 椅子の状態をまとめた変数
	unsigned int chairStates = 0;
	// どの椅子にレリックを配置したかをまとめた変数
	Array<ItemID> relicsOnChairs = Array<ItemID>(7, ItemID::ItemIdMAX);

	// 椅子の状態を設定・取得する関数
	void setChairState(int chairIndex, ChairState state, ItemID relic);
	ChairState getChairState(int chairIndex) const;
	ItemID getRelicOnChair(int chairIndex) const;
	bool clearCheck();

	// クリアフラグ
	bool bClear = false;
	bool bPlayEpilogue = false;

// 	// old
// 	// 錆びた鍵を持っている
// 	bool bRustedKeyHave = false;

// 	// パンを持っている
// 	bool bBreadHave = false;

// 	// 火かき棒を持っている
// 	bool bPokerHave = false;

// 	// 羊皮紙を持っている
// 	bool bParchmentHave = false;

// 	// 手記を読んでいる
// 	bool bMemo = false;

// 	// 壁を調べた
// 	bool bWall = false;

// 	// 針金を持っている
// 	bool bWire = false;

// 	// 針金製の鍵を持っている
// 	bool bWireKey = false;

// 	// 鉄製の鍵を持っている
// 	bool bIronKeyHave = false;

// 	// 暖炉の火が強くなった
// 	bool bFireplaceStrong = false;

// 	// 炙った羊皮紙を読んでいる
// 	bool bToastedParchmentRead = false;

// 	// ハンガーを見つけた
// 	bool bHangerFind = false;

// 	// ハンガーを持っている
// 	bool bHangerHave = false;

// 	// 汚れた布を持っている
// 	bool bDirtyClothHave = false;

// 	// 手記を持っている
// 	bool bMemoHave = false;

// 	// 布を持っている
// 	bool bClothHave = false;

// 	// 布を読んでいる
// 	bool bClothRead = false;

// 	// 樽３の近くにいる
// 	//bool bBarrel3Lockon = false;

// 	// トイレ２の近くにいる
// 	bool bToilet2Lockon = false;

// 	// 暖炉の近くにいる
// 	bool bFireplaceLockon = false;

// 	// 火の強い暖炉の近くにいる（オミット）
// //	bool bFireplaceStrongLockon = false;

// 	// 鉄製の鍵の前にいる
// 	bool bIronKeyLockon = false;

// 	// 最初の部屋のドアの前にいる
// 	bool bDoorLockon = false;

// 	// 左下の部屋のドアの前にいる
// 	bool bDoor2Lockon = false;

// 	// 奥の部屋のドアの前にいる
// 	bool bDoor3Lockon = false;

// 	// 黄金の鍵を持っている
// 	bool bDrawerClear = false;

// 	// （仮）石板クリアフラグ
// 	// bool bStoneclear = false;

	const Font& boldFont = FontAsset(U"Bold");

	// テキストを事前に読み込むための関数
	void dummyTextView(Array<String> text) const;
	void dummyTextView(Array<Array<String>> text) const;

	// メッセージのパターン数
	const int MessagePatternMax = 3;

//	Array<int> messageRead;

	// message番号
	const int exitDoorMessage = 94;
	const int exitDoorPlateMessage = 95;

	const int prideChairMessage = 96;
	const int greedChairMessage = 96;
	const int envyChairMessage = 96;
	const int wrathChairMessage = 96;
	const int lustChairMessage = 96;
	const int gluttonyChairMessage = 96;
	const int slothChairMessage = 96;

	const int prideRelicGetMessage = 97;
	const int greedRelicGetMessage = 98;
	const int envyRelicGetMessage = 99;
	const int wrathRelicGetMessage = 100;
	const int lustRelicGetMessage = 101;
	const int gluttonyRelicGetMessage = 102;
	const int slothRelicGetMessage = 103;

	Array<int> relicGetMessageArray = {
		prideRelicGetMessage,
		greedRelicGetMessage,
		envyRelicGetMessage,
		wrathRelicGetMessage,
		lustRelicGetMessage,
		gluttonyRelicGetMessage,
		slothRelicGetMessage
	};

	const int relicNotFrontOfChairMessage = 104;
	const int relicCollectMessage = 105;
	const int chairAlreadySetRelicMessage = 106;
	const int allRelicPlacedMessage = 107;
	const int gameClearMessage = 108;

	// 画面下のテキスト
	Array<String> Text =
	{
		// シナリオ系

		// 0 最初
		U"　　　　腹が減った。まずは食事をしなければ…。　　　　",	// 0
		U"",	// 0
		U"",	// 0

		// 1 パンを取った後のシナリオテキスト
		U"「SPACE」キーでインベントリを開き、パンを使用する。",
		U"",
		U"",

		// 2 パンを食べた後
		//U"パンの中に手記が入っていた。",
		U"腹の足しにはならないな……それでも、少しマシになった",
		U"",
		U"",
		
		// 3 メッセージ無しとして使います（旧：手記を読んだ後）
		//U"ベッド脇の壁を調べてみよう。",
		//U"この手記は何かのヒントだろうか…。",
		U"",
		U"",
		U"",

		// 4 錆びた鍵を取った後
		//U"この鍵は錆びて使えないな…。",
		U"",
		U"",
		U"",

		// 5 棚の上のハンガー（これは使わない）
		//U"棚の中にハンガーがあった。これは何かに使えるかも？",
		U"",
		U"",
		U"",

		// 6 針金製の鍵を手に入れた後
		//U"この針金製の鍵で扉を開けてみよう。",
		U"",
		U"",
		U"",

		// 7
		U"水槽がある。",	// 7
		U"",	// 7
		U"",	// 7

		U"羊皮紙の汚れを落としたい。",	// 8
		U"",	// 8
		U"",	// 8

		// 9 引き出し
		U"この引き出しは何だろう。",	// 9
		U"",	// 9
		U"",	// 9

		// 10 鍵がかかっている鉄格子の扉
		U"厚みのある鉄格子が道を塞いでいる。しっかりと鍵がかかっているようだ。",
		U"鉄の扉が頑丈そうに閉じている。無理に開けるのは現実的じゃない。",
		U"鍵穴が見える。何かで開けるしかなさそうだ。",

		// 11 自分のベッド
		U"汚れたマットレス。ここで眠っていたのか……？",
		U"臭いが染みついている……寝返りを打つのも苦痛だっただろう",	// 11
		U"何の夢も見なかった。いや、夢を見ていたのかも……思い出せない",	// 11

		// 12 トイレ
		U"錆びたバケツが床に置かれている。どうやらこれがトイレらしい……最悪だ",
		U"悪臭が鼻を突く。中身は……見ないほうがいい。",
		U"トイレとは名ばかりの容器がそこにある。長居したくない。",

		// 13 棚
		U"手作りのような粗末な棚だ。",	
		U"",
		U"",

		// 14 ベッド
		U"薄汚れたマットレスが載っている。何かの染みが点々とついている……。",
		U"横になる気も失せるほど、マットレスがへたっている。カビ臭さが鼻を刺す。",
		U"汚れと湿気が染みついた寝床。数日は誰かがここで過ごした形跡がある。",

		// 15  金属フレームのみのベッド
		U"マットレスすらない、剥き出しの金属フレーム。横になれば骨が痛みそうだ",
		U"錆びついたフレームだけが残っている。ここで寝るのは拷問に近い。",
		U"寝具の欠片も見当たらない。ただの鉄の枠が虚しく横たわっている。",

		U"ん？この壁は…。",	// 16
		U"",	// 16
		U"",	// 16

		// 棚（ハンガーを見つける前）
		U"何かあるぞ。",	// 17
		U"",	// 17
		U"",	// 17

		// 18 暖炉
		U"炉の中で火が燃えている。",
		U"この部屋の荒れた様子とは、どこか釣り合わない温もりだ。",
		U"さっきまで誰かがここにいたのだろうか",

		// 19
		U"",	// 19
		U"",	// 19
		U"",	// 19

		U"扉だ。この針金製の鍵で開けられるかもしれない。",	// 20
		U"",	// 20
		U"",	// 20

		U"火が強くなった。",	// 21
		U"",	// 21
		U"",	// 21

		U"ハンガーがある。これは何かに使えるかも？",	// 22
		U"",	// 22
		U"",	// 22

		// 23 教団の紋章
		U"奇妙な模様が描かれた布が掛けられている。何かの紋章だろうか……。",
		U"見慣れない図形が布に刻まれている。宗教的な意味を持っていそうだ",
		U"円と棘が交わるようなマーク……教団の象徴かもしれない。",

		U"針金を見つけた。この針金で錆びた鍵の形を模すことはできるだろうか…。",	// 24
		U"",	// 24
		U"",	// 24

		// 25 樽
		U"古びた木製の樽。中からは液体の音がしない。空っぽのようだ。",	// 25
		U"",	// 25
		U"",	// 25

		// 26 汚れた布
		U"布だ。",	// 26
		U"",	// 26
		U"",	// 26

		// 27 樽（中に水が入っている）
		U"古びた木製の樽。中に液体が入っている。",
		U"しかし、この樽を開けることは無理そうだ…。",
		U"",

		// 28 
		U"",	// 28
		U"",	// 28
		U"",	// 28

		// 29 ドア２（開けることができないとき）
		//U"手持ちの鍵で開けることができないようだ。別の鍵があるのだろうか。",	// 29
		U"",	// 29
		U"",	// 29
		U"",	// 29

		// 30 閉まっている木製の扉（奥の部屋のドア）
		U"古びた木の扉。鍵がかかっているようだ",	// 30
		U"頑丈な造りだ。そう簡単には開かないだろう",	// 30
		U"",	// 34

		// 31 鉄製の鍵
		U"格子の向こうに鍵が落ちている……届かない",	// 31
		U"手を伸ばしてみたが、あと少し届かない",	// 31
		U"",	// 31

		// 32 トイレ２（左上の部屋）
		U"臭い……ここがトイレなのか？",	// 32
		U"見るに耐えない……だが、使うしかなかったのだろう",	// 32
		U"水が少し溜まっている。使えるかもしれないが……気が進まない",	// 32

		// 33 閉まっている鉄製の扉（左下の部屋のドア）
		U"重たそうな鉄扉だ。びくともしない……",	// 33
		U"頑丈な造りだ。そう簡単には開かないだろう",	// 33
		U"",	// 33

		// 34 引き出しギミック
		U"奇妙な紋章が彫られている……",	// 34
		U"この模様…何を意味しているのだろうか？",	// 34
		U"",	// 34

		// 35 プロローグ用
		//U"　　　　　　　　　　壁は冷たい石。窓もない。　　　　　　　　　　",	// 35
		U"　　　　壁は冷たい石。窓もない。　　　　",	// 35
		U"",	// 35
		U"",	// 35

		// 36 プロローグ用
		U"　　　　床には薄汚れたマットレス、隅にはバケツのようなものが置かれている。　　　　",	// 36
		U"",	// 36
		U"",	// 36

		// 37 プロローグ用
		U"　　　　ここは……牢屋だろうか。　　　　",	// 37
		U"",	// 37
		U"",	// 37

		// 38 引き出し
		//U"左クリック：引き出しを開ける　右クリック：カメラを戻す",	// 38
		//U"　花　鎖　羽根　蛇　瞳　戻る",
		U"",
		U"",
		U"",

		// 39 引き出し
		U"▶花　鎖　羽根　蛇　瞳　戻る",
		U"",
		U"",

		// 40
		U"　花▶鎖　羽根　蛇　瞳　戻る",
		U"",	// 39
		U"",	// 39

		// 41
		U"　花　鎖▶羽根　蛇　瞳　戻る",
		U"",	// 40
		U"",	// 40

		// 42
		U"　花　鎖　羽根▶蛇　瞳　戻る",
		U"",	// 41
		U"",	// 41

		// 43
		U"　花　鎖　羽根　蛇▶瞳　戻る",
		U"",	// 42
		U"",	// 42

		// 44
		U"　花　鎖　羽根　蛇　瞳▶戻る",
		U"",	// 43
		U"",	// 43

		// 45 詰所の壁際の椅子
		U"年季の入った木製の椅子だ",	// 45
		U"座面が擦り減っている。長く使われていたようだ",	// 45
		U"",	// 45

		// 46 詰所のテーブル
		U"厚い木板で作られたテーブルだ。",	// 46
		U"汚れと傷が目立つ。誰かがここで作業していたのかもしれない",	// 46
		U"",	// 46

		// 47 木箱
		U"中身は空のようだ",	// 47
		U"蓋の隙間から埃が舞い上がった。しばらく開けられていないようだ",	// 47
		U"",	// 47

		U"",	// 48
		U"",	// 48
		U"",	// 48

		U"",	// 49
		U"",	// 49
		U"",	// 49

		// アイテム系
		U"黒ずんだ硬いパンがある。これは食べられそうだ。",	// 50 パン
		U"",	// 50
		U"",	// 50

		// 51 手記がある
		//U"古い手記が落ちている。",
		U"この紙……最近のものじゃない。誰が残した？",
		U"",
		U"",

		// 52 古びた鍵
		U"この壁に刺さっている物は何だ？",	// 52 鍵
		U"",	// 52
		U"",	// 52

		U"何の棒だろう。",	// 53 火かき棒
		U"",	// 53
		U"",	// 53

		U"紙？",	// 54 羊皮紙
		U"",	// 54
		U"",	// 54

		U"ただの針金だ。",	// 55 針金
		U"",	// 55
		U"",	// 55

		U"ひどく汚れていて、何かが書かれているようだが判別できない。洗えば読めるかもしれない。",	// 
		U"",	// 56
		U"",	// 56

		U"文字がはっきりと見える。何かの順番を示しているようだ。",	// 57
		U"",	// 57
		U"",	// 57

		U"表面に新たな文字が浮かび上がった。これは隠された何かのヒントだろうか？",	// 58
		U"",	// 58
		U"",	// 58

		// 59 引き出し
		U"この石板は何だろう。",
		U"",
		U"",

		// 60
		U"▶青　緑　赤　黄　戻る",
		U"",
		U"",

		// 61
		U"　青▶緑　赤　黄　戻る",
		U"",
		U"",

		// 62
		U"　青　緑▶赤　黄　戻る",
		U"",
		U"",

		// 63
		U"　青　緑　赤▶黄　戻る",
		U"",
		U"",

		// 64
		U"　青　緑　赤　黄▶戻る",
		U"",
		U"",

		// 65 詰所の壁際の椅子
		U"年季の入った木製の椅子だ",
		U"座面が擦り減っている。長く使われていたようだ",
		U"",

		// 66 詰所のテーブル
		U"厚い木板で作られたテーブルだ",
		U"汚れと傷が目立つ。誰かがここで作業していたのかもしれない",
		U"",

		// 67 木箱
		U"中身は空のようだ",
		U"蓋の隙間から埃が舞い上がった。しばらく開けられていないようだ",
		U"",

		// 68 引き出しの「無」
		U"鍵がかかっている",
		U"",
		U"",

		// 69 錆びた鍵を扉の前で使う
		U"錆びていて使えそうもない。何か代用できるものはないだろうか？",
		U"",
		U"",

		// 70 特に何もない
		U"特に何もない",
		U"もう用はない",
		U"",

		// 71 特に何もない
		U"火かき棒で【鉄製の鍵】を引き寄せた",
		U"",
		U"",

		// 72 壁に埋め込まれた手錠
		U"これは……誰かを、ここに縛りつけるためのものか",
		U"壁に鉄製の手錠。何に使われていたかは、考えるまでもない",
		U"拷問に使われていたのだろうか……錆びてはいるが、まだ動きそうだ",

		// 73 不気味な絵画（風景）
		U"不気味な絵画だ……。どこかの風景だろうか",
		U"古びた風景画。色あせていて、何が描かれているのか分かりづらい",
		U"",
		
		// 74 壁の小棚
		U"特になにもなさそうだ",
		U"",
		U"",

		// 75 石板のギミックのみを解いて扉を調べる
		U"石板は正しく解いたはずだ。だが、扉は開かない……。何か、まだ“鍵”となるものが必要らしい。",
		U"",
		U"",

		// 76 引き出しギミックのみを解いて扉を調べる
		U"引き出しは正しく解いたはずだ。だが扉は開かない……。何かまだ“鍵”となるものが必要らしい。",
		U"",
		U"",

		// TODO 埋め込み形式にした方がよさそうですが…
		// 77 【アイテム名】を入手した 
		U"【パン】を入手した",
		U"",
		U"",
		// 78 【アイテム名】を入手した 
		U"【手記】を入手した",
		U"",
		U"",
		// 79 【アイテム名】を入手した 
		U"【錆びた鍵】を入手した",
		U"",
		U"",
		// 80 【アイテム名】を入手した 
		U"【火かき棒】を入手した",
		U"",
		U"",
		// 81 【アイテム名】を入手した 
		U"【羊皮紙】を入手した",
		U"",
		U"",
		// 82 【アイテム名】を入手した 
		U"【ハンガー】を入手した",
		U"",
		U"",
		// 83 【アイテム名】を入手した 
		U"【汚れた布】を入手した",
		U"",
		U"",
		// 84 【アイテム名】を入手した 
		U"【布】を入手した",
		U"",
		U"",
		// 85 【アイテム名】を入手した 
		U"【炙った羊皮紙】を入手した",
		U"",
		U"",
		// 86 【アイテム名】を入手した 
		U"【針金製の鍵】を入手した",
		U"",
		U"",
		// 87 【アイテム名】を入手した 
		U"【鉄製の鍵】を入手した",
		U"",
		U"",
		// 88 引き出しインタラクト
		U"レリーフが刻まれた引き出しがある。中には何も入っていなさそうだが…",
		U"",
		U"",
		// 89 引き出し正解後
		U"どこかから音が聞こえた",
		U"",
		U"",
		// 90 石板インタラクト
		U"石板のようなものがある。どうやら押すことができそうだ",
		U"",
		U"",
		// 91 石板正解後
		U"どこかから音が聞こえた",
		U"",
		U"",

		// 92 暖炉上の絵画
		U"不気味な絵画だ……何を描いているのか、判然としない。",
		U"特に変わった点はないようだ。",
		U"",

		// 93 脱出の扉の左にボード
		U"知と象徴、両輪の理解なしに、この扉を越えること能わず。",
		U"",
		U"",

		// 94 出口のドア
		U"鉄製の大きな扉だ。固く閉ざされていて、びくともしない。",
		U"ここが開ければ外に出られるのだろうか。",
		U"",

		// 95 出口のドアのプレート
		U"扉を開けるヒントがここに記されている。",
		U"",
		U"",

		// 96　椅子の共通メッセージ
		U"古びた椅子だ。",
		U"誰かが、長い間ここに座っていた気配が残っている。",
		U"――何かを、待っているようだ。",
		
		// インベントリ用のテキスト
		// 97 傲慢のレリック
		U"【割れた王冠】を入手した",
		U"",
		U"",

		// 98 強欲のレリック
		U"【金で縁取られた小箱】を入手した",
		U"",
		U"",

		// 99 嫉妬のレリック
		U"【歪んだ天秤】を入手した",
		U"",
		U"",

		// 100 憤怒のレリック
		U"【引きちぎられた鎖】を入手した",
		U"",
		U"",

		// 101 色欲のレリック
		U"【香水の瓶】を入手した",
		U"",
		U"",

		// 102 暴食のレリック
		U"【欠けた皿】を入手した",
		U"",
		U"",

		// 103 怠惰のレリック
		U"【止まった懐中時計】を入手した",
		U"",
		U"",

		/// 104 椅子の前にいないときにレリックを使用した
		U"なにかに引き寄せられているような気がする。",
		U"",
		U"",

		/// 105 正しい椅子にレリックを配置した
		U"在るべき場所に還ったようだ。",
		U"",
		U"",

		// 106 椅子にほかのレリックが置かれているときにレリックを使用した
		U"ここには別の遺物が置かれている。",
		U"",
		U"",

		// 107 全て正しい椅子にレリックを配置した
		U"遠くで扉が開く音が聞こえた。",
		U"",
		U"",

		// 108 dummyクリア後扉
		U"扉は既に開いている。",
		U"",
		U"",

	};

	// インベントリ用のテキスト
	// ItemIDに対応
	Array<Array<String>> itemText =
	{
		// 0 傲慢のレリック
		{
			U"かつての栄光を象徴する金の冠。今は真ん中で割れ、　",
			U"輝きだけが残っている。　　　　　　　　　　　　　　",
			U"誰よりも高くあろうとした末に、支えるものを失った。",
		},

		// 1 強欲のレリック 24文字
		{
			U"どれほど詰め込んでも、満たされることはなかった。",
			U"掌に乗るほど小さいのに、持つ者の心を重くする。　",
		},

		// 2 嫉妬のレリック
		{
			U"片方だけが沈み、もう一方は錆びついて動かない。　",
			U"比べ続けた果てに、何も映らなくなった。　　　　　",
		},

		// 3 憤怒のレリック
		{
			U"理性を繋ぎ止めていたもの。　　　　　　　　　　　",
			U"今は冷たく、ねじ曲がった鉄の匂いを放っている。　",
		},

		// 4 色欲のレリック
		{
			U"かつては甘く、今は何も残っていない。　　　　　　",
			U"揮発した香りだけが、記憶のように空気に漂う。　　",
		},

		// 5 暴食のレリック
		{
			U"食べ尽くした後の静けさだけが残る。　　　　　　　",
			U"欠片には、かすかに何かがこびりついている。　　　",
		},

		// 6 怠惰のレリック
		{
			U"長い間、時を刻むことをやめている。　　　　　　　",
			U"針は重く、持ち上げることさえ億劫に思える。　　　",
		},

		// 7 布
		{
			U"文字がはっきりと見える。　　　　　",
			U"何かの順番を示しているようだ。　　",
			U"　　　　　　　　　　　　　　　　　",
		},

		// 8 炙った羊皮紙
		{
			U"表面に新たな文字が浮かび上がった。",
			U"これは隠された何かの　　　　　　　",
			U"ヒントだろうか？　　　　　　　　　",
		},

		// 9 針金製の鍵
		{
			U"ハンガーの針金で作った鍵。　　　　",
			U"これで扉を開けることができるかも　",
			U"しれない。　　　　　　　　　　　　",
		},

		// 10 鉄製の鍵
		{
			U"鉄製の鍵だ。　　　　　　　　　　　",
			U"　　　　　　　　　　　　　　　　　",
			U"　　　　　　　　　　　　　　　　　",
		},

		// 11 黄金の鍵
		{
			U"黄金の鍵だ。　　　　　　　　　　　",
			U"　　　　　　　　　　　　　　　　　",
			U"　　　　　　　　　　　　　　　　　",
		},
	};

	// インベントリ用のテキスト（アイテム名）
	// ItemIDに対応
	Array<String> itemNameText =
	{
		// アイテム系
		U"割れた王冠",	// 0 
		U"金で縁取られた小箱",	// 1 
		U"歪んだ天秤",	// 2 
		U"引きちぎられた鎖",	// 3 
		U"香水の瓶",	// 4 
		U"欠けた皿",	// 5 針金→ハンガー
		U"止まった懐中時計",	// 6
		U"布",	// 7
		U"炙った羊皮紙",	// 8
		U"針金製の鍵",	// 9
		U"鉄製の鍵",	// 10
		U"黄金の鍵",	// 11
	};

	// プロローグ
	Array<String> prologueText =
	{
		U"　　目を覚ますと、見知らぬ部屋にいた。　　",
		U"　　かすかな埃の匂い。壁には古びた絵画が並び、　　",
		U"　　壁際には七つの椅子が沈黙している。　　",
		U"",
		U"　　扉は固く閉ざされ、外の気配はない。　　",
		U"　　ここから出なければ――そう思った瞬間、　　",
		U"　　耳の奥で、誰かの囁きがした。　　",
		U"",
		U"",
		U"　　「魂を、座に戻せ。」　　",
	};

	// エピローグ
	Array<String> epilogueText =
	{
		U"　　扉を出た時、気づいた。　　",
		U"　　あの部屋に囚われていたのは、　　",
		U"　　ほかでもない――自分自身だった。　　",
		U"",
		U"　　罪は消えない。　　",
		U"　　けれど、もう逃げることはない。　　",
		U"　　光の中で、静かに息をついた。　　",
	};

	Array<String> exitPlateText =
	{
		U"七つの座が、罪を抱いて沈黙している。",
		U"それぞれの椅子には、かつて誰かが座っていた。",
		U"",
		U"魂を見つけ、戻しなさい。",
		U"それが、ここを出る唯一の道。",
		U"",
		U"すべての座が満たされたとき、",
		U"扉は静かに開くだろう。",
	};

	Array<Array<String>> pictureText =
	{
		// 0 傲慢
		{
			U"ここには、かつて自分を王だと信じた者がいた。\n",
			U"彼の声は誰よりも高く、命令は絶対だった。\n",
			U"だが、やがて臣下は去り、讃える声も消えた。\n",
			U"\n",
			U"残されたのは、空の椅子と、\n",
			U"自分の影にさえ背を向けた男の沈黙だけだった。\n",
		},

		// 1 強欲
		{
			U"ここには、金を抱えたまま眠れなかった者がいた。\n",
			U"宝は積まれ、指先は輝きに埋もれていった。\n",
			U"けれど、その眼は決して閉じなかった。\n",
			U"\n",
			U"朝が来ても、誰も座ることのない椅子だけが残り、\n",
			U"金庫の中では、鍵がひとりでに転がっていた。\n",
		},

		// 2 嫉妬
		{
			U"この部屋には、もう一人いた。\n",
			U"その人の笑顔を、壁の隙間から何度も見た。\n",
			U"どうしてあの光は、自分には向かないのだろう。\n",
			U"\n",
			U"いつしか、影の中が居場所になった。\n",
			U"そして、見つめることだけが、生きる理由になった。\n",
		},

		// 3 憤怒
		{
			U"ここには、壁の向こうから返事を待つ男が座っていた。\n",
			U"誰も答えない夜が続き、\n",
			U"やがて拳だけが言葉になった。\n",
			U"\n",
			U"壁は沈黙を守り、\n",
			U"男の中で何かが壊れた。\n",
		},

		// 4 色欲
		{
			U"ここには、美しさを讃えられた女が座っていた。\n",
			U"彼女は愛されるたびに、\n",
			U"自分が削れていくのを感じていたという。\n",
			U"\n",
			U"それでも、誰かの眼差しがないと、\n",
			U"生きていることさえ確かめられなかった。\n",
		},

		// 5 暴食
		{
			U"ここには、食べても食べても飢えていた者がいた。\n",
			U"皿は山となり、机は崩れた。\n",
			U"それでも、手は止まらなかった。\n",
			U"\n",
			U"最後に残った一口を見つめながら、\n",
			U"彼はようやく気づいたという。\n",
			U"――何を食べていたのか、思い出せないことに。\n",
		},

		// 6 怠惰
		{
			U"ここには、いつか起き上がるはずだった者がいた。\n",
			U"ベッドは沈み、部屋は荒んでいる。\n",
			U"この部屋の時は、とうに止まったままだ。\n",
			U"\n",
			U"それでも、窓の外では朝が来て、\n",
			U"何事もなかったように、埃だけが光を受けていた。\n",
		},
	};

	float prologueCount = 0;
	float messageCount = 0;
	bool  bPrologueMessageEnd = false;

	float epilogueCount = 0;
	bool  bEpilogueMessageEnd = false;
	bool bStartPlaying = false;

	
	const float prologueTiming = 30;
	const float epilogueTiming = 30;
	const float prologueMessageSpeed = 5;
	const float epilogueMessageSpeed = 5;
	const float MessageSpeed = 5;

	// SEを鳴らす
	void playSEandBGMStop(String SE);
	void playSE(String SE);

	// コリジョンのデバッグ表示
	void debugViewCollision();

	// インベントリ表示
	void viewInventory();

	// チュートリアルフラグ（パン入手から使うまで。使ったらfalseになる）
	bool bTutorial = true;

	// // 合成アイテム番号
	// int synthesisIndex = -1;

	// モデルの表示
	void viewModel();

	// ロックオン
	bool bLockon = false;

	// オブジェクトのロックオン
	void lockon();

	// // 引き出しモード
	// bool bDrawerMode = false;

	// // 引き出しのインデックス番号
	// int drawerIndex = 0;

	// // 引き出しの順番
	// int drawerOrder = 0;

	// // 引き出しのカウンター
	// int drawerCounter= 0;

	// // 引き出しフラグ
	// bool drawerPull[6] = {
	// 	false,
	// 	false,
	// 	false,
	// 	false,
	// 	false,
	// 	false,
	// };

	// // 引き出しアニメーション用のカウンター
	// float drawerPullCount = 0;

	// // 石板モード
	// bool bStoneMode = false;

	// // 石板のインデックス番号
	// int stoneIndex = 0;

	// // 石板の順番
	// int stoneOrder = 0;

	// // 石板のカウンター
	// int stoneCounter = 0;

	// // 石板フラグ
	// bool stonePull[4] = {
	// 	false,
	// 	false,
	// 	false,
	// 	false,
	// };

	// // 石板：クリア後にゲーム画面に戻るためのカウンター
	// float stonePushCount = 0;

	float debugHeight = 1.58;
	float debugDis = 0.55;
	float debugRot = -0.53;

	// 足音変数
	int footpattern = 0;
	float footcount = 1;

	bool bMouseL = false;

	const float BGMVolume = 0.56f;

	bool bDrawerNone = false;
	int lastDrawerIndex = 0;
	bool bDrawerOpen = false;

	// bool bRustedKeyUse = false;

	// 前のフレームでぶつかっているか
	bool lastCheckCollision = false;

	float breathCount = 0;

	int bookingMessage = 0;

	// 優先メッセージ
	int priorityMessage = 0;

	// 強制メッセージを表示する時間
	float priorityMessageCount = 0;

	// 優先メッセージ表示時間
	const float priorityMessageCountMax = 3.0;

	// 前回のメッセージ
	int lastMessage = 0;

	float xboxCameraSpeed = 2.0;

	bool xboxControllerDown = false;
	bool xboxControllerDownX = false;
	bool xboxControllerDownY = false;

	bool KeyMode = false;

	Vec2 lastCursorPos;

	int selectItem = -1;
};
