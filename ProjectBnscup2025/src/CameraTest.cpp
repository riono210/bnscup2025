#include "CameraTest.hpp"

double GetDistance(const Vec2& a, const Vec2& b)
{
	double dx = b.x - a.x;
	double dy = b.y - a.y;
	return std::sqrt(dx * dx + dy * dy);
}

// コンストラクタ
CameraTest::CameraTest(const InitData& init)
	: IScene{ init }
{
	// 背景の色
	backgroundColor = ColorF{ 0.05, 0.08, 0.1 }.removeSRGBCurve();
	
	// カメラ
	camera = BasicCamera3D{ renderTexture.size(), m_verticalFOV, Vec3{ 10, 10, -10 } };

	// 最初にカーソルを中央に
	Cursor::SetPos(center.x, center.y);

	// マウスの初期化
	mousePosX = Cursor::PosF().x;
	mousePosY = Cursor::PosF().y;
	toMousePosX = mousePosX;
	toMousePosY = mousePosY;
	virtualCursorPos.x = center.x;
	virtualCursorPos.y = center.y;

#ifdef _DEBUG
	bDebugViewCollision = true;
#endif

//#ifndef _DEBUG
	// テキストメッセージを先に読み込んでおく

//#endif

	// メッセージを読んだかどうかのフラグをリセット
	//for (int i = 0; i < Text.size() / MessagePatternMax; i++)
	//{
	//	messageRead.push_back(-1);
	//}

	// 扉の回転
	//toDoorRotY = doorRot.y;
	
	// 扉の移動（横に開ける）
	toDoorPosX = doorPos.x;
	toDoor2PosX = door2Pos.x;

	// 扉が開いているかどうか
	bDoorOpen[0] = false;	// 最初の部屋
	bDoorOpen[1] = false;	// 左下の部屋
	bDoorOpen[2] = true;	// 奥の部屋
	bDoorOpen[3] = true;	// 左上の部屋

	// プロローグの文字列の長さ
	for (int i = 0; i < prologueText.size(); ++i)
	{
		prologueLength += prologueText[i].size();
	}

	// リソースの読み込み
//	loadResources();

	// 暖炉のビルボード
	fireBillboard.startDraw(1000, Texture(U"assets/sprites/EF_Fire02.png"), 8u, 1u);

	// Now Loading 関連
//	nowLoadingTime = 0;
//	bNowLoadingEnd = false;

	// ストップウォッチ（なるべく最後に実行する）
	Stopwatch stopwatch{ StartImmediately::Yes };
}

void CameraTest::loadResources() const
{
	// ローディング画面でリソース読み込み

	switch (loadCount)
	{
	case 0:
		model = std::make_unique<Model>(modelPath);
		Model::RegisterDiffuseTextures(*model, TextureDesc::MippedSRGB);
		break;
	case 1:
		modelDoor = std::make_unique<Model>(modelDoorPath);
		Model::RegisterDiffuseTextures(*modelDoor, TextureDesc::MippedSRGB);
		break;
	case 2:
		modelRustedKey = std::make_unique<Model>(modelRustedKeyPath);
		Model::RegisterDiffuseTextures(*modelRustedKey, TextureDesc::MippedSRGB);
		break;
	case 3:
		modelIronKey = std::make_unique<Model>(modelIronKeyPath);
		Model::RegisterDiffuseTextures(*modelIronKey, TextureDesc::MippedSRGB);
		break;
	case 4:
		modelBread = std::make_unique<Model>(modelBreadPath);
		Model::RegisterDiffuseTextures(*modelBread, TextureDesc::MippedSRGB);
		break;
	case 5:
		modelPoker = std::make_unique<Model>(modelPokerPath);
		Model::RegisterDiffuseTextures(*modelPoker, TextureDesc::MippedSRGB);
		break;
	case 6:
		modelDrawerChain = std::make_unique<Model>(modelDrawerChainPath);
		Model::RegisterDiffuseTextures(*modelDrawerChain, TextureDesc::MippedSRGB);
		break;
	case 7:
		modelDrawerEye = std::make_unique<Model>(modelDrawerEyePath);
		Model::RegisterDiffuseTextures(*modelDrawerEye, TextureDesc::MippedSRGB);
		break;
	case 8:
		modelDrawerFeather = std::make_unique<Model>(modelDrawerFeatherPath);
		Model::RegisterDiffuseTextures(*modelDrawerFeather, TextureDesc::MippedSRGB);
		break;
	case 9:
		modelDrawerFlower = std::make_unique<Model>(modelDrawerFlowerPath);
		Model::RegisterDiffuseTextures(*modelDrawerFlower, TextureDesc::MippedSRGB);
		break;
	case 10:
		//modelDrawerNon = std::make_unique<Model>(modelDrawerNonPath);
		//Model::RegisterDiffuseTextures(*modelDrawerNon, TextureDesc::MippedSRGB);
		break;
	case 11:
		modelDrawerSnake = std::make_unique<Model>(modelDrawerSnakePath);
		Model::RegisterDiffuseTextures(*modelDrawerSnake, TextureDesc::MippedSRGB);
		break;
	case 12:
		modelShelf = std::make_unique<Model>(modelShelfPath);
		Model::RegisterDiffuseTextures(*modelShelf, TextureDesc::MippedSRGB);
		break;
	case 13:
		modelExclamationMark = std::make_unique<Model>(modelExclamationMarkPath);
		Model::RegisterDiffuseTextures(*modelExclamationMark, TextureDesc::MippedSRGB);
		break;
	case 14:
		modelParchment = std::make_unique<Model>(modelParchmentPath);
		Model::RegisterDiffuseTextures(*modelParchment, TextureDesc::MippedSRGB);
		break;
	case 15:
		modelHanger = std::make_unique<Model>(modelHangerPath);
		Model::RegisterDiffuseTextures(*modelHanger, TextureDesc::MippedSRGB);
		break;
	case 16:
		modelDirtyCloth = std::make_unique<Model>(modelDirtyClothPath);
		Model::RegisterDiffuseTextures(*modelDirtyCloth, TextureDesc::MippedSRGB);
		break;
	case 17:
		modelMemo = std::make_unique<Model>(modelMemoPath);
		Model::RegisterDiffuseTextures(*modelMemo, TextureDesc::MippedSRGB);
		break;
	case 18:
		modelStoneBase = std::make_unique<Model>(modelStoneBasePath);
		Model::RegisterDiffuseTextures(*modelStoneBase, TextureDesc::MippedSRGB);
		break;
	case 19:
		modelStoneBlue = std::make_unique<Model>(modelStoneBluePath);
		Model::RegisterDiffuseTextures(*modelStoneBlue, TextureDesc::MippedSRGB);
		break;
	case 20:
		modelStoneGreen = std::make_unique<Model>(modelStoneGreenPath);
		Model::RegisterDiffuseTextures(*modelStoneGreen, TextureDesc::MippedSRGB);
		break;
	case 21:
		modelStoneRed = std::make_unique<Model>(modelStoneRedPath);
		Model::RegisterDiffuseTextures(*modelStoneRed, TextureDesc::MippedSRGB);
		break;
	case 22:
		modelStoneYellow = std::make_unique<Model>(modelStoneYellowPath);
		Model::RegisterDiffuseTextures(*modelStoneYellow, TextureDesc::MippedSRGB);
		break;
	case 23:
		dummyTextView(Text);
		break;
	case 24:
		dummyTextView(itemText);
		break;
	case 25:
		dummyTextView(itemNameText);
		break;
	case 26:
		dummyTextView(memoText);
		break;
	case 27:
		dummyTextView(toastedParchmentText);
		break;
	case 28:
		dummyTextView(prologueText);
		break;
	case 29:
		// BGMの読み込み
		AudioAsset(U"BGM").setVolume(0.0);
		AudioAsset(U"BGM").play();
		AudioAsset(U"BGM").stop();
		break;
	case 30:
		AudioAsset(U"bonfire").setVolume(0.0);
		AudioAsset(U"bonfire").play();
		AudioAsset(U"bonfire").stop();
		break;
	case 31:
		AudioAsset(U"footsteps1").setVolume(0.0);
		AudioAsset(U"footsteps1").play();
		AudioAsset(U"footsteps1").stop();
		break;
	case 32:
		AudioAsset(U"footsteps2").setVolume(0.0);
		AudioAsset(U"footsteps2").play();
		AudioAsset(U"footsteps2").stop();
		break;
	case 33:
		AudioAsset(U"drawer_open").setVolume(0.0);
		AudioAsset(U"drawer_open").play();
		AudioAsset(U"drawer_open").stop();
		break;
	case 34:
		AudioAsset(U"drawer_close").setVolume(0.0);
		AudioAsset(U"drawer_close").play();
		AudioAsset(U"drawer_close").stop();
		break;
	case 35:
		AudioAsset(U"GET").setVolume(0.0);
		AudioAsset(U"GET").play();
		AudioAsset(U"GET").stop();
		break;
	case 36:
		AudioAsset(U"IronDoor_Close").setVolume(0.0);
		AudioAsset(U"IronDoor_Close").play();
		AudioAsset(U"IronDoor_Close").stop();
		break;
	case 37:
		AudioAsset(U"WoodDoor_Close").setVolume(0.0);
		AudioAsset(U"WoodDoor_Close").play();
		AudioAsset(U"WoodDoor_Close").stop();
		break;
	case 38:
		AudioAsset(U"Item").setVolume(0.0);
		AudioAsset(U"Item").play();
		AudioAsset(U"Item").stop();
		break;
	case 39:
		AudioAsset(U"Paper").setVolume(0.0);
		AudioAsset(U"Paper").play();
		AudioAsset(U"Paper").stop();
		break;
	case 40:
		// これは鳴らしているのでやらない
		//AudioAsset(U"Title").setVolume(0.0);
		//AudioAsset(U"Title").play();
		//AudioAsset(U"Title").stop();
		break;
	case 41:
		AudioAsset(U"Water").setVolume(0.0);
		AudioAsset(U"Water").play();
		AudioAsset(U"Water").stop();
		break;
	case 42:
		AudioAsset(U"Inventory").setVolume(0.0);
		AudioAsset(U"Inventory").play();
		AudioAsset(U"Inventory").stop();
		break;
	case 43:
		//for (int itemId = 0; itemId < ItemIdMAX; itemId++)
		//{
		//	drawMiniItem(itemId, 0, 0);
		//	drawBigItem(itemId, 0, 0);
		//}
		break;
	case 44:
	//	billboard.draw(camera.billboard(markPosition, markSize), uvChecker);
		break;
	case 45:
		AudioAsset(U"牢屋の扉を開ける").setVolume(0.0);
		AudioAsset(U"牢屋の扉を開ける").play();
		AudioAsset(U"牢屋の扉を開ける").stop();
		break;
	case 46:
		dummyTextView(clothText);
		break;
	case 47:
	//	modelDrawerTest = std::make_unique<Model>(modelDrawerTestPath);
	//	Model::RegisterDiffuseTextures(*modelDrawerTest, TextureDesc::MippedSRGB);
		break;
	default:
		bLoaded = true;	// リソースが読み込まれた
		break;
	}
	loadCount++;

	// 実際に使用して初回ローディングを済ませる
//	inventorySprite.draw(0, 0);
//	breadMiniSprite.draw(0, 0);
//	breadBigSprite.draw(0, 0);
//	memoMiniSprite.draw(0, 0);
//	memoBigSprite.draw(0, 0);
//	keyMiniSprite.draw(0, 0);
//	keyBigSprite.draw(0, 0);
//	//pokerMiniSprite.draw(0, 0);
//	//pokerBigSprite.draw(0, 0);
//	pokerSprite.resized(inventoryMini, inventoryMini).draw(0, 0);
//	pokerSprite.resized(inventoryBig, inventoryBig).draw(0, 0);
//	parchmentMiniSprite.draw(0, 0);
//	parchmentBigSprite.draw(0, 0);
//	wireMiniSprite.draw(0, 0);
//	wireBigSprite.draw(0, 0);
//	//wireKeyMiniSprite.draw(0, 0);
//	//wireKeyBigSprite.draw(0, 0);
//	wireKeySprite.resized(inventoryMini, inventoryMini).draw(0, 0);
//	wireKeySprite.resized(inventoryBig, inventoryBig).draw(0, 0);
//	toastedParchmentMiniSprite.draw(0, 0);
//	toastedParchmentBigSprite.draw(0, 0);
//	dirtyClothMiniSprite.draw(0, 0);
//	dirtyClothBigSprite.draw(0, 0);
//	clothMiniSprite.draw(0, 0);
//	clothBigSprite.draw(0, 0);
//	ironKeyMiniSprite.draw(0, 0);
//	ironKeyBigSprite.draw(0, 0);

	// model.draw();
	// modelDoor.draw();
	// modelKey.draw();
	// modelIronKey.draw();
	// modelBread.draw();
	// modelPoker.draw();
	// modelDrawerChain.draw();
	// modelDrawerEye.draw();
	// modelDrawerFeather.draw();
	// modelDrawerFlower.draw();
	// modelDrawerNon.draw();
	// modelDrawerSnake.draw();
	// modelShelf.draw();
	// modelExclamationMark.draw();
	// modelParchment.draw();
	// modelHanger.draw();
	// modelDirtyCloth.draw();
	// modelMemo.draw();

//	Graphics2D::Flush();
//	System::Update();
}

// テキストメッセージを先に読み込んでおく
void CameraTest::dummyTextView(Array<String> text) const
{
	for (int i = 0; i < text.size(); ++i)
	{
		boldFont(text[i]).drawAt(
			28,
			{ center.x, center.y },
			ColorF{ 1, 1, 1, 0 }
		);
	}
}

void CameraTest::dummyTextView(Array<Array<String>> text) const
{
	for (int i = 0; i < text.size(); ++i)
	{
		dummyTextView(text[i]);
	}
}

// デバッグ機能
void CameraTest::debug()
{
	// デバッグ表示のクリア
	ClearPrint();

	Print << Profiler::FPS() << U" FPS";

	if (Key1.down())
	{
		mouseDirectionX == 1 ? mouseDirectionX = -1 : mouseDirectionX = 1;
	}
	if (Key2.down())
	{
		mouseDirectionY == 1 ? mouseDirectionY = -1 : mouseDirectionY = 1;
	}
	if (Key3.down())
	{
		// コリジョンワイヤーフレーム
		bDebugViewFrame == true ? bDebugViewFrame = false : bDebugViewFrame = true;
	}
	if (Key4.down())
	{
		// コリジョンを表示
		bDebugViewCollision == true ? bDebugViewCollision = false : bDebugViewCollision = true;
	}
	if (Key5.down())
	{
		// コリジョンを有効
		bCollision == true ? bCollision = false : bCollision = true;
	}
	if (Key6.down())
	{
		// モデルを表示
		bDebugviewModel == true ? bDebugviewModel = false : bDebugviewModel = true;
	}
	if (Key7.down())
	{
		// キャラライトの明るさ
		lightStrong += 0.1;
	}
	if (Key8.down())
	{
		// キャラライトの明るさ
		lightStrong -= 0.1;
	}

	if (KeyTab.down())
	{
		// プロローグスキップ
		bPrologueMessageEnd = true;
		bStartPlaying = true;

		prologueCount = 9999;
		messageCount = 9999;
	}



	if (KeyZ.pressed())
	{
		boardPos.x += 0.01;
	}
	if (KeyX.pressed())
	{
		boardPos.x -= 0.01;
	}

	if (KeyC.pressed())
	{
		boardPos.y += 0.01;
	}
	if (KeyV.pressed())
	{
		boardPos.y -= 0.01;
	}

	if (KeyB.pressed())
	{
		boardPos.z += 0.01;
	}
	if (KeyN.pressed())
	{
		boardPos.z -= 0.01;
	}

	if (mouseDirectionX == 1)
	{
		Print << U"[1]カメラ横回転：順";
	}
	else
	{
		Print << U"[1]カメラ横回転：逆";
	}

	if (mouseDirectionY == 1)
	{
		Print << U"[2]カメラ縦回転：順";
	}
	else
	{
		Print << U"[2]カメラ縦回転：逆";
	}
	
	if (bDebugViewFrame)
	{
		Print << U"[3]コリジョン：ワイヤーフレーム";
	}
	else
	{
		Print << U"[3]コリジョン：塗りつぶし";
	}

	if (bDebugViewCollision)
	{
		Print << U"[4]コリジョン表示：表示";
	}
	else
	{
		Print << U"[4]コリジョン表示：非表示";
	}

	if (bCollision)
	{
		Print << U"[5]コリジョン：有効";
	}
	else
	{
		Print << U"[5]コリジョン：無効";
	}

	if (bDebugviewModel)
	{
		Print << U"[6]モデル：表示";
	}
	else
	{
		Print << U"[6]モデル：非表示";
	}

	Print << U"[7][8]キャラライトの明るさ：" << lightStrong;
	//Print << U"[9][0]暖炉の明るさ（最小値）：" << fireplaceStrong;
	//Print << U"[K][L]暖炉の明るさ（最大値）：" << fireplaceStrong + fireplaceSin;
	//Print << U"[Z][X]暖炉のR：" << fireplaceR;
	//Print << U"[C][V]暖炉のG：" << fireplaceG;
	//Print << U"[B][N]暖炉のB：" << fireplaceB;
	//Print << U"[T][Y]環境光の明るさ：" << globalAmbientColor;
	//Print << U"[O][P]太陽光の明るさ：" << sunColor;

	Print << U"[TAB]プロローグスキップ";

	// 環境光
	float globalAmbientColor = 0.01;

	// 太陽光
	float sunColor = 0.0;

	//if (bDebugFlashingLight)
	//{
	//	Print << U"[7]ライトの点滅：あり";
	//}
	//else
	//{
	//	Print << U"[7]ライトの点滅：なし";
	//}

	//if (bDebugShader)
	//{
	//	Print << U"[8]シェーダー：あり";
	//}
	//else
	//{
	//	Print << U"[8]シェーダー：なし";
	//}

	Print << U"[R][F]上下移動";

#ifdef _DEBUG

	Print << U"CameraX=" << toCameraPos.x;
	Print << U"CameraY=" << toCameraPos.y;
	Print << U"CameraZ=" << toCameraPos.z;

	Print << U"bMouseL=" << bMouseL;

#endif
}

// マウスポインタのRay
Ray CameraTest::getMouseRay() const
{
	return camera.screenToRay(Cursor::PosF());
}

void CameraTest::drawMiniItem(
	int itemId,
	int x,
	int y
) const
{
	switch (itemId)
	{
	case Bread:
		// パン
		breadMiniSprite.draw(x, y);
		break;
	case Memo:
		// 手記
		memoSprite.resized(inventoryMini, inventoryMini).draw(x, y);
		break;
	case RustedKey:
		// 錆びた鍵
		rustedKeySprite.resized(inventoryMini, inventoryMini).draw(x, y);
		break;
	case Poker:
		// 火かき棒
		pokerSprite.resized(inventoryMini, inventoryMini).draw(x, y);
		break;
	case Parchment:
		// 羊皮紙
		parchmentSprite.resized(inventoryMini, inventoryMini).draw(x, y);
		break;
	case Hanger:
		// 針金
		hangerSprite.resized(inventoryMini, inventoryMini).draw(x, y);
		break;
	case DirtyCloth:
		// 汚れた布
		dirtyClothSprite.resized(inventoryMini, inventoryMini).draw(x, y);
		break;
	case Cloth:
		// 布
		clothSprite.resized(inventoryMini, inventoryMini).draw(x, y);
		break;
	case ToastedParchment:
		// 炙った羊皮紙
		toastedParchmentSprite.resized(inventoryMini, inventoryMini).draw(x, y);
		break;
	case WireKey:
		// 針金の鍵
		wireKeySprite.resized(inventoryMini, inventoryMini).draw(x, y);
		break;
	case IronKey:
		// 鉄製の鍵
		ironKeyMiniSprite.draw(x, y);
		break;
	case GoldKey:
		// 黄金の鍵
		goldKeySprite.resized(inventoryMini, inventoryMini).draw(x, y);
		break;
	}
}

void CameraTest::drawBigItem(
	int itemId,
	int x,
	int y
) const
{
	switch (itemId)
	{
	case Bread:
		// パン
		breadBigSprite.draw(x, y);
		break;
	case Memo:
		// 手記
		memoSprite.resized(inventoryBig, inventoryBig).draw(x, y);
		break;
	case RustedKey:
		// 錆びた鍵
		rustedKeySprite.resized(inventoryBig, inventoryBig).draw(x, y);
		break;
	case Poker:
		// 火かき棒
		pokerSprite.resized(inventoryBig, inventoryBig).draw(x, y);
		break;
	case Parchment:
		// 羊皮紙
		parchmentSprite.resized(inventoryBig, inventoryBig).draw(x, y);
		break;
	case Hanger:
		// ハンガー
		hangerSprite.resized(inventoryBig, inventoryBig).draw(x, y);
		break;
	case DirtyCloth:
		// 汚れた布
		dirtyClothSprite.resized(inventoryBig, inventoryBig).draw(x, y);
		break;
	case Cloth:
		// 布
		clothSprite.resized(inventoryBig, inventoryBig).draw(x, y);
		break;
	case ToastedParchment:
		// 炙った羊皮紙
		toastedParchmentSprite.resized(inventoryBig, inventoryBig).draw(x, y);
		break;
	case WireKey:
		// 針金の鍵
		wireKeySprite.resized(inventoryBig, inventoryBig).draw(x, y);
		break;
	case IronKey:
		// 鉄製の鍵
		ironKeyBigSprite.draw(x, y);
		break;
	case GoldKey:
		// 黄金の鍵
		goldKeySprite.resized(inventoryBig, inventoryBig).draw(x, y);
		break;
	}
}

void CameraTest::inventoryOnOff()
{
	if (bStartPlaying == false)
	{
		// プロローグ中は何もしない
		return;
	}

	// インベントリフラグの切り替え
	bInventory = bInventory ? false : true;

	// いったんカーソルを強制的に中央に戻す
	Cursor::SetPos(center.x, center.y);

	// フラグはオフにする TODO 増えたら困る
	bMemo = false;
	bToastedParchmentRead = false;
	bClothRead = false;
	bRustedKeyUse = false;

	// 合成の選択を解除
	synthesisIndex = -1;

	// ロックオンをオフ
	bLockon = false;

	// SE
	//if (AudioAsset(U"Inventory").isPlaying())
	//{
	//	AudioAsset(U"Inventory").stop();
	//}
	//AudioAsset(U"Inventory").setVolume(1.0);
	//AudioAsset(U"Inventory").play();
	playSE(U"Inventory");
}


void CameraTest::update()
{
	const double deltaTime = Scene::DeltaTime();
	
	if (bLoaded == false)
	{
		return;
	}

#ifdef _DEBUG
	// デバッグ表示
	debug();
#endif

	// シナリオの進捗によってメッセージを変える
	if (bStartPlaying)
	{
		if (bRustedKeyUse)
		{
			// 例外：錆びた鍵を使ったとき
			messagePattern = 0;
		}
		else if (priorityMessageCount > 0)
		{
			// 優先メッセージを表示
			message = priorityMessage;
			priorityMessageCount -= deltaTime;
			messagePattern = 0;

			// 例外：火かき棒で鉄製の鍵を入手したメッセージの後、【鉄製の鍵】を入手したというメッセージを表示
			if (priorityMessageCount <= 0 && message == 71 )
			{
				// SEを鳴らす
				playSEandBGMStop(U"Item");

				priorityMessage = 87;
				priorityMessageCount = priorityMessageCountMax;
			}
		}
		else if (bLockon)
		{
			// 何か見ているときは更新しない
			if (bookingMessage != message)
			{
				// 違うオブジェクトにカーソルが移動したら、いったんメッセージを非表示にする
				message = 3;
			}
		}
		else
		{
			message = scenario;
			messagePattern = 0;
		}
	}

	// 指定したプレイヤーインデックスの XInput コントローラを取得
	auto xboxController = XInput(playerIndex);
	xboxController.setLeftTriggerDeadZone();
	xboxController.setRightTriggerDeadZone();
	xboxController.setLeftThumbDeadZone();
	xboxController.setRightThumbDeadZone();

	// マウスのRayを取得
	Ray ray = getMouseRay();

	// 移動速度
	double addSpeed = 1.0;
	if ((KeyControl | KeyCommand).pressed() || xboxController.rightTrigger > 0.1)
	{
		// コントロールキーを押しているときの速度
	//	addSpeed = 20.0;
		addSpeed = 5.0;
	}
	else if (KeyShift.pressed() || xboxController.buttonRB.pressed())
	{
		// シフトキーを押しているときの速度
	//	addSpeed = 5.0;
		addSpeed = 2.5;
	}
	const double scaledSpeed = cameraSpeed * addSpeed * deltaTime;

	// マウスの座標を取得
	double diffMousePosX = 0.0f;
	double diffMousePosY = 0.0f;

	if (bStartPlaying == false)
	{
		// プロローグ中
		if (Window::GetState().focused)
		{
			// マウスカーソルを非表示
			Cursor::RequestStyle(CursorStyle::Hidden);

			// カーソルを強制的に中央に戻す
			Cursor::SetPos(center.x, center.y);
		}
	}
	else if (bInventory == false)
	{
		// インベントリを表示していない（通常時）

		// このアプリケーションがアクティブな時だけ
		if (Window::GetState().focused)
		{
			// マウスカーソルを非表示
			Cursor::RequestStyle(CursorStyle::Hidden);

			// 現在のマウス座標
			Vec2 currentCursorPos = Cursor::PosF();

			// 中心からの差分を取る
			Vec2 delta = currentCursorPos - center;

			// 仮想座標に加算
			virtualCursorPos += delta;

			toMousePosX = virtualCursorPos.x;
			toMousePosY = virtualCursorPos.y;

			diffMousePosX = (toMousePosX - mousePosX) / 10 * mouseDirectionX;
			diffMousePosY = -(toMousePosY - mousePosY) / 10 * mouseDirectionY;

			mousePosX = toMousePosX;
			mousePosY = toMousePosY;

			// 毎フレーム、カーソルを強制的に中央に戻す
			Cursor::SetPos(center.x, center.y);
		}


		// プレイヤーの移動
		if (bDrawerMode || bStoneMode)
		{
			// 引き出し、石板モード中は操作できない
		}
		else
		{
			if (KeyLeft.pressed())
			{
				phiController.rotate(60_deg, deltaTime, 1.0);
			}

			if (xboxController.rightThumbX < -0.1)
			{
				phiController.rotate(60_deg, deltaTime, -xboxController.rightThumbX * xboxCameraSpeed);
			}

			if (diffMousePosX < -0.1)
			{
				phiController.rotate(60_deg, deltaTime, -diffMousePosX);
			}

			if (KeyRight.pressed())
			{
				phiController.rotate(-60_deg, deltaTime, 1.0);
			}

			if (xboxController.rightThumbX > 0.1)
			{
				phiController.rotate(-60_deg, deltaTime, xboxController.rightThumbX * xboxCameraSpeed);
			}

			if (diffMousePosX > 0.1)
			{
				phiController.rotate(-60_deg, deltaTime, diffMousePosX);
			}
		}

		if (bDrawerMode)
		{
			// 引き出しモード中

			//if (KeyW.down())
			//{
			//	drawerIndex--;
			//	if (drawerIndex < 0)
			//	{
			//		drawerIndex = 0;
			//	}
			//}
			//if (KeyS.down())
			//{
			//	drawerIndex++;
			//	if (drawerIndex > 5)
			//	{
			//		drawerIndex = 5;
			//	}
			//}

			if (priorityMessageCount <= 0)
			{
				if (bDrawerPullNow == false && bDrawerClear == false)
				{
					// 引き出しを開け閉めしていない時だけ操作可能

					if (-0.1 < xboxController.leftThumbX && xboxController.leftThumbX < 0.1)
					{
						xboxControllerDown = false;
					}

					if (KeyA.down() || xboxController.buttonLeft.down())
					{
						drawerIndex--;
						if (drawerIndex < 0)
						{
							drawerIndex = 0;
						}
						bDrawerNone = false;
					}
					if (xboxController.leftThumbX < -0.1 && xboxControllerDown == false)
					{
						xboxControllerDown = true;

						// TODO 共通化
						drawerIndex--;
						if (drawerIndex < 0)
						{
							drawerIndex = 0;
						}
						bDrawerNone = false;
					}

					if (KeyD.down() || xboxController.buttonRight.down())
					{
						drawerIndex++;
						if (drawerIndex > 5)
						{
							drawerIndex = 5;
						}

						bDrawerNone = false;
					}
					if (xboxController.leftThumbX > 0.1 && xboxControllerDown == false)
					{
						xboxControllerDown = true;

						// TODO 共通化
						drawerIndex++;
						if (drawerIndex > 5)
						{
							drawerIndex = 5;
						}

						bDrawerNone = false;
					}
				}

				//	message = 38 + drawerIndex;
				message = 39 + drawerIndex;

				// 「無」をクリックした
				if (bDrawerNone)
				{
					message = 68;
				}
			}
		}
		else if (bStoneMode)
		{
			// 石板モード中
			if (priorityMessageCount <= 0)
			{

				if (bStonePullNow == false && bStoneclear == false)
				{
					if (-0.1 < xboxController.leftThumbX && xboxController.leftThumbX < 0.1)
					{
						xboxControllerDown = false;
					}

					if (KeyA.down() || xboxController.buttonLeft.down())
					{
						stoneIndex--;
						if (stoneIndex < 0)
						{
							stoneIndex = 0;
						}
					}
					if (xboxController.leftThumbX < -0.1 && xboxControllerDown == false)
					{
						xboxControllerDown = true;

						// TODO 共通化
						stoneIndex--;
						if (stoneIndex < 0)
						{
							stoneIndex = 0;
						}
					}

					if (KeyD.down() || xboxController.buttonRight.down())
					{
						stoneIndex++;
						if (stoneIndex > 4)
						{
							stoneIndex = 4;
						}
					}
					if (xboxController.leftThumbX > 0.1 && xboxControllerDown == false)
					{
						xboxControllerDown = true;

						// TODO 共通化
						stoneIndex++;
						if (stoneIndex > 4)
						{
							stoneIndex = 4;
						}
					}
				}

				message = 60 + stoneIndex;
			}
		}
		else
		{
			const double xr = (scaledSpeed * sss);
			const double zr = (scaledSpeed * ccc);

			bool isWalk = false;
			if (KeyW.pressed() || xboxController.buttonUp.pressed())
			{
				toCameraPos.x += xr;
				toCameraPos.z += zr;
				isWalk = true;
			}
			if (xboxController.leftThumbY > 0.1)
			{
				toCameraPos.x += (xr * xboxController.leftThumbY);
				toCameraPos.z += (zr * xboxController.leftThumbY);
				isWalk = true;
			}

			if (KeyS.pressed() || xboxController.buttonDown.pressed())
			{
				toCameraPos.x -= xr;
				toCameraPos.z -= zr;
				isWalk = true;
			}
			if (xboxController.leftThumbY < -0.1)
			{
				toCameraPos.x -= (xr * -xboxController.leftThumbY);
				toCameraPos.z -= (zr * -xboxController.leftThumbY);
				isWalk = true;
			}

			if (KeyA.pressed() || xboxController.buttonLeft.pressed())
			{
				toCameraPos.x -= zr;
				toCameraPos.z += xr;
				isWalk = true;
			}
			if (xboxController.leftThumbX < -0.1)
			{
				toCameraPos.x -= (zr * -xboxController.leftThumbX);
				toCameraPos.z += (xr * -xboxController.leftThumbX);
				isWalk = true;
			}

			if (KeyD.pressed() || xboxController.buttonRight.pressed())
			{
				toCameraPos.x += zr;
				toCameraPos.z -= xr;
				isWalk = true;
			}
			if (xboxController.leftThumbX > 0.1)
			{
				toCameraPos.x += (zr * xboxController.leftThumbX);
				toCameraPos.z -= (xr * xboxController.leftThumbX);
				isWalk = true;
			}

#ifdef _DEBUG
			if (KeyR.pressed())
			{
				toCameraPos.y += scaledSpeed;
			}
			if (KeyF.pressed())
			{
				toCameraPos.y -= scaledSpeed;
			}
#endif

			footcount += deltaTime * ((float)addSpeed / 2);

			if (isWalk)
			{
				
				if (footcount > 0.3 && lastCheckCollision == false)
				{
					// 鳴らす
					if (footpattern % 2 == 0)
					{
						if (AudioAsset(U"footsteps1").isPlaying())
						{
							AudioAsset(U"footsteps1").stop();
						}
						AudioAsset(U"footsteps1").setVolume(1.0);
						AudioAsset(U"footsteps1").play();
					}
					else
					{
						if (AudioAsset(U"footsteps2").isPlaying())
						{
							AudioAsset(U"footsteps2").stop();
						}
						AudioAsset(U"footsteps2").setVolume(1.0);
						AudioAsset(U"footsteps2").play();
					}

					// 足音ストップ
					// TODO 共通化
				//	if (AudioAsset(U"footsteps1").isPlaying()) {
				//		AudioAsset(U"footsteps1").stop();
				//		footpattern++;
				//	}
				//	if (AudioAsset(U"footsteps2").isPlaying()) {
				//		AudioAsset(U"footsteps2").stop();
				//		footpattern++;
				//	}
					footpattern++;
					footcount = 0;
				}

				// はぁはぁ
				if (addSpeed >= 2.0 && lastCheckCollision == false)
				{
					breathCount += deltaTime * addSpeed;

					if (breathCount >= 15)
					{
						// はぁはぁのSE
						if (AudioAsset(U"breath_pant").isPlaying() == false)
						{
							// 鳴ってなかったら鳴らす
							AudioAsset(U"breath_pant").setVolume(0.5);
							AudioAsset(U"breath_pant").play();
						}
					}
				}
				else
				{
					// 歩いているとき（走っていない）
					
					// はぁはぁリセット
					if (breathCount > 0)
					{
						breathCount -= deltaTime * 3;
					}
				}
			}
			else {
				//if (AudioAsset(U"footsteps1").isPlaying()) {
				//	AudioAsset(U"footsteps1").stop();
				//	footpattern++;
				//}
				//if (AudioAsset(U"footsteps2").isPlaying()) {
				//	AudioAsset(U"footsteps2").stop();
				//	footpattern++;
				//}
				//footcount = 0;

				//stopwatch.restart();

				if (breathCount >= 30)
				{
					AudioAsset(U"breath_pant").stop();
					if (AudioAsset(U"breath_deep").isPlaying() == false)
					{
						AudioAsset(U"breath_deep").setVolume(0.5);
						AudioAsset(U"breath_deep").play();
					}
				}
				// はぁはぁリセット
				if (breathCount > 0)
				{
					breathCount -= deltaTime * 5;
				}
			}
		}

		if (bDrawerMode || bStoneMode)	// TODO
		{
			// 引き出しモード中、石板モード中は操作できない
		}
		else
		{
			const double yDelta = deltaTime;

			if (KeyUp.pressed())
			{
				to_m_focusY += yDelta;
			}

			if (xboxController.rightThumbY > 0.1)
			{
				to_m_focusY += (yDelta * xboxController.rightThumbY * xboxCameraSpeed);
			}

			if (diffMousePosY > 0.1)
			{
				to_m_focusY += (yDelta * diffMousePosY);
			}


			if (KeyDown.pressed())
			{
				to_m_focusY -= yDelta;
			}

			if (xboxController.rightThumbY < -0.1)
			{
				to_m_focusY -= (yDelta * -xboxController.rightThumbY * xboxCameraSpeed);
			}

			if (diffMousePosY < -0.1)
			{
				to_m_focusY -= (yDelta * -diffMousePosY);
			}

			// カメラ上下の可動域
			if (to_m_focusY < -focusY_max)
			{
				to_m_focusY = -focusY_max;
			}
			else if (to_m_focusY > focusY_max)
			{
				to_m_focusY = focusY_max;
			}
		}

		// ロックオン
		lockon();

		// メッセージパターンの自動変更
	//	messagePatternCount += deltaTime;

		// メッセージを切り替える
		if (bLockon)
		{
			if (MouseL.down() || xboxController.buttonA.down())
			{
				if (lastMessage != message)
				{
					messagePattern = -1;
				}
				lastMessage = message;

				messagePattern++;
				if (messagePattern >= MessagePatternMax * 2)
				{
					messagePattern = 0;
				}

				// 下と同じ条件ですが、クリックしたときは０に戻したいため
				int m = message * MessagePatternMax + messagePattern / 2;
				if (Text[m].isEmpty())
				{
					// メッセージが空の場合、０に戻す
					messagePattern = 0;
				}
			}
		}

		if (messagePattern >= 0)
		{
			int m = message * MessagePatternMax + messagePattern / 2;
			if (Text[m].isEmpty())
			{
				// メッセージが空の場合、０に戻す
				messagePattern = -1;
			}
		}
		



		//if (message == 14)
		//{
		//	if (MouseL.down())
		//	{
		//		messagePattern++;
		//		messagePattern %= 3;
		//	}

		//	// ベッド
		//	switch (messagePattern)
		//	{
		//	case 0:
		//		message = 23;
		//		break;
		//	case 1:
		//		message = 24;
		//		break;
		//	case 2:
		//		message = 25;
		//		break;
		//	}
		//}

	}
	else
	{
		// インベントリを表示している

		// 足音の削除
		if (AudioAsset(U"footsteps1").isPlaying()) {
			AudioAsset(U"footsteps1").stop();
		}

		if (KeyUp.down())
		{
			itemIndex -= 4;

			if (itemIndex < 0)
			{
				itemIndex += 12;
			}
		}

		if (KeyDown.down())
		{
			itemIndex += 4;

			if (itemIndex >= 12)
			{
				itemIndex -= 12;
			}
		}

		if (KeyLeft.down())
		{
			itemIndex -= 1;
			if (itemIndex < 0)
			{
				itemIndex += 12;
			}
		}

		if (KeyRight.down())
		{
			itemIndex += 1;
			if (itemIndex >= 15)
			{
				itemIndex -= 12;
			}
		}



	}

	// インベントリの表示・非表示
//	if (KeyI.down()
	if (KeySpace.down()
		|| xboxController.buttonY.down()
	)
	{
		inventoryOnOff();
	}

	// コリジョンを無効にするエリア
	for (int i = 0; i < DoorNum; i++)
	{
		if (bDoorOpen[i] == false)
		{
			continue;
		}

		if ( collisionNone[i][0] < toCameraPos.x
		  && toCameraPos.x < collisionNone[i][1]
		  && collisionNone[i][2] < toCameraPos.z
		  && toCameraPos.z < collisionNone[i][3]
		)
		{
			// コリジョンを無効にするエリア
			bCollisionDoor = false;
			break;
		}
		else
		{
			bCollisionDoor = true;
		}
	}

	// ライトエリア
	//int tmpLightArea = 0;
	//for (auto& row : collisionLight)
	//{
	//	if (row[0] < toCameraPos.x
	//		&& toCameraPos.x < row[1]
	//		&& row[2] < toCameraPos.z
	//		&& toCameraPos.z < row[3]
	//		)
	//	{
	//		// エリアの変更
	//		if (lightArea == lastLightArea)
	//		{
	//		//	lightTime = 2;
	//			lightTime = 1;
	//		//	lightTime = 0.5;
	//		}
	//		lightArea = tmpLightArea;
	//		break;
	//	}
	//	tmpLightArea++;
	//}

	Vec3_ myPosition = {
		toCameraPos.x,
		myRadius+0.1,
		toCameraPos.z
	};

	// 線分交差で判定する
	if (bCollision && bCollisionDoor)
	{
		Vec3 A{
			lastCameraPosition.x,
			lastCameraPosition.y,
			lastCameraPosition.z
		};

		Vec3 cameraNormal = Vec3(
			toCameraPos.x - lastCameraPosition.x,
			toCameraPos.y - lastCameraPosition.y,
			toCameraPos.z - lastCameraPosition.z
		).normalized();

		// 向かっている位置より少し前にコリジョンを持たせる
		Vec3 B{
			toCameraPos.x + cameraNormal.x / 2,
			toCameraPos.y + cameraNormal.y / 2,
			toCameraPos.z + cameraNormal.z / 2,
		};

		Segment segment = {
			{
				lastCameraPosition.x,
				0.1,
				lastCameraPosition.z
			}
			, 
			{
				toCameraPos.x + cameraNormal.x / 2,
				0.1,
				toCameraPos.z + cameraNormal.z / 2,
			}
		};

		const ColorF LineColor = ColorF{ 0.3, 0.2, 0.0 }.removeSRGBCurve();

		Line3D{ Vec3{ lastCameraPosition.x, lastCameraPosition.y, lastCameraPosition.z }, Vec3{ toCameraPos.x, toCameraPos.y, toCameraPos.z } }.draw(LineColor);

		// モデルデータと判定する
		lastCheckCollision = false;
		bool checkCollision = false;
		for (const auto& object : model->objects())
		{
			const std::array<Vec3, 8> cube = object.boundingBox.getCorners();

			for (int i = 0; i < 12; i++)
			{
				Triangle_ tri = {
					Vec3_{cube[collisionTriangle[i][0]].x / 100, cube[collisionTriangle[i][0]].y / 100, cube[collisionTriangle[i][0]].z / 100},
					Vec3_{cube[collisionTriangle[i][1]].x / 100, cube[collisionTriangle[i][1]].y / 100, cube[collisionTriangle[i][1]].z / 100},
					Vec3_{cube[collisionTriangle[i][2]].x / 100, cube[collisionTriangle[i][2]].y / 100, cube[collisionTriangle[i][2]].z / 100}
				};

				HitResult hit;

				Vec3_ triA = { cube[collisionTriangle[i][0]].x / 100, cube[collisionTriangle[i][0]].y / 100, cube[collisionTriangle[i][0]].z / 100 };
				Vec3_ triB = { cube[collisionTriangle[i][1]].x / 100, cube[collisionTriangle[i][1]].y / 100, cube[collisionTriangle[i][1]].z / 100 };
				Vec3_ triC = { cube[collisionTriangle[i][2]].x / 100, cube[collisionTriangle[i][2]].y / 100, cube[collisionTriangle[i][2]].z / 100 };

				if (IsSphereIntersectingTriangle(myPosition, myRadius, triA, triB, triC))
				{
#ifdef _DEBUG
					Print << U"交差しました！";
#endif
					// 交差している（ぶつかった）
					checkCollision = true;

					// いったん止める
					toCameraPos = lastCameraPosition;

					break;





					// プレイヤーの移動速度
					Vec2 velocity = Vec2(
						toCameraPos.x - lastCameraPosition.x,
						toCameraPos.z - lastCameraPosition.z
					);
					double length = velocity.length();

					Vec2 velocityNormal = Vec2(
						toCameraPos.x - lastCameraPosition.x,
						toCameraPos.z - lastCameraPosition.z
					).normalized();

					// 当たり判定の法線
					Vec2 wallNormal = Vec2(
						cube[collisionList[i][0]].x - cube[collisionList[i][1]].x,
						cube[collisionList[i][0]].z - cube[collisionList[i][1]].z
					).normalized();

					// 壁の法線に対する接線方向を計算
					//Vec2 tangent = Vec2(wallNormal.x, wallNormal.y); // 法線に直交するベクトル

					// 今の速度を壁に沿ってスライド（内積で投影）
					//double dot = (velocityNormal.dot(wallNormal));

					// 壁の法線に対する接線方向を計算
					Vec2 tangent = Vec2(wallNormal.y, -wallNormal.x); // 法線に直交するベクトル

					// 今の速度を壁に沿ってスライド（内積で投影）
					Vec2 resultVelocity = tangent * (velocityNormal.dot(tangent));

					Vec2 resultVelocity2 = Vec2(
						resultVelocity.y,
						resultVelocity.x
					);

					double dot = (velocityNormal.dot(resultVelocity2));

					//if (dot > 0)
					//{
					//	// 前に進む
					//}
					//else if (dot < 0)
					//{
					//	// 後ろに進む
					//	// 進行方向を反転
					//	resultVelocity2.x *= -1;
					//	resultVelocity2.y *= -1;
					//}
#ifdef _DEBUG
					Print << U"dot " << dot;
#endif
					// いったん前の状態に戻す
					toCameraPos = lastCameraPosition;

					// 壁に沿ってスライド
					toCameraPos.x += (resultVelocity2.x * length * dot);
					toCameraPos.z += (resultVelocity2.y * length * dot);


					// TODO ここから先、関数化する
					
					// 進んだ先にコリジョンがないかどうか
					Vec2 AA{
						lastCameraPosition.x,
						lastCameraPosition.z
					};

					// 向かっている位置より少し前にコリジョンを持たせる TODO これじゃなくて、距離にしたい
					Vec2 cameraNormal2 = Vec2(
						toCameraPos.x - lastCameraPosition.x,
						toCameraPos.z - lastCameraPosition.z
					).normalized();

					Vec2 BB{
						toCameraPos.x + cameraNormal2.x / 2,
						toCameraPos.z + cameraNormal2.y / 2,
					};

					const auto& object2Temp = model->objects();
					for (const auto& object2 : object2Temp)
					{
						const std::array<Vec3, 8> cube2 = object2.boundingBox.getCorners();

						for (int i2 = 0; i2 < 12; i2++)
						{
							if (
								isIntersecting(
									AA,
									BB,
									Vec2{ cube2[collisionList[i2][0]].x / 100, cube2[collisionList[i2][0]].z / 100 },
									Vec2{ cube2[collisionList[i2][1]].x / 100, cube2[collisionList[i2][1]].z / 100 }
								)
							)
							{
								// 交差している（ぶつかった）失敗
								checkCollision = false;
								toCameraPos = lastCameraPosition;
								break;
						
							}
						}
						if (checkCollision == false)
						{
							// 進んだ先にコリジョンがある 失敗
							break;
						}
					}


					break;
				}
			}
			if (checkCollision)
			{
				// 正しいコリジョン
				lastCheckCollision = true;
				break;
			}
		}
	}

	// 引き出しモード
	if (bDrawerMode && bInventory == false)
	{
		if (MouseL.up() || xboxController.buttonA.up())
		{
			// いっかいマウスを離してから有効にする
			bMouseL = true;
		}

		if (MouseL.down() || xboxController.buttonA.down())
		{
			if (bMouseL)
			{
				// クリックした
				if (message == 88 && priorityMessageCount > 0)
				{
					priorityMessageCount = 0;
				}
				else if (drawerIndex == 5)
				{
					// 引き出しモード解除
					bDrawerMode = false;

					toCameraPos.x = drawerPos[0].x;
					toCameraPos.y = 1.5;
					toCameraPos.z = drawerPos[0].z - 2.0;

					to_m_focusY = -0.3;

					//	if (bDrawerClear == false)
					//	{
							// 各引き出しを元の位置に戻す
						//	for (int i = 0; i < 6; i++)
					for (int i = 0; i < 5; i++)
					{
						//	toDrawerPos[i + 1].z = drawerPos[i + 1].z;	// これだと開けっ放しになる
						toDrawerPos[i + 1].z = 1.49;
						drawerPull[i] = false;
					}

					drawerCounter = 0;
					drawerOrder = 0;
					//	}

					bMouseL = false;
				}
				else if (drawerPull[drawerIndex] == false)
				{
					//if (drawerIndex == 0)
					//{
					//	// 一番上の引き出し
					////	if (drawerOrder == 51432)
					//	if (bDrawerOpen == true)
					//	{
					//		// 開けた（クリア）
					//		toDrawerPos[drawerIndex + 1].z -= 0.2;
					//		drawerPull[drawerIndex] = true;

					//	//	items << GoldKey;

					//		bDrawerClear = true;

					//		playSEandBGMStop(U"Item");

					//		// ゲーム画面に戻るためのカウントとして使う
					//		drawerPullCount = 1.0;

					//		bMouseL = false;
					//	}
					//	else
					//	{
					//		// しまっている
					//		bDrawerNone = bDrawerNone ? false : true;
					//	}
					//}
					//else
					{
						// それ以外の引き出し
						toDrawerPos[drawerIndex + 1].z -= 0.2;
						drawerPull[drawerIndex] = true;
						drawerPullCount = 1.0;
						bDrawerPullNow = true;

						if (drawerIndex == 1)
						{
							// いったんリセット
							drawerOrder = 0;
							drawerCounter = 0;
						}

						if (lastDrawerIndex != drawerIndex)
						{
							drawerOrder += (drawerIndex + 1) * std::pow(10, drawerCounter);
							drawerCounter++;

							//	if (drawerIndex == 5 && drawerOrder == 51432 && bDrawerOpen == false)
							if (drawerIndex == 4 && drawerOrder == 51432 && bDrawerOpen == false)
							{
								//	playSE(U"牢屋の扉を開ける");
								bDrawerOpen = true;

								// 開けた（クリア）
							//	toDrawerPos[drawerIndex + 1].z -= 0.2;
							//	drawerPull[drawerIndex] = true;

								bDrawerClear = true;

							//	playSEandBGMStop(U"Item");
								playSEandBGMStop(U"lock_wood_door1");

								// ゲーム画面に戻るためのカウントとして使う
								drawerPullCount = 1.0;

								bMouseL = false;

								priorityMessage = 89;
								priorityMessageCount = priorityMessageCountMax;
							}
							else
							{
								playSE(U"drawer_open");
							}
						}
						else
						{
							playSE(U"drawer_open");
						}

						lastDrawerIndex = drawerIndex;
					}
				}
			}
		}
	}

	// 引き出しを閉める処理
	if (drawerPullCount > 0)
	{
		drawerPullCount -= deltaTime;

		if (drawerPullCount <= 0)
		{
			// 閉じる
		//	for (int i = 1; i < 6; i++)
			for (int i = 0; i < 5; i++)
			{
				if (drawerPull[i] == true)
				{
					toDrawerPos[i + 1].z += 0.2;
					//if (!AudioAsset(U"drawer_close").isPlaying())
					//{
					//	AudioAsset(U"drawer_close").setLoop(false);
					//	AudioAsset(U"drawer_close").setVolume(1.0);
					//	AudioAsset(U"drawer_close").play();
					//}
					playSE(U"drawer_close");
					drawerPull[i] = false;
					bDrawerPullNow = false;
				}
			}

			if (bDrawerOpen && bDrawerClear && bDrawerMode)
			{
				// 引き出しモード解除 TODO 共通化
				bDrawerMode = false;

				toCameraPos.x = drawerPos[0].x;
				toCameraPos.y = 1.5;
				toCameraPos.z = drawerPos[0].z - 2.0;

				to_m_focusY = -0.3;

				bMouseL = false;
			}
		}
	}

	// 石板モード
	if (bStoneMode && bInventory == false)
	{
		if (MouseL.up() || xboxController.buttonA.up())
		{
			// いっかいマウスを離してから有効にする
			bMouseL = true;
		}

		if (MouseL.down() || xboxController.buttonA.down())
		{
			if (bMouseL)
			{
				// クリックした
				if (message == 90 && priorityMessageCount > 0)
				{
					priorityMessageCount = 0;
				}
				else if (stoneIndex == 4)	// 戻るを選択
				{
					// 石板モード解除
					bStoneMode = false;

					toCameraPos.x = stonePos[0].x - 2.0;
					toCameraPos.y = 1.5;
					toCameraPos.z = stonePos[0].z;

					to_m_focusY = -0.3;

					if (bStoneclear == false)
					{
						for (int i = 0; i < 4; i++)
						{
							toStonePos[i + 1].x = 20.18;
							stonePull[i] = false;
						}

						stoneCounter = 0;
						stoneOrder = 0;
					}

					bMouseL = false;
				}
				else if (stonePull[stoneIndex] == false)
				{
					// 石板の移動
					toStonePos[stoneIndex + 1].x += 0.02;
					stonePull[stoneIndex] = true;

					stoneOrder += (stoneIndex + 1) * std::pow(10, stoneCounter);
					stoneCounter++;

					bStonePullNow = true;

					// 最後の石板
					if (stoneOrder == 4132)
					{
						// 石板クリア

					//	items << GoldKey;

					//	bGoldKeyHave = true;

						bStoneclear = true;

						//playSEandBGMStop(U"Item");
						playSEandBGMStop(U"lock_wood_door1");

						stonePushCount = 1.0;

						bMouseL = false;

						priorityMessage = 91;
						priorityMessageCount = priorityMessageCountMax;
					}
					else
					{
						// それ以外の石板
						playSE(U"Stone");
					}
				}
			}
		}
	}

	// 石板からゲーム画面に自動で戻る処理
	if (stonePushCount > 0)
	{
		stonePushCount -= deltaTime;

		if (stonePushCount <= 0)
		{
			// 石板モード解除 TODO 共通化
			bStoneMode = false;

			toCameraPos.x = stonePos[0].x - 2.0;
			toCameraPos.y = 1.5;
			toCameraPos.z = stonePos[0].z;

			to_m_focusY = -0.3;

			bMouseL = false;
		}
	}

	// 引き出しの移動
	for (int i = 1; i < 7; i++)
	{
	//	drawerPos[i].z = Math::Lerp(drawerPos[i].z, toDrawerPos[i].z, 0.1);
	//	drawerPos[i].z = Math::Lerp(drawerPos[i].z, toDrawerPos[i].z, 0.06);
		drawerPos[i].z = Math::Lerp(drawerPos[i].z, toDrawerPos[i].z, 0.065);
	}

	// 石板の移動
	bStonePullNow = false;
	for (int i = 1; i < 5; i++)
	{
		stonePos[i].x = Math::Lerp(stonePos[i].x, toStonePos[i].x, 0.06);

		if (stonePos[i].x + 0.003 < toStonePos[i].x)
		{
			bStonePullNow = true;
		}
	}

	// カメラ関係
	const double to_s = (Math::Cos(phiController.getPhi()));
	const double to_c = (Math::Sin(phiController.getPhi()));

	sss = Math::Lerp(sss, to_s, smooth); // 回転もスムーズに
	ccc = Math::Lerp(ccc, to_c, smooth); // 回転もスムーズに

	// ゆっくり移動
	curCameraPosition = curCameraPosition.lerp(toCameraPos, smooth);

	// 線分交差の判定用に保存
	lastCameraPosition = toCameraPos;

	// 回転もスムーズに
	m_focusY = Math::Lerp(m_focusY, to_m_focusY, smooth); 

	// カメラ
	Vec3 focusVector { sss, m_focusY, ccc };

	camera.setProjection(
		Graphics3D::GetRenderTargetSize(),
		//m_verticalFOV - zoom,
		m_verticalFOV,
		m_nearClip
	);

	camera.setView(curCameraPosition, (curCameraPosition + focusVector), m_upDirection);

	Graphics3D::SetCameraTransform(camera);


	// 扉の回転
//	doorRot.y = Math::Lerp(doorRot.y, toDoorRotY, smooth);
//	doorRot.y = Math::Lerp(doorRot.y, toDoorRotY, smooth/10);	// 扉はゆっくり開ける

	// 扉の移動
	doorPos.x = Math::Lerp(doorPos.x, toDoorPosX, smooth / 10);	// 扉はゆっくり開ける
	door2Pos.x = Math::Lerp(door2Pos.x, toDoor2PosX, smooth / 10);	// 扉はゆっくり開ける





	//if (lightArea != lastLightArea)
	//{
	//	// ライトの位置が変わった

	//	toGlobalAmbientColorR = 0.1;
	//	toGlobalAmbientColorG = 0.1;
	//	toGlobalAmbientColorB = 0.125;

	//	lightTime -= deltaTime;

	//	if (lightTime < 0)
	//	{
	//		lastLightArea = lightArea;

	//		isGlowEffect = true;
	//	}
	//}
	//else
	//{
		// ライト位置
		lightPos = curCameraPosition;
		//lightPos = lightPosList[lightArea];
		//switch (lightArea)
		//{
		//case 0:
		//	lightPos.x = 0;
		//	lightPos.y = 2.084;
		//	lightPos.z = 7;
		//	break;
		//case 1:
		//	lightPos.x = 4.26;
		//	lightPos.y = 2.60;
		//	lightPos.z = -0.93;
		//	break;
		//case 2:
		//	lightPos.x = 16.05;
		//	lightPos.y = 2.60;
		//	lightPos.z = -0.92;
		//	break;
		//case 3:
		//	lightPos.x = 16.05;
		//	lightPos.y = 2.60;
		//	lightPos.z = -4.27;
		//	break;
		//}

		// ライトの点滅 TODO いつか使うかも？
		//if (Random(0, 100) == 0)
		//{
		//	glowEffectType++;
		//	if (glowEffectType >= 12)
		//	{
		//		glowEffectType = 0;
		//	}
		//}

		//if (glowEffectType == 9
		//	|| glowEffectType == 11
		//	)
		//{
		//	// 点滅
		//	if (Random(0, 5) == 0)
		//	{
		//		isGlowEffect = isGlowEffect ? false : true;
		//	}
		//}
		//else if (glowEffectType == 10)
		//{
		//	// 暗闇
		//	isGlowEffect = false;
		//}
		//else
		//{
		//	// 点灯
		//	isGlowEffect = true;
		//}

		// エミッシブ
		//if (isGlowEffect)
		//{
		//	toEmission = 1.0;
		//}
		//else
		//{
		//	toEmission = 0.0;
		//}

		//if (!bDebugFlashingLight)
		//{
		//	// 点滅しない場合は常に点灯
		//	isGlowEffect = true;
		//}

		// 光源の設定
		//if (isGlowEffect)
		//{
		//	// 点灯
		//	toGlobalAmbientColorR = 0.8;
		//	toGlobalAmbientColorG = 0.8;
		//	toGlobalAmbientColorB = 1.0;
		//}
		//else
		//{
		//	// 点灯していない
		//	toGlobalAmbientColorR = 0.1;
		//	toGlobalAmbientColorG = 0.1;
		//	toGlobalAmbientColorB = 0.125;
		//}
	//}

	//GlobalAmbientColorR = Math::Lerp(GlobalAmbientColorR, toGlobalAmbientColorR, lightSmooth);
	//GlobalAmbientColorG = Math::Lerp(GlobalAmbientColorG, toGlobalAmbientColorG, lightSmooth);
	//GlobalAmbientColorB = Math::Lerp(GlobalAmbientColorB, toGlobalAmbientColorB, lightSmooth);

	//Graphics3D::SetGlobalAmbientColor(
	//	ColorF{
	//		GlobalAmbientColorR,
	//		GlobalAmbientColorG,
	//		GlobalAmbientColorB
	//	}
	//);
	//Graphics3D::SetSunColor(
	//	ColorF{
	//		GlobalAmbientColorR,
	//		GlobalAmbientColorG,
	//		GlobalAmbientColorB
	//	}
	//);

	// 環境光を小さくする
	Graphics3D::SetGlobalAmbientColor(ColorF{ globalAmbientColor });
	
	// 太陽光をオフにする
	Graphics3D::SetSunColor(ColorF{ sunColor });

	// モデルの表示
	viewModel();

	// [RenderTexture を 2D シーンに描画]
	{
		Graphics3D::Flush();
		renderTexture.resolve();
		Shader::LinearToScreen(renderTexture);
	}

	// ライトの表示
#if _DEBUG
	//emission = Math::Lerp(emission, toEmission, smooth);

	//if (emission >= 0.2)
	//{ 
	//	const ScopedCustomShader2D shader{ psBright };
	//	const ScopedRenderTarget2D target{ gaussianA4.clear(ColorF{0.0}) };
	//	renderTexture.scaled(1).draw();

	//	PhongMaterial phong;
	//	phong.ambientColor = ColorF{ 1.0 };
	//	phong.diffuseColor = ColorF{ 0.0 };
	//	phong.emissionColor = ColorF{ 1.0, 1.0, 1.0 }.removeSRGBCurve() * (emission);
	//	Sphere{ lightPos, lightSize }.draw(phong);

		//const auto& materials = model.materials();
		//for (const auto& object : model.objects())
		//{
		//	if (object.name == U"FixRoom EV_Light01")
		//	{
		//		object.draw(materials);
		//	}
		//	Print << object.name;
		//}
		
		// 高輝度部分のぼかしテクスチャの生成
		//Shader::GaussianBlur(gaussianA4, gaussianB4, gaussianA4);
		//Shader::Downsample(gaussianA4, gaussianA8);
		//Shader::GaussianBlur(gaussianA8, gaussianB8, gaussianA8);
		//Shader::GaussianBlur(gaussianA8, gaussianB8, gaussianA8);
		//Shader::Downsample(gaussianA8, gaussianA16);
		//Shader::GaussianBlur(gaussianA16, gaussianB16, gaussianA16);
		//Shader::GaussianBlur(gaussianA16, gaussianB16, gaussianA16);
		//Shader::GaussianBlur(gaussianA16, gaussianB16, gaussianA16);
		
		// Glow エフェクト
		//const ScopedRenderStates2D blend{ BlendState::AdditiveRGB };
		//{
		//	const ScopedRenderTarget2D target{ gaussianA8 };
		//	gaussianA16.scaled(2.0).draw(ColorF{ 3.0 });
		//}

		//{
		//	const ScopedRenderTarget2D target{ gaussianA4 };
		//	gaussianA8.scaled(2.0).draw(ColorF{ 1.0 });
		//}

		//gaussianA4.resized(Scene::Size()).draw(ColorF{ 1.0 });
	//}
#endif

	// UI
	if (bInventory)
	{
		// インベントリ表示
		viewInventory();
	}
	else
	{
#if _DEBUG
		// デバッグ表示
		if (bDebugViewCollision)
		{
			// フォーカスエリア
			Rect rect(
				WINDOW_WIDTH / 2 - focusWidth,
				WINDOW_HEIGHT / 2 - focusHeight,
				focusWidth*2,
				focusHeight*2
			);

			rect.drawFrame(2, Palette::Red);
		}
#endif
	}

	if (bPrologueMessageEnd == false)
	{
		// プロローグカウンター
		prologueCount += deltaTime;

		//if (prologueCount > 29)
		//{
		//	// 2倍速
		//	prologueCount += deltaTime;
		//}

		if (MouseL.down() || xboxController.buttonA.down())
		{
			if (prologueCount < 28)
			{
				float prologueIndex = prologueCount * 5;
				prologueCount = 0;
				for (int i = 0; i < prologueText.size(); ++i)
				{
					prologueCount += (float)prologueText[i].size() / 5.0f;
					prologueIndex -= prologueText[i].size();

					if (prologueIndex <= 0)
					{
						break;
					}
				}
			}
		}

	//	if (prologueCount > 47)
		if (prologueCount > prologueTiming)
		{
			bPrologueMessageEnd = true;

			message = 35;
		}
	}

	if (bPrologueMessageEnd == true && bStartPlaying == false)
	{
		// プロローグカウンター2
		
		// 2倍速
	//	prologueCount += (deltaTime * 2);
		messageCount += deltaTime;
#ifdef _DEBUG
		Print << U"message*MessagePatternMax=" << message * MessagePatternMax;
		Print << U"Text[message*MessagePatternMax].size()=" << Text[message * MessagePatternMax].size();
		Print << U"message=" << message;
#endif
		if (MouseL.down() || xboxController.buttonA.down())
		{
			if (messageCount < Text[message * MessagePatternMax].size() / MessageSpeed - 0.8)
			{
				messageCount = Text[message * MessagePatternMax].size() / MessageSpeed - 0.8;
			}
		}

		if (message == 35 && messageCount > Text[message * MessagePatternMax].size() / MessageSpeed)
		{
			message = 36;
			messageCount = 0;
		}
		else if (message == 36 && messageCount > Text[message * MessagePatternMax].size() / MessageSpeed)
		{
			message = 37;
			messageCount = 0;
		}
		else if (message == 37 && messageCount > Text[message * MessagePatternMax].size() / MessageSpeed)
		{
			message = 0;
			messageCount = 0;
		}
		else if (message == 0 && messageCount > Text[message * MessagePatternMax].size() / MessageSpeed)
		{
			bStartPlaying = true;
			messageCount = 9999;
		}
	}

	//if (bPrologue2End)
	//{
	//	// メッセージカウンター
	//	messageCount += deltaTime;

	//	if (messageCount > 5)
	//	{
	//		bPrologueBGM = true;
	//	}
	//}

	if (bStartPlaying)
	{
		// 止まっているBGMを再度鳴らす
		{
			if (bgmStopCount <= 0.0f)
			{
				if (bMemo || bToastedParchmentRead || bClothRead)	// TODO 増えたら困る
				{
					// 文章を呼んでいるときは、BGMを再開させない
				}
				else
				{
					// BGMの再開
					if (!AudioAsset(U"BGM").isPlaying())
					{
						AudioAsset(U"BGM").setVolume(BGMVolume);
						AudioAsset(U"BGM").play();
					}

					if (!AudioAsset(U"bonfire").isPlaying())
					{
						AudioAsset(U"bonfire").setVolume(0.0);
						AudioAsset(U"bonfire").play();
					}
				}
			}
			else {
				bgmStopCount -= deltaTime;

				// 演出
				//if (scenario == 6 && lightArea == 1 && bgmStopCount > 4.1)
				//{
				//	bgmStopCount = 1.0;
				//}
			}
		}
	}

	// 暖炉のBGMの音量
	{
		double d = GetDistance(
			Vec2{ toCameraPos.x, toCameraPos.z },
			Vec2{ fireplacePos.x, fireplacePos.z }
		);
#if _DEBUG
//		Print << U"GetDistance=" << d;
#endif
		double Volume = 3 - d;
		if (Volume < 0)
		{
			Volume = 0;
		}
		// ノーマライズ
		Volume /= 3;

		AudioAsset(U"bonfire").setVolume(Volume);
	}

	//if (bPrologueBGM && messageCount > 0)
	//{
	//	bStartPlaying = true;
	//}

	// 経過時間を取得
	const double frameTime = stopwatch.sF();
	if (frameTime < targetDeltaTime)
	{
		// 残り時間だけスリープ（精度を高めたいなら Sleep せずにループで待機する方法もある）
		System::Sleep(targetDeltaTime - frameTime);
	}

	// タイマーをリセットして次のフレームへ
	stopwatch.restart();
}

void CameraTest::draw() const
{
	// 背景色
	Scene::SetBackground(ColorF{ 0, 0, 0 });

	if (bLoaded == false)
	{
	//	nowLoadingSprite.draw(SCENE_WIDTH - 520, SCENE_HEIGHT - 140);
	//	nowLoadingSprite.resized(384, 96).draw(SCENE_WIDTH - 384 - 10, SCENE_HEIGHT - 96 - 10);
		nowLoadingSprite.resized(256, 64).draw(SCENE_WIDTH - 256 - 10, SCENE_HEIGHT - 64 - 10);
		loadResources();
		return;
	}

	// プロローグ
	if (bStartPlaying == false)
	{
		float prologueAlpha = prologueTiming - prologueCount;
		if (prologueAlpha > 1)
		{
			prologueAlpha = 1;
		}

		// 画面全体を黒で描画
		Rect{ 0, 0, Scene::Width(), Scene::Height() }.draw(ColorF{ 0.0, prologueAlpha });

		// TODO 共通化する
		double lineSpacing = 96; // 行間（フォントサイズより少し大きめ）

		// 今、表示するための文字数
		int prologueIndex = prologueCount*5;

		// テキストのアルファ値
		float a = prologueCount * 5 - (float)prologueIndex;

		for (int i = 0; i < prologueText.size(); ++i)
		{
			if (prologueIndex <= 0)
			{
				break;
			}

			double x = center.x;
		//	double y = center.y + lineSpacing * (i - (int)prologueText.size() / 2) + 38;	// TODO 38は高さ調整のため
			double y = center.y + lineSpacing * (i - (int)prologueText.size() / 2);

			int num = 0;
			int sub = 0;

			if (prologueIndex >= prologueText[i].size())
			{
				// すべて表示
				num = prologueText[i].size();
				prologueIndex -= prologueText[i].size();
			}
			else
			{
				// 一部表示
				num = prologueIndex;
				sub = prologueText[i].size() - num;
				prologueIndex = 0;
			}

			String tmp = prologueText[i].substr(0, num);
			tmp.append(sub, U'　');	// 後ろをスペースで埋める

			boldFont(tmp).drawAt(
				28,
				{ x, y },
				ColorF{ 1, 1, 1, prologueAlpha }
			);

			// 半透明文字
			if (num+1 < prologueText[i].size())
			{
				String tmp2;
				tmp2.append(num, U'　');		// 後ろをスペースで埋める
				tmp2.append(1, prologueText[i][num]);
				tmp2.append(sub - 1, U'　');

				boldFont(tmp2).drawAt(
					28,
					{ x, y },
					ColorF{ 1, 1, 1, a }
				);
			}

		}
	}

	// セリフ表示
	if (0 <= message && message < Text.size() / MessagePatternMax && messagePattern % 2 == 0)
	{
		// 画面下を黒い半透明で描画
		Rect{ 0, Scene::Height()-40, Scene::Width(), Scene::Height() }.draw(ColorF{ 0.0, 0.5 });

		int m = message * MessagePatternMax + messagePattern / 2;

		// 今、表示するための文字数
		int messageIndex = messageCount * MessageSpeed;

		// テキストのアルファ値
		float a = messageCount * MessageSpeed - (float)messageIndex;

		// フェードアウト用のアルファ
		float fadeout = 1;
		
		if (bStartPlaying == false && message != 0)
		{
			fadeout = (float)Text[m].size() - messageCount * MessageSpeed;
		//	fadeout /= 5;	// ゆっくりにしたい
			fadeout /= 2.5;	// ゆっくりにしたい
			if (fadeout > 1)
			{
				fadeout = 1;
			}
		}

		// ここでのチェックは不要
		//if (Text[m].isEmpty())
		//{
		//	m = message * MessagePatternMax;
		//}

		//boldFont(Text[m]).drawAt(
		//	28,
		////	{ center.x, 700 },
		//	{ center.x, height - 20 },
		//	ColorF{ 1, 1, 1, a }
		//);

		double x = center.x;
		double y = height - 20;

		int num = 0;
		int sub = 0;

		if (messageIndex >= Text[m].size())
		{
			// すべて表示
			num = Text[m].size();
		}
		else
		{
			// 一部表示
			num = messageIndex;
			sub = Text[m].size() - num;
		}

		String tmp = Text[m].substr(0, num);
		tmp.append(sub, U'　');	// 後ろをスペースで埋める

		boldFont(tmp).drawAt(
			28,
			{ x, y },
			ColorF{ 1, 1, 1, fadeout }
		);

		// 半透明文字
		if (num + 1 < Text[m].size())
		{
			String tmp2;
			tmp2.append(num, U'　');		// 後ろをスペースで埋める
			tmp2.append(1, Text[m][num]);
			tmp2.append(sub - 1, U'　');

			boldFont(tmp2).drawAt(
				28,
				{ x, y },
				ColorF{ 1, 1, 1, a }
			);
		}
	}

	// インベントリのアイテム説明
	if (
		bInventory
		&& 0 <= itemMessage
		&& itemMessage < itemText.size()
	)
	{
		//boldFont(itemText[itemMessage]).drawAt(
		//	28,
		////	{ center.x, 546 },
		//	{ center.x, center.y + itemHeight },
		//	ColorF{ 1, 1, 1, 1 }
		//);

		// TODO 共通化する
		double lineSpacing = 40.0; // 行間（フォントサイズより少し大きめ）

		for (int i = 0; i < itemText[itemMessage].size(); ++i)
		{
			double x = center.x;
		//	double y = center.y + 345 + lineSpacing * (i - (int)itemText.size() / 2);
			double y = center.y + 145 + lineSpacing * i;

			boldFont(itemText[itemMessage][i]).drawAt(
				28,
				{ x, y },
				ColorF{ 1, 1, 1, 1 }
			);
		}
	}

	// アイテムの名前
	if (
		bInventory
		&& 0 <= itemMessage
		&& itemMessage < itemText.size()
		)
	{
		int fontSize = 28;

		// 長すぎるアイテム名だった場合はフォントサイズを小さくする
		if (itemNameText[itemMessage].size() > 5)
		{
			fontSize = 28 - (itemNameText[itemMessage].size() - 5) * 3;
		}

		boldFont(itemNameText[itemMessage]).drawAt(
			fontSize,
		//	{ 805, 236 },
			{ center.x + 165, center.y -128 },
			ColorF{ 1, 1, 1, 1 }
		);
	}

	// 手記
	if (bInventory && bMemo)
	{
		// 半透明の黒い画像
		Rect{ 0, 0, Scene::Width(), Scene::Height() }.draw(ColorF{ 0.0, 0.5 });

		// TODO 共通化する
		double lineSpacing = 40.0; // 行間（フォントサイズより少し大きめ）

		for (int i = 0; i < memoText.size(); ++i)
		{
			double x = center.x;
			double y = center.y + lineSpacing * (i - (int)memoText.size()/2);

			boldFont(memoText[i]).drawAt(
				28,
				{ x, y },
				ColorF{ 1, 1, 1, 1 }
			);
		}

	}

	// 羊皮紙
	if (bInventory && bToastedParchmentRead)
	{
		// 半透明の黒い画像
		Rect{ 0, 0, Scene::Width(), Scene::Height() }.draw(ColorF{ 0.0, 0.5 });

		// TODO 共通化する
		double lineSpacing = 40.0; // 行間（フォントサイズより少し大きめ）

		for (int i = 0; i < toastedParchmentText.size(); ++i)
		{
			double x = center.x;
			double y = center.y + lineSpacing * (i - (int)toastedParchmentText.size() / 2);

			boldFont(toastedParchmentText[i]).drawAt(
				28,
				{ x, y },
				ColorF{ 1, 1, 1, 1 }
			);
		}
	}

	// 布
	if (bInventory && bClothRead)
	{
		// 半透明の黒い画像
		Rect{ 0, 0, Scene::Width(), Scene::Height() }.draw(ColorF{ 0.0, 0.5 });

		// TODO 共通化する
		double lineSpacing = 40.0; // 行間（フォントサイズより少し大きめ）

		for (int i = 0; i < clothText.size(); ++i)
		{
			double x = center.x;
			double y = center.y + lineSpacing * (i - (int)clothText.size() / 2);

			boldFont(clothText[i]).drawAt(
				28,
				{ x, y },
				ColorF{ 1, 1, 1, 1 }
			);
		}
	}

}

// SEを鳴らす＆BGMをストップ
void CameraTest::playSEandBGMStop(String SE)
{
	AudioAsset(U"BGM").stop();
	AudioAsset(U"bonfire").stop();
	if (AudioAsset(SE).isPlaying())
	{
		AudioAsset(SE).stop();
	}
	AudioAsset(SE).setVolume(1.0);
	AudioAsset(SE).play();
	bgmStopCount = 4.00;
}

// SEを鳴らす（BGMは鳴らしたまま）
void CameraTest::playSE(String SE)
{
	if (AudioAsset(SE).isPlaying())
	{
		AudioAsset(SE).stop();
	}
	AudioAsset(SE).setVolume(1.0);
	AudioAsset(SE).play();
}

// コリジョンのデバッグ表示
void CameraTest::debugViewCollision()
{
	if (bDebugViewCollision == false)
	{
		return;
	}
	// モデルのワイヤーフレーム表示
	{
		Transformer3D t{ Mat4x4::RotateY(0_deg).scaled(roomScale).translated(roomPos) };
		const auto& objectTmp = model->objects();
		for (const auto& object : objectTmp)
		{
			if (bDebugViewFrame)
			{
				// ワイヤーフレーム
				//object.boundingBox.drawFrame(ColorF{ 1, 1, 1, 1 });

				// 三角形
				const std::array<Vec3, 8> cube = object.boundingBox.getCorners();

				ColorF color{ 1, 1, 1, 1 };

				for (int i = 0; i < 12; i++)
				{
					Line3D{ cube[collisionTriangle[i][0]], cube[collisionTriangle[i][1]] }.draw(color);
					Line3D{ cube[collisionTriangle[i][1]], cube[collisionTriangle[i][2]] }.draw(color);
					Line3D{ cube[collisionTriangle[i][2]], cube[collisionTriangle[i][0]] }.draw(color);
				}
			}
			else
			{
				// 塗りつぶし
				object.boundingBox.draw(ColorF{ 1, 1, 1, 1 });
			}
		}
	}

	// コリジョンなし
	for (int i = 0; i < DoorNum; i++)
	{
		if (bDoorOpen[i] == false)
		{
			continue;
		}

		ColorF color{ 0.0, 0.0, 1, 1 };

		// 縦
		Line3D{ Vec3{collisionNone[i][0], 0, collisionNone[i][2]}, Vec3{collisionNone[i][0], 3, collisionNone[i][2]} }.draw(color);
		Line3D{ Vec3{collisionNone[i][1], 0, collisionNone[i][2]}, Vec3{collisionNone[i][1], 3, collisionNone[i][2]} }.draw(color);
		Line3D{ Vec3{collisionNone[i][0], 0, collisionNone[i][3]}, Vec3{collisionNone[i][0], 3, collisionNone[i][3]} }.draw(color);
		Line3D{ Vec3{collisionNone[i][1], 0, collisionNone[i][3]}, Vec3{collisionNone[i][1], 3, collisionNone[i][3]} }.draw(color);

		// 下
		Line3D{ Vec3{collisionNone[i][0], 0, collisionNone[i][2]}, Vec3{collisionNone[i][1], 0, collisionNone[i][2]} }.draw(color);
		Line3D{ Vec3{collisionNone[i][0], 0, collisionNone[i][3]}, Vec3{collisionNone[i][1], 0, collisionNone[i][3]} }.draw(color);
		Line3D{ Vec3{collisionNone[i][0], 0, collisionNone[i][2]}, Vec3{collisionNone[i][0], 0, collisionNone[i][3]} }.draw(color);
		Line3D{ Vec3{collisionNone[i][1], 0, collisionNone[i][2]}, Vec3{collisionNone[i][1], 0, collisionNone[i][3]} }.draw(color);

		// 上
		Line3D{ Vec3{collisionNone[i][0], 3, collisionNone[i][2]}, Vec3{collisionNone[i][1], 3, collisionNone[i][2]} }.draw(color);
		Line3D{ Vec3{collisionNone[i][0], 3, collisionNone[i][3]}, Vec3{collisionNone[i][1], 3, collisionNone[i][3]} }.draw(color);
		Line3D{ Vec3{collisionNone[i][0], 3, collisionNone[i][2]}, Vec3{collisionNone[i][0], 3, collisionNone[i][3]} }.draw(color);
		Line3D{ Vec3{collisionNone[i][1], 3, collisionNone[i][2]}, Vec3{collisionNone[i][1], 3, collisionNone[i][3]} }.draw(color);
	}

	// ライトの判定
	//for (auto& row : collisionLight)
	//{
	//	ColorF color{ 1.0, 1.0, 0.0, 1 };

	//	// 縦
	//	Line3D{ Vec3{row[0], 0, row[2]}, Vec3{row[0], 3, row[2]} }.draw(color);
	//	Line3D{ Vec3{row[1], 0, row[2]}, Vec3{row[1], 3, row[2]} }.draw(color);
	//	Line3D{ Vec3{row[0], 0, row[3]}, Vec3{row[0], 3, row[3]} }.draw(color);
	//	Line3D{ Vec3{row[1], 0, row[3]}, Vec3{row[1], 3, row[3]} }.draw(color);

	//	// 下
	//	Line3D{ Vec3{row[0], 0, row[2]}, Vec3{row[1], 0, row[2]} }.draw(color);
	//	Line3D{ Vec3{row[0], 0, row[3]}, Vec3{row[1], 0, row[3]} }.draw(color);
	//	Line3D{ Vec3{row[0], 0, row[2]}, Vec3{row[0], 0, row[3]} }.draw(color);
	//	Line3D{ Vec3{row[1], 0, row[2]}, Vec3{row[1], 0, row[3]} }.draw(color);

	//	// 上
	//	Line3D{ Vec3{row[0], 3, row[2]}, Vec3{row[1], 3, row[2]} }.draw(color);
	//	Line3D{ Vec3{row[0], 3, row[3]}, Vec3{row[1], 3, row[3]} }.draw(color);
	//	Line3D{ Vec3{row[0], 3, row[2]}, Vec3{row[0], 3, row[3]} }.draw(color);
	//	Line3D{ Vec3{row[1], 3, row[2]}, Vec3{row[1], 3, row[3]} }.draw(color);
	//}

	Print << U"m_focusPosition" << camera.getFocusPosition();
	modelExclamationMark->draw(camera.getFocusPosition());

}

// インベントリ表示
void CameraTest::viewInventory()
{
	int itemX = center.x - inventoryWidth / 2;
	int itemY = center.y - inventoryHeight / 2;
	inventorySprite.scaled(0.5).draw(itemX, itemY);

#ifdef _DEBUG
	Print << U"selectItem=" << selectItem;
	Print << U"KeyMode=" << KeyMode;
	Print << U"items.size()=" << items.size();
#endif

	// 初期化
	itemMessage = -1;

	auto xboxController = XInput(playerIndex);
	xboxController.setLeftTriggerDeadZone();
	xboxController.setRightTriggerDeadZone();
	xboxController.setLeftThumbDeadZone();
	xboxController.setRightThumbDeadZone();

	// インベントリ全体
	{
		Rect rect(itemX, itemY, inventoryWidth, inventoryHeight);
		if (rect.mouseOver())
		{
			//Print << U"インベントリ内";
		}
		else
		{
			//Print << U"インベントリ外";
			if (MouseL.down())
			{
				// インベントリを非表示にする
				inventoryOnOff();
			}
		}

		if (xboxController.buttonB.down())
		{ 
			// インベントリを非表示にする
			inventoryOnOff();
		}
	}

	if (-0.1 < xboxController.leftThumbX && xboxController.leftThumbX < 0.1)
	{
		xboxControllerDownX = false;
	}
	if (-0.1 < xboxController.leftThumbY && xboxController.leftThumbY < 0.1)
	{
		xboxControllerDownY = false;
	}

	if (KeyA.down() || xboxController.buttonLeft.down())
	{
		KeyMode = true;

		selectItem--;
		if (selectItem < 0)
		{
			selectItem = 0;
		}
	}
	if (xboxController.leftThumbX < -0.1 && xboxControllerDownX == false)
	{
		xboxControllerDownX = true;

		KeyMode = true;

		selectItem--;
		if (selectItem < 0)
		{
			selectItem = 0;
		}
	}

	if (KeyD.down() || xboxController.buttonRight.down())
	{
		KeyMode = true;

		selectItem++;
		if (selectItem > items.size() - 1)
		{
			selectItem = items.size() - 1;
		}
	}
	if (xboxController.leftThumbX > 0.1 && xboxControllerDownX == false)
	{
		xboxControllerDownX = true;

		KeyMode = true;

		selectItem++;
		if (selectItem > items.size() - 1)
		{
			selectItem = items.size() - 1;
		}
	}

	if (KeyS.down() || xboxController.buttonDown.down())
	{
		KeyMode = true;

		selectItem += 4;
		if (selectItem > items.size() - 1)
		{
			selectItem = items.size() - 1;
		}
	}
	if (xboxController.leftThumbY < -0.1 && xboxControllerDownY == false)
	{
		xboxControllerDownY = true;

		KeyMode = true;

		selectItem += 4;
		if (selectItem > items.size() - 1)
		{
			selectItem = items.size() - 1;
		}
	}

	if (KeyW.down() || xboxController.buttonUp.down())
	{
		KeyMode = true;

		selectItem -= 4;
		if (selectItem < 0)
		{
			selectItem = 0;
		}
	}
	if (xboxController.leftThumbY > 0.1 && xboxControllerDownY == false)
	{
		xboxControllerDownY = true;

		KeyMode = true;

		selectItem -= 4;
		if (selectItem < 0)
		{
			selectItem = 0;
		}
	}

	if (items.size() == 0)
	{
		selectItem = -1;
	}

	// マウスを動かしたかどうか
	Vec2 currentCursorPos = Cursor::PosF();
	if (currentCursorPos != lastCursorPos)
	{
		KeyMode = false;
	}
	lastCursorPos = currentCursorPos;


	// アイテム
	for (int i = 0; i < items.size(); i++)
	{
		int itemMiniX = center.x - itemMiniWidth / 2 + (i % 4 * 82) - 194;
		int itemMiniY = center.y - itemMiniHeight / 2 + (i / 4 * 82) - 104;

		drawMiniItem(items[i], itemMiniX, itemMiniY);

		if (bMemo || bToastedParchmentRead || bClothRead)	// TODO 増えたら困る
		{
			// 選択できないようにする
			continue;
		}

		// 現在選択中のアイテムに枠を表示
		Rect rect(itemMiniX, itemMiniY, itemMiniWidth, itemMiniHeight);

		if (KeyMode == false)
		{
			if (rect.mouseOver())
			{
				selectItem = i;

				Color rectColor = Palette::Skyblue;

				// 合成中なら色を変える
				if (synthesisIndex >= 0)
				{
					if (synthesisIndex == i)
					{
						// 合成元
						rectColor = Palette::Greenyellow;
					}
					else
					{
						// 合成先
						rectColor = Palette::Yellow;
					}
				}

				// 枠線
				rect.drawFrame(2, rectColor);

				// コメント
				itemMessage = items[selectItem];
			}
			else
			{
				Color rectColor = Palette::Black;

				// 合成中なら色を変える
				if (synthesisIndex >= 0)
				{
					if (synthesisIndex == i)
					{
						// 合成元
						rectColor = Palette::Greenyellow;
					}
				}

				// 枠線
				rect.drawFrame(1, rectColor);
			}
		}
		else
		{
			// キーボード対応
			if (selectItem == i)
			{
				Color rectColor = Palette::Skyblue;

				// 合成中なら色を変える
				if (synthesisIndex >= 0)
				{
					if (synthesisIndex == i)
					{
						// 合成元
						rectColor = Palette::Greenyellow;
					}
					else
					{
						// 合成先
						rectColor = Palette::Yellow;
					}
				}

				// 枠線
				rect.drawFrame(2, rectColor);

				// コメント
				itemMessage = items[selectItem];
			}
			else
			{
				Color rectColor = Palette::Black;

				// 合成中なら色を変える
				if (synthesisIndex >= 0)
				{
					if (synthesisIndex == i)
					{
						// 合成元
						rectColor = Palette::Greenyellow;
					}
				}

				// 枠線
				rect.drawFrame(1, rectColor);
			}
		}

	}

	//現在選択中のアイテム
	if (selectItem >= 0)
	{
		int itemBigX = center.x - itemBigWidth / 2 + 166;
		int itemBigY = center.y - itemBigHeight / 2 - 35;

		drawBigItem(items[selectItem], itemBigX, itemBigY);

		if (items[selectItem] != RustedKey)
		{
			// 錆びた鍵を使ったフラグをリセット
			bRustedKeyUse = false;
		}
	}
	else
	{
		// 錆びた鍵を使ったフラグをリセット
		bRustedKeyUse = false;
	}

	if (MouseL.down() || xboxController.buttonA.down())
	{
		// アイテムを使う
		if (bMemo || bToastedParchmentRead || bClothRead)	// TODO 増えたら困る
		{
			// メッセージが表示されていたら閉じる系
			bMemo = false;
			bToastedParchmentRead = false;
			bClothRead = false;
		}
		else if (items.size() <= selectItem)
		{
			// アイテムのない場所は処理しない
		}
		else if (items[selectItem] == Bread)
		{
			// パンを食べる
			items.remove_at(selectItem);

			// シナリオを進める
			scenario = 2;	// パンを食べた後

			// SEを鳴らす
		//	playSE(U"GET");
			playSEandBGMStop(U"Item");

			// チュートリアル終了
			bTutorial = false;
		}
		else if (items[selectItem] == Memo)
		{
			// 手記を使った
			if (synthesisIndex == -1)	// 合成アイテムを選択していない状態
			{
				bMemo = true;
				playSEandBGMStop(U"Paper");
			}
		}
		else if (items[selectItem] == RustedKey)
		{
			// 錆びた鍵

			if (synthesisIndex == -1)
			{
				if (bDoorLockon)
				{
					// 扉の前で使う
					message = 69;
					bRustedKeyUse = true;
				}
			}
			else if (items[synthesisIndex] == Hanger)
			{
				// ハンガーを針金製の鍵にする
				items[synthesisIndex] = WireKey;

				bWireKey = true;

				synthesisIndex = -1;	// 選択を解除

				// SEを鳴らす
			//	playSE(U"GET");
				playSEandBGMStop(U"Item");

				//if (scenario == 1)	// パンを食べた後
				//{
				//	scenario = 2;	// なし
				//}

				priorityMessage = 86;
				priorityMessageCount = priorityMessageCountMax;
			}
		}
		else if (items[selectItem] == Hanger)
		{
			// ハンガー
			if (synthesisIndex == -1)
			{
				// カーソルを緑にする
				synthesisIndex = selectItem;
			}
			else if (items[synthesisIndex] == Hanger)
			{
				// キャンセル
				synthesisIndex = -1;
			}

			//else if (items[synthesisIndex] == RustedKey)
			//{
			//	// ハンガーを針金製の鍵にする
			//	items[selectItem] = WireKey;

			//	bWireKey = true;

			//	synthesisIndex = -1;	// 選択を解除

			//	// SEを鳴らす
			//	playSE(U"GET");

			//	if (scenario == 1)	// パンを食べた後
			//	{
			//		scenario = 2;	// なし
			//	}
			//}

			// 錆びた鍵を持っているかどうか
			//if (bRustedKeyHave)
			//{
			//	// 錆びた鍵を持っている状態で針金を使う
			//	items[selectItem] = WireKey;
			//	bWireKey = true;

			//	// SEを鳴らす
			//	playSE(U"GET");

			//	if (scenario == 1)	// パンを食べた後
			//	{
			//		scenario = 2;	// なし
			//	}
			//}
		}
		else if (items[selectItem] == WireKey)
		{
			if (bDoorLockon)
			{
				// 扉の前で使う

				// 扉を開いた
				bDoorOpen[0] = true;
				toDoorPosX = doorPos.x + 1.49;	// 移動で開ける
				
				//if (scenario == 1)	// パンを食べた後
				//{
				//	scenario = 2;	// なし
				//}

				// SEを鳴らす
			//	playSE(U"牢屋の扉を開ける");
			//	playSEandBGMStop(U"Cancellation");
				playSEandBGMStop(U"IronDoor_Close");

				inventoryOnOff();
			}
		}
		else if (items[selectItem] == IronKey)
		{
			if (bDoor2Lockon)
			{
				// 扉２の前で使う

				// 扉２を開いた
				bDoorOpen[1] = true;
				toDoor2PosX = door2Pos.x + 1.49;	// 移動で開ける

				// SEを鳴らす
			//	playSE(U"牢屋の扉を開ける");
			//	playSEandBGMStop(U"Cancellation");
				playSEandBGMStop(U"IronDoor_Close");

				inventoryOnOff();
			}
		}
		//else if (items[selectItem] == GoldKey)
		//{
		//	if (bDoor3Lockon)
		//	{
		//		// 扉３の前で使う

		//		// SEを鳴らす
		//		playSEandBGMStop(U"WoodDoor_Close");

		//		inventoryOnOff();

		//		// ゲームクリア（仮）
		//		changeScene(State::ToBeContinued);
		//	}
		//}
		else if (items[selectItem] == Poker)
		{
			// 鉄製の鍵のところで使う
			if (bIronKeyLockon)
			{
				// 鉄製の鍵を入手
				items << IronKey;
				bIronKeyHave = true;

				inventoryOnOff();

				// SEを鳴らす
			//	playSE(U"GET");
			//	playSEandBGMStop(U"Item");
				playSEandBGMStop(U"KeyGimmick01");

				// 火かき棒で鍵をとった時のメッセージを表示する時間
				priorityMessage = 71;
				priorityMessageCount = priorityMessageCountMax;
			}
		}
		else if (items[selectItem] == Parchment)
		{
			// 火が強くなった暖炉の近くで使う（オミット）
			// 暖炉で使う
			if (bFireplaceLockon)
			{
				// 暖炉で羊皮紙を使う 
				items[selectItem] = ToastedParchment;

				// SEを鳴らす
			//	playSE(U"GET");
				playSEandBGMStop(U"Item");

				priorityMessage = 85;
				priorityMessageCount = priorityMessageCountMax;
			}
		}
		else if (items[selectItem] == ToastedParchment)
		{
			// 炙った羊皮紙を使った
			if (synthesisIndex == -1)	// 合成アイテムを選択していない状態
			{
				bToastedParchmentRead = true;
				playSEandBGMStop(U"Paper");
			}

		}
		else if (items[selectItem] == DirtyCloth)
		{
			// 汚れた布

			// トイレの近くで使う
			if (bToilet2Lockon)
			{
				items[selectItem] = Cloth;

				// SEを鳴らす
			//	playSE(U"GET");
				playSEandBGMStop(U"Water");

				priorityMessage = 84;
				priorityMessageCount = priorityMessageCountMax;
			}
		}
		else if (items[selectItem] == Cloth)
		{
			// 布を使った
			if (synthesisIndex == -1)	// 合成アイテムを選択していない状態
			{
				bClothRead = true;
			//	playSE(U"Item");
			}
		}

		// ニュートラルの位置に戻す
	//	selectItem = -1;
	}
}

void CameraTest::viewModel()
{
	const ScopedRenderTarget3D target{ renderTexture.clear(backgroundColor) };

	// ライトの位置（ポイントライトVer.1）
	//ConstantBuffer<Light> cb;
	//cb->g_pointLightPos = lightPos;
	//cb->g_pointLightStrong = lightStrong;
	//Graphics3D::SetConstantBuffer(ShaderStage::Pixel, 2, cb);

	// 点光源を設定する
	constantBuffer->setPointLight(0, lightPos, ColorF{ 1.0, 1.0, 1.0 }, lightStrong);
	constantBuffer->setPointLight(1, fireplaceLightPos, ColorF{ fireplaceR, fireplaceG, fireplaceB }, Periodic::Sine0_1(2s) * fireplaceSin + fireplaceStrong);

	const ScopedCustomShader3D shader(vs3D, ps3D);

	// ピクセルシェーダに定数バッファを渡す
	Graphics3D::SetPSConstantBuffer(4, constantBuffer);

	constantBuffer->drawPointLightAsEmissiveSphere(0, 0);
	constantBuffer->drawPointLightAsEmissiveSphere(1, 0.1);

	// モデルを描画
	if (bDebugviewModel)
	{
		Transformer3D t{ Mat4x4::RotateY(0_deg).scaled(roomScale).translated(roomPos) };
		model->draw();
	}

	// 扉
	{
		Transformer3D t{
			Mat4x4::RotateY(doorRot.y).scaled(roomScale).translated(doorPos)
		};
		modelDoor->draw();
	}

	// 扉２
	{
		Transformer3D t{
			Mat4x4::RotateY(door2Rot.y).scaled(roomScale).translated(door2Pos)
		};
		modelDoor->draw();
	}

	// パンの描画
	if (bBreadHave == false)
	{
		Transformer3D t{
			Mat4x4::RotateY(0_deg).scaled(0.01).translated(breadPos)
		};

		modelBread->draw();
	}

	// 錆びた鍵の描画
	if (bRustedKeyHave == false)
	{
		Transformer3D t{
		//	Mat4x4::RotateZ(90_deg).scaled(0.015).translated(rustedKeyPos)
			Mat4x4::RotateZ(180_deg).scaled(0.015).translated(rustedKeyPos)
		};
		modelRustedKey->draw();
	}

	// 鉄製の鍵の描画
	if (bIronKeyHave == false)
	{
		Transformer3D t{
			Mat4x4::RotateZ(0_deg).scaled(0.015).translated(IronkeyPos)
		};
		modelIronKey->draw();
	}

	// 火かき棒の描画
	if (bPokerHave == false)
	{
		Transformer3D t{
			Mat4x4::RotateZ(75_deg).scaled(0.01).translated(pokerPos)
		};

		modelPoker->draw();
	}

	// 引き出し
	for (int i = 0; i < 7; i++) 
	{
		Transformer3D t{
		//	Mat4x4::RotateY(0_deg).scaled(0.01).translated(Vec3{drawerPos[i].x, drawerPos[i].y, drawerPos[i].z})
			Mat4x4::RotateY(0_deg).scaled(0.01).translated(drawerPos[i])
		};

		switch (i)
		{
		case 0:
			modelShelf->draw();
			break;
		//case 1:
		//	modelDrawerNon->draw();
		//	break;
		case 1:
			modelDrawerFlower->draw();
			break;
		case 2:
			modelDrawerChain->draw();
			break;
		case 3:
			modelDrawerFeather->draw();
			break;
		case 4:
			modelDrawerSnake->draw();
			break;
		case 5:
			modelDrawerEye->draw();
			break;
		}
	}

	// 引き出しテスト
	//{
	//	Transformer3D t{
	//		Mat4x4::RotateY(0_deg).scaled(0.01).translated(drawerTestPos)
	//	};
	//	modelDrawerTest->draw();
	//}

	// 羊皮紙の描画
	if (bParchmentHave == false)
	{
		Transformer3D t{
			Mat4x4::RotateY(parchmentRot.y).scaled(0.01).translated(parchmentPos)
		};

		modelParchment->draw();
	}

	// ハンガーの描画
	if (bHangerHave == false)
	{
		Transformer3D t{
			Mat4x4::RotateY(hangerRot.y).scaled(0.01).translated(hangerPos)
		};

		modelHanger->draw();
	}

	// 汚れた布の描画
	if (bDirtyClothHave == false)
	{
		Transformer3D t{
			Mat4x4::RotateY(0_deg).scaled(0.01).translated(dirtyClothPos)
		};

		modelDirtyCloth->draw();
	}

	// 手記の描画
	if (bMemoHave == false)
	{
		Transformer3D t{
			Mat4x4::RotateY(memoRot.y).scaled(0.03).translated(memoPos)
		};

		modelMemo->draw();
	}

	// 石板の描画
	{
		for (int i = 0; i < 5; i++)
		{
			Transformer3D t{
				Mat4x4::RotateY(stoneRot.y).scaled(0.01).translated(stonePos[i])
			};

			switch (i)
			{
			case 0:
				modelStoneBase->draw();
				break;
			case 1:
				modelStoneBlue->draw();
				break;
			case 2:
				modelStoneGreen->draw();
				break;
			case 3:
				modelStoneRed->draw();
				break;
			case 4:
				modelStoneYellow->draw();
				break;
			}
		}
	}

	// ビックリマークのビルボード
	if (bLockon)
	{
		// 少し上にする
	//	markPosition.y += markHigh;	// 近づきすぎると表示されなくなるのでやめる

		// 3Dのビックリマーク
		//Transformer3D t{
		//	Mat4x4::RotateY(0_deg).scaled(1).translated(MarkPosition)
		//};
		//modelExclamationMark.draw();

		// ビックリマークとカメラの距離
	//	Vec3 focus = camera.getFocusPosition();
		double distance = curCameraPosition.distanceFrom(markPosition);

		float tempSize = distance * markSize;
#ifdef _DEBUG
		//Print << U"distance" << distance;
		//Print << U"tempSize" << tempSize;
#endif
		// ビルボードの表示
		const Mat4x4 billboardMat = camera.getInvView();
		billboard.draw(camera.billboard(markPosition, tempSize), uvChecker);
	}

	// 暖炉の炎
	{
		const ScopedRenderStates3D blend{ BlendState::Additive };
		fireBillboard.draw(camera, fireBillboardPos, fireBillboardScale, fireBillboardColor);
	}

	// デバッグ表示
#if _DEBUG
	debugViewCollision();
#endif
}

void CameraTest::lockon()
{
	// アイテムのロックオンフラグのリセット
	bLockon = false;

	// パン
	if (!bLockon)
	{
		Vec3 temp = breadPos;
		temp.y += markHigh;

		auto [a, b, c, d] = breadController.update(
			temp,
			camera,
			curCameraPosition, 
		//	ray,
			markPosition,
			0,
			true
		);
		if (a == true && bBreadHave == false)
		{
			// アイテムを取った
			items << Bread;
			bBreadHave = a;
			bgmStopCount = c;

			scenario = 1;	// パンを取った後

			priorityMessage = 77;
			priorityMessageCount = priorityMessageCountMax;
		}
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 50;
		}
		if (d)
		{
			message = 50;	// パンをとってしまうので、使われない
			priorityMessageCount = 0;
		}
	}

	// 錆びた鍵
	if (!bLockon && bRustedKeyHave == false && bTutorial == false)
	{
		// 座標の調整
		Vec3 temp = rustedKeyPos;
	//	temp.x -= 0.12;
		temp.x -= 0.22;
		temp.y += 0.2+ markHigh;
		temp.z += 0.0;

		auto [a, b, c, d] = rustedKeyController.update(
			temp,
			camera,
			curCameraPosition,
		//	ray, 
			markPosition,
			0, 
			true
		);
		if (a == true)
		{
			// アイテムを取った
			items << RustedKey;
			bRustedKeyHave = true;
			bgmStopCount = c;
			scenario = 3;	// なし

			priorityMessage = 79;
			priorityMessageCount = priorityMessageCountMax;
		}
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 52;
		}
		if (d)
		{
			message = 52;	// とってしまうので、使われない
			priorityMessageCount = 0;
		}
	}

	// 手記
	if (!bLockon && bMemoHave == false && bTutorial == false)
	{
		Vec3 temp = memoPos;
		temp.y += markHigh;

		auto [a, b, c, d] = memoController.update(
			temp,
			camera,
			curCameraPosition,
			//	ray, 
			markPosition,
			2,
			true
		);
		if (a == true)
		{
			// 手記を取得
			bMemoHave = true;
			items << Memo;
			bgmStopCount = c;

			scenario = 3;	// なし

			priorityMessage = 78;
			priorityMessageCount = priorityMessageCountMax;
		}
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 51;
		}
		if (d)
		{
			message = 51;	// とってしまうので、使われない
			priorityMessageCount = 0;
		}
	}

	// ハンガー
	if (!bLockon && bHangerHave == false && bTutorial == false)
	{
		Vec3 temp = hangerPos;
		temp.y += markHigh;
		temp.z -= 0.05;

		auto [a, b, c, d] = hangerController.update(
			temp,
			camera,
			curCameraPosition,
			//	ray,
			markPosition,
			0,
			true
		);
		if (a == true)
		{
			// ハンガーを取得
			bHangerHave = true;
			items << Hanger;
			bgmStopCount = c;

			scenario = 3;	// なし

			priorityMessage = 82;
			priorityMessageCount = priorityMessageCountMax;
		}
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 22;
		}
		if (d)
		{
			message = 22;	// とってしまうので、使われない
			priorityMessageCount = 0;
		}
	}

	// 鉄製の鍵
	if (!bLockon && bIronKeyHave == false)
	{
		Vec3 temp = IronkeyPos;
		temp.y += markHigh;

		auto [a, b, c, d] = ironkeyController.update(
			temp,
			camera,
			curCameraPosition,
		//	ray,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 31;
			bIronKeyLockon = true;
		}
		else
		{
			bIronKeyLockon = false;
		}
		if (d)
		{
			message = 31;
			priorityMessageCount = 0;
		}
	}
	else
	{
		bIronKeyLockon = false;
	}

	// 火かき棒
	if (!bLockon)
	{
		Vec3 temp = pokerPos;
		temp.y += markHigh;

		auto [a, b, c, d] = pokerController.update(
			temp,
			camera,
			curCameraPosition,
		//	ray, 
			markPosition,
			0,
			true
		);
		if (a == true && bPokerHave == false)
		{
			// アイテムを取った
			items << Poker;
			bPokerHave = a;
			bgmStopCount = c;

			priorityMessage = 80;
			priorityMessageCount = priorityMessageCountMax;
		}
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 53;
		}
		if (d)
		{
			message = 53;
			priorityMessageCount = 0;
		}
	}

	// 扉
	if (!bLockon && bDoorOpen[0] == false)
	{
		// 座標の調整
		Vec3 temp = doorPos;
		//temp.x += 0.70;	// 原点が端っこの時
		temp.x += 0.0;
		temp.y += 1.3+ markHigh;
		temp.z += 0.2;

		//Print << U"扉";

		Vec3 screenPos = camera.worldToScreenPoint(temp);
		Vec3 focus = camera.getFocusPosition();
		double distance = focus.distanceFrom(temp);

		//Print << U"screenPos=" << screenPos;
		//Print << U"focus=" << focus;
		//Print << U"distance=" << distance;

		//Print << U"(WINDOW_WIDTH / 2 - focusWidth)=" << (WINDOW_WIDTH / 2 - focusWidth);
		//Print << U"(WINDOW_WIDTH / 2 + focusWidth)=" << (WINDOW_WIDTH / 2 + focusWidth);
		//Print << U"(WINDOW_HEIGHT / 2 - focusHeight)=" << (WINDOW_HEIGHT / 2 - focusHeight);
		//Print << U"(WINDOW_HEIGHT / 2 + focusHeight)=" << (WINDOW_HEIGHT / 2 + focusHeight);

		auto [a, b, c, d] = doorController.update(
			temp,
			camera,
			curCameraPosition,
		//	ray,
			markPosition,
			-1,
			false
		);

		//Print << U"扉END";

		// TODO インベントリから開ける
		//if (a == true && bDoorOpen[0] == false && bWireKey)
		//{
		//	// 扉を開いた
		//	bDoorOpen[0] = true;
		////	toDoorRotY = 270_deg;	// 回転で開ける
		////	toDoorPosX = -0.11;	// 移動で開ける
		//	toDoorPosX = doorPos.x + 1.49;	// 移動で開ける
		//	bgmStopCount = c;

		//	scenario = 6;
		//}
		if (b)
		{
			// 見ている
			bLockon = b;
			//	if (bWireKey)
			//	{
			//		message = 20;
			//	}
			//	else
			//	{
			bookingMessage = 10;
			//	}
			bDoorLockon = true;
		}
		else
		{
			bDoorLockon = false;
		}
		if (d)
		{
			message = 10;
			priorityMessageCount = 0;
		}
	}
	else
	{
		bDoorLockon = false;
	}

	// 扉２
	if (!bLockon && bDoorOpen[1] == false)
	{
		// 座標の調整
		Vec3 temp = door2Pos;
		temp.x += 0.0;
		temp.y += 1.3 + markHigh;
		temp.z -= 0.2;

		auto [a, b, c, d] = door2Controller.update(
			temp,
			camera,
			curCameraPosition,
		//	ray,
			markPosition,
			-1,
			false
		);
		// TODO インベントリから開ける
		//if (a == true && bDoorOpen[1] == false)
		//{
		//	// 扉を開いた
		//	bDoorOpen[1] = true;
		//	toDoor2PosX = door2Pos.x + 1.49;	// 移動で開ける
		//	bgmStopCount = c;
		//}
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 33;
			bDoor2Lockon = true;
		}
		else
		{
			bDoor2Lockon = false;
		}
		if (d)
		{
			message = 33;
			priorityMessageCount = 0;
		}
	}
	else
	{
		bDoor2Lockon = false;
	}

	// 扉３（木の扉）
	if (!bLockon)
	{
		Vec3 temp = door3Pos;
		temp.y += markHigh;

		auto [a, b, c, d] = door3Controller.update(
			temp,
			camera,
			curCameraPosition,
		//	ray,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			if (bStoneclear && bDrawerClear == false)
			{
				// 石板だけクリア
				bookingMessage = 75;
			}
			else if (bStoneclear == false && bDrawerClear)
			{
				// 引き出しだけクリア
				bookingMessage = 76;
			}
			else
			{
				// 何もクリアしていない
				bookingMessage = 30;
			}
			bDoor3Lockon = true;
		}
		else
		{
			bDoor3Lockon = false;
		}
		if (d)
		{
			if (bStoneclear && bDrawerClear)
			{
				// メッセージを消す
				message = 3;

			//	playSEandBGMStop(U"WoodDoor_Close");
				playSEandBGMStop(U"WoodDoor_open01");
				
			//	inventoryOnOff();

				// ゲームクリア（仮）
				changeScene(State::ToBeContinued);
			}
			else if (bStoneclear && bDrawerClear == false)
			{
				// 石板だけクリア
				message = 75;
				if (messagePattern % 2 == 1 || messagePattern == -1)
				{
					playSE(U"WoodDoor_Close");
				}
			}
			else if (bStoneclear == false && bDrawerClear)
			{
				// 引き出しだけクリア
				message = 76;
				if (messagePattern % 2 == 1 || messagePattern == -1)
				{
					playSE(U"WoodDoor_Close");
				}
			}
			else
			{
				// 何もクリアしていない
				message = 30;
				if (messagePattern % 2 == 1 || messagePattern == -1)
				{
					playSE(U"WoodDoor_Close");
				}
			}
			priorityMessageCount = 0;
		}
	}
	else
	{
		bDoor3Lockon = false;
	}

	// 扉４（右上の部屋）
	if (!bLockon)
	{
		Vec3 temp = door4Pos;
		temp.y += markHigh;
		temp.z += 0.05;

		auto [a, b, c, d] = door2Controller.update(
			temp,
			camera,
			curCameraPosition,
			//	ray,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 33;
		}
		if (d)
		{
			message = 33;
			priorityMessageCount = 0;
		}
	}

	// 羊皮紙
	if (!bLockon)
	{
		Vec3 temp = parchmentPos;
		temp.y += markHigh;

		auto [a, b, c, d] = parchmentController.update(
			temp,
			camera,
			curCameraPosition, 
		//	ray,
			markPosition,
			2,
			true
		);
		if (a == true && bParchmentHave == false)
		{
			// アイテムを取った
			items << Parchment;
			bParchmentHave = a;
			bgmStopCount = c;

			priorityMessage = 81;
			priorityMessageCount = priorityMessageCountMax;
		}
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 54;
		}
		if (d)
		{
			message = 54;
			priorityMessageCount = 0;
		}
	}

	// トイレ２（左上の部屋）
	if (!bLockon)
	{
		Vec3 temp = toilet2Pos;
		temp.y += markHigh;

		auto [a, b, c, d] = toiletController.update(
			temp,
			camera,
			curCameraPosition,
			//	ray,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 32;
			bToilet2Lockon = true;
		}
		else
		{
			bToilet2Lockon = false;
		}
		if (d)
		{
			message = 32;
			priorityMessageCount = 0;
		}
	}
	else
	{
		bToilet2Lockon = false;
	}

	// 暖炉（火が弱い）
	if (!bLockon && bFireplaceStrong == false)
	{
		Vec3 temp = fireplacePos;
		temp.y += markHigh;

		//	auto [a, b, c, d] = fireplaceController.update(fireplacePos, camera, m_eyePosition, ray, MarkPosition, 0, bPokerHave);
		auto [a, b, c, d] = fireplaceController.update(
			temp,
			camera,
			curCameraPosition,
			//	ray,
			markPosition,
			0,
			false
		);
		//if (a == true)
		//{
		//	// 火が強くなった
		//	bFireplaceStrong = true;
		//}
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 18;
			bFireplaceLockon = true;
		}
		else
		{
			bFireplaceLockon = false;
		}
		if (d)
		{
			message = 18;
			priorityMessageCount = 0;
		}
	}
	else
	{
		bFireplaceLockon = false;
	}

	// 汚れた布
	if (!bLockon && bDirtyClothHave == false)
	{
		Vec3 temp = dirtyClothPos;
		temp.y += markHigh;

		auto [a, b, c, d] = dirtyClothController.update(
			temp,
			camera,
			curCameraPosition,
			//	ray, 
			markPosition,
			0,
			true
		);
		if (a == true)
		{
			// 汚れた布を取得
			bDirtyClothHave = true;
			items << DirtyCloth;
			bgmStopCount = c;

			priorityMessage = 83;
			priorityMessageCount = priorityMessageCountMax;
		}
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 26;
		}
		if (d)
		{
			message = 26;
			priorityMessageCount = 0;
		}
	}

	// 引き出し（本体）
	if (!bLockon && bDrawerMode == false)
	{
		Vec3 temp = drawerPos[0];
		temp.x += 0.0;
		temp.y += 0.4 + markHigh;
	//	temp.z -= 0.3;
		temp.z -= 0.5;

		auto [a, b, c, d] = drawerController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			0,
			false
		);

		if (b)
		{
			// 見ている
			bLockon = b;

			if (bDrawerClear == false)
			{
				// クリアしていない
				bookingMessage = 9;
			}
			else
			{
				// クリアしている
				bookingMessage = 70;
			}
		}

		if (d)
		{
			if (bDrawerClear == false)
			{
				// クリックした
				bDrawerMode = true;

				// カメラの座標と向きを調整
				toCameraPos.x = 16.3;
				//	toCameraPos.y = 0.65;
				//	toCameraPos.y = 1.58;
			//	toCameraPos.y = 1.1;
			//	toCameraPos.y = 0.6;
			//	toCameraPos.y = 0.8;
				toCameraPos.y = 1;
				//	toCameraPos.z = 0.3;
				//	toCameraPos.z = 1.2;
				//toCameraPos.z = 0.55;
				toCameraPos.z = 0.4;

				//	to_m_focusY = 0;
				//to_m_focusY = -0.53;
			//	to_m_focusY = -0.2;
				to_m_focusY = -0.6;
				phiController.setCameraPosition(toCameraPos);
				phiController.setFocusPosition(drawerPos[0]);

				priorityMessage = 88;
				priorityMessageCount = priorityMessageCountMax;

				drawerIndex = 0;
			}
			else
			{
				// クリア後
				message = 70;
				priorityMessageCount = 0;
			}
		}
	}

	// 石板（本体）
	if (!bLockon && bStoneMode == false)
	{
		Vec3 temp = stonePos[0];
		temp.x -= 0.22;
		//temp.y += markHigh;
		temp.y += 0.1;
		//	temp.y += 0.5;
		//	temp.z -= 0.3;

		auto [a, b, c, d] = stoneController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			0,
			false
		);

		if (b)
		{
			// 見ている
			bLockon = b;
			if (bStoneclear == false)
			{
				// クリアしていない
				bookingMessage = 59;
			}
			else
			{
				// クリアしている
				bookingMessage = 70;
			}
		}

		if (d)
		{
			if (bStoneclear == false)
			{
				// クリアしていない

				// クリックした
				bStoneMode = true;

				// カメラの座標と向きを調整
				toCameraPos.x = 19.25;
				toCameraPos.y = 1.5;
				toCameraPos.z = -2.18;

				to_m_focusY = -0.53;

				phiController.setCameraPosition(toCameraPos);
				phiController.setFocusPosition(stonePos[0]);

				priorityMessage = 90;
				priorityMessageCount = priorityMessageCountMax;

				stoneIndex = 0;
			}
			else
			{
				// クリアしている
				message = 70;
				priorityMessageCount = 0;
			}
		}
	}

	//=======================
	// ↓優先度低め
	//=======================

	// 自分のベッド
	if (!bLockon)
	{
		Vec3 temp = bedPos;
		temp.y += markHigh;

		auto [a, b, c, d] = bedController.update(
			temp,
			camera,
			curCameraPosition,
		//	ray,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 11;
		}
		if (d)
		{
			message = 11;
			priorityMessageCount = 0;
		}
	}

	// 他人のベッド2
	if (!bLockon)
	{
		Vec3 temp = bed2Pos;
		temp.y += markHigh;

		auto [a, b, c, d] = bed2Controller.update(
			temp,
			camera,
			curCameraPosition, 
		//	ray,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 14;
		}
		if (d)
		{
			message = 14;
			priorityMessageCount = 0;
		}
	}

	// 他人のベッド3
	if (!bLockon)
	{
		Vec3 temp = bed3Pos;
		temp.y += markHigh;

		auto [a, b, c, d] = bed3Controller.update(
			temp,
			camera,
			curCameraPosition,
		//	ray, 
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 14;
		}
		if (d)
		{
			message = 14;
			priorityMessageCount = 0;
		}
	}

	// 他人のベッド4
	if (!bLockon)
	{
		Vec3 temp = bed4Pos;
		temp.y += markHigh;

		auto [a, b, c, d] = bed4Controller.update(
			temp,
			camera,
			curCameraPosition,
		//	ray,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 14;
		}
		if (d)
		{
			message = 14;
			priorityMessageCount = 0;
		}
	}

	// 古いベッド
	if (!bLockon)
	{
		Vec3 temp = oldBedPos;
		temp.y += markHigh;

		auto [a, b, c, d] = oldBedController.update(
			temp,
			camera,
			curCameraPosition,
		//	ray,
			markPosition,
			-1, 
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 15;
		}
		if (d)
		{
			message = 15;
			priorityMessageCount = 0;
		}
	}

	// トイレ
	if (!bLockon)
	{
		Vec3 temp = toiletPos;
		temp.y += markHigh;

		auto [a, b, c, d] = toiletController.update(
			temp,
			camera,
			curCameraPosition,
		//	ray,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 12;
		}
		if (d)
		{
			message = 12;
			priorityMessageCount = 0;
		}
	}

	// 棚
	if (!bLockon)
	{
		Vec3 temp = shelfPos;
		temp.y += markHigh;

		auto [a, b, c, d] = shelfController.update(
			temp,
			camera, 
			curCameraPosition,
		//	ray,
			markPosition,
			-1, 
			false
		);
		//if (a == true && bWire == false)
		//{
		//	// 針金が取れる
		////	items << Shelf;
		//	bWire = true;
		//	bgmStopCount = c;
		//	scenario = 4;
		//	bHangerFind = true;
		//}
		if (b)
		{
			// 見ている
			bLockon = b;
			//if (bKeyHave)
			//{
			//	// 鍵を持っている
			//	message = 17;
			//}
			//else
			//{
				// 鍵を持っていない
			bookingMessage = 13;
			//}
		}
		if (d)
		{
			message = 13;
			priorityMessageCount = 0;
		}
	}

	// 壁
	//if (!bLockon && !bWall && scenario == 2)
	//{
	//	auto [a, b, c, d] = wallController.update(wallPos, camera, m_eyePosition, ray, markPosition, 0, true);
	//	if (a == true && bParchmentHave == false)
	//	{
	//		// 鍵が現れる
	//		bWall = true;
	//		bgmStopCount = c;
	//	}
	//	if (b)
	//	{
	//		// 見ている
	//		bLockon = b;
	//		message = 16;
	//	}
	//}

	// 暖炉（火が強い）
	// オミット
	//if (!bLockon && bFireplaceStrong)
	//{
	//	auto [a, b, c, d] = fireplaceStrongController.update(fireplacePos, camera, m_eyePosition, ray, markPosition, -1, false);
	//	if (b)
	//	{
	//		// 見ている
	//		bLockon = b;
	//		message = 21;
	//		bFireplaceStrongLockon = true;
	//	}
	//	else
	//	{
	//		bFireplaceStrongLockon = false;
	//	}
	//}
	//else
	//{
	//	bFireplaceStrongLockon = false;
	//}

	// 樽
	if (!bLockon)
	{
		Vec3 temp = barrelPos;
		temp.y += markHigh;

		auto [a, b, c, d] = barrelController.update(
			temp,
			camera,
			curCameraPosition,
		//	ray,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 25;
		}
		if (d)
		{
			message = 25;
			priorityMessageCount = 0;
		}
	}

	// 樽2
	if (!bLockon)
	{
		Vec3 temp = barrel2Pos;
		temp.y += markHigh;

		auto [a, b, c, d] = barrelController.update(
			temp,
			camera, 
			curCameraPosition,
		//	ray,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 25;
		}
		if (d)
		{
			message = 25;
			priorityMessageCount = 0;
		}
	}

	// 樽3
	if (!bLockon)
	{
		Vec3 temp = barrel3Pos;
		temp.y += markHigh;

		auto [a, b, c, d] = barrelController.update(
			temp,
			camera, 
			curCameraPosition,
		//	ray,
			markPosition,
			-1, 
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 25;
			//	message = 27;

			//	bBarrel3Lockon = true;
		}
		else
		{
			//	bBarrel3Lockon = false;
		}
		if (d)
		{
			message = 25;
			priorityMessageCount = 0;
		}
	}
	else
	{
		//	bBarrel3Lockon = false;
	}

	// 教団の紋章
	if (!bLockon)
	{
		Vec3 temp = emblemPos;
		temp.y += markHigh;

		auto [a, b, c, d] = barrelController.update(
			temp,
			camera,
			curCameraPosition,
		//	ray,
			markPosition,
			-1, 
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 23;
		}
		if (d)
		{
			message = 23;
			priorityMessageCount = 0;
		}
	}

	// 暖炉上の絵画
	if (!bLockon)
	{
		Vec3 temp = painting2Pos;
		temp.y += markHigh;

		auto [a, b, c, d] = paintingController.update(
			temp,
			camera,
			curCameraPosition,
			//	ray,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 92;
		}
		if (d)
		{
			message = 92;
			priorityMessageCount = 0;
		}
	}

	// 詰所の壁際の椅子1
	if (!bLockon)
	{
		Vec3 temp = chairPos;
		temp.y += markHigh;

		auto [a, b, c, d] = chairController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 65;
		}
		if (d)
		{
			message = 65;
			priorityMessageCount = 0;
		}
	}

	// 詰所の壁際の椅子2
	if (!bLockon)
	{
		Vec3 temp = chair2Pos;
		temp.y += markHigh;

		auto [a, b, c, d] = chairController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 65;
		}
		if (d)
		{
			message = 65;
			priorityMessageCount = 0;
		}
	}

	// 詰所の壁際の椅子3
	if (!bLockon)
	{
		Vec3 temp = chair3Pos;
		temp.y += markHigh;

		auto [a, b, c, d] = chairController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 65;
		}
		if (d)
		{
			message = 65;
			priorityMessageCount = 0;
		}
	}

	// 詰所の壁際の椅子4
	if (!bLockon)
	{
		Vec3 temp = chair4Pos;
		temp.y += markHigh;

		auto [a, b, c, d] = chairController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 65;
		}
		if (d)
		{
			message = 65;
			priorityMessageCount = 0;
		}
	}

	// 詰所のテーブル
	if (!bLockon)
	{
		Vec3 temp = tablePos;
		temp.y += markHigh;

		auto [a, b, c, d] = chairController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 66;
		}
		if (d)
		{
			message = 66;
			priorityMessageCount = 0;
		}
	}

	// 木箱
	if (!bLockon)
	{
		Vec3 temp = woodenBoxPos;
		temp.y += markHigh;

		auto [a, b, c, d] = chairController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 67;
		}
		if (d)
		{
			message = 67;
			priorityMessageCount = 0;
		}
	}

	// 木箱2
	if (!bLockon)
	{
		Vec3 temp = woodenBox2Pos;
		temp.y += markHigh;

		auto [a, b, c, d] = chairController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 67;
		}
		if (d)
		{
			message = 67;
			priorityMessageCount = 0;
		}
	}

	// 木箱3
	if (!bLockon)
	{
		Vec3 temp = woodenBox3Pos;
		temp.y += markHigh;

		auto [a, b, c, d] = chairController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 67;
		}
		if (d)
		{
			message = 67;
			priorityMessageCount = 0;
		}
	}

	// 手錠1
	if (!bLockon)
	{
		Vec3 temp = handcuffsPos;
		temp.y += markHigh;

		auto [a, b, c, d] = handcuffsController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 72;
		}
		if (d)
		{
			message = 72;
			priorityMessageCount = 0;
		}
	}

	// 手錠2
	if (!bLockon)
	{
		Vec3 temp = handcuffs2Pos;
		temp.y += markHigh;

		auto [a, b, c, d] = handcuffsController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 72;
		}
		if (d)
		{
			message = 72;
			priorityMessageCount = 0;
		}
	}

	// 絵画
	if (!bLockon)
	{
		Vec3 temp = paintingPos;
		temp.y += markHigh;

		auto [a, b, c, d] = paintingController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 73;
		}
		if (d)
		{
			message = 73;
			priorityMessageCount = 0;
		}
	}

	// 小さい棚
	if (!bLockon)
	{
		Vec3 temp = smallShelfPos;
		temp.y += markHigh;

		auto [a, b, c, d] = smallShelfController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 74;
		}
		if (d)
		{
			message = 74;
			priorityMessageCount = 0;
		}
	}

	// 小さい棚2
	if (!bLockon)
	{
		Vec3 temp = smallShelf2Pos;
		temp.y += markHigh;

		auto [a, b, c, d] = smallShelfController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 74;
		}
		if (d)
		{
			message = 74;
			priorityMessageCount = 0;
		}
	}

	// 小さい棚3
	if (!bLockon)
	{
		Vec3 temp = smallShelf3Pos;
		temp.y += markHigh;

		auto [a, b, c, d] = smallShelfController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 74;
		}
		if (d)
		{
			message = 74;
			priorityMessageCount = 0;
		}
	}

	//  脱出の扉の左にボード
	if (!bLockon)
	{
		Vec3 temp = boardPos;
		temp.y += markHigh;

		auto [a, b, c, d] = boardController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false
		);
		if (b)
		{
			// 見ている
			bLockon = b;
			bookingMessage = 93;
		}
		if (d)
		{
			message = 93;
			priorityMessageCount = 0;
		}
	}

}