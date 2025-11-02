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
	backgroundColor = ColorF{ 0.4, 0.6, 0.8 }.removeSRGBCurve();

	// 環境光を設定する
	Graphics3D::SetGlobalAmbientColor(ColorF{ 0.75, 0.75, 0.75 });

	// 太陽光を設定する
	Graphics3D::SetSunColor(ColorF{ 0.5, 0.5, 0.5 });

	// 太陽の方向を設定する
	Graphics3D::SetSunDirection(Vec3{ 0, 1, -0.3 }.normalized());
	
	// カメラ
	camera = BasicCamera3D{ renderTexture.size(), m_verticalFOV, Vec3{ 0, 10, -2 } };

	// 最初にカーソルを中央に
	Cursor::SetPos(center.x, center.y);

	// マウスの初期化
	mousePosX = Cursor::PosF().x;
	mousePosY = Cursor::PosF().y;
	toMousePosX = mousePosX;
	toMousePosY = mousePosY;
	virtualCursorPos.x = center.x;
	virtualCursorPos.y = center.y;

	 Vec3 initialFocusPosition = Vec3{ 0, 0, -3 };
	 phiController.setFocusPosition(initialFocusPosition);

#ifdef _DEBUG
	bDebugViewCollision = true;
#endif
	// ストップウォッチ（なるべく最後に実行する）
	Stopwatch stopwatch{ StartImmediately::Yes };
}

void CameraTest::loadResources() const
{
	// ローディング画面でリソース読み込み

	switch (loadCount)
	{
		case 0:
			roomModel = std::make_unique<Model>(roomModelPath);
			Model::RegisterDiffuseTextures(*roomModel, TextureDesc::MippedSRGB);
			break;
		case 1:
			modelExclamationMark = std::make_unique<Model>(modelExclamationMarkPath);
			Model::RegisterDiffuseTextures(*modelExclamationMark, TextureDesc::MippedSRGB);
			break;
		case 2:
			prideRelicModel = std::make_unique<Model>(relicPrideModelPath);
			Model::RegisterDiffuseTextures(*prideRelicModel, TextureDesc::MippedSRGB);
			break;
		case 3:
			greedRelicModel = std::make_unique<Model>(relicGreedModelPath);
			Model::RegisterDiffuseTextures(*greedRelicModel, TextureDesc::MippedSRGB);
			break;
		case 4:
			envyRelicModel = std::make_unique<Model>(relicEnvyModelPath);
			Model::RegisterDiffuseTextures(*envyRelicModel, TextureDesc::MippedSRGB);
			break;
		case 5:
			wrathRelicModel = std::make_unique<Model>(relicWrathModelPath);
			Model::RegisterDiffuseTextures(*wrathRelicModel, TextureDesc::MippedSRGB);
			break;
		case 6:
			lustRelicModel = std::make_unique<Model>(relicLustModelPath);
			Model::RegisterDiffuseTextures(*lustRelicModel, TextureDesc::MippedSRGB);
			break;
		case 7:
			gluttonyRelicModel = std::make_unique<Model>(relicGluttonyModelPath);
			Model::RegisterDiffuseTextures(*gluttonyRelicModel, TextureDesc::MippedSRGB);
			break;
		case 8:
			slothRelicModel = std::make_unique<Model>(relicSlothModelPath);
			Model::RegisterDiffuseTextures(*slothRelicModel, TextureDesc::MippedSRGB);
			break;
		case 9:
			dummyTextView(Text);
			break;
		case 10:
			dummyTextView(itemText);
			break;
		case 11:
			dummyTextView(itemNameText);
			break;
		case 12:
			dummyTextView(prologueText);
			break;
		case 13:
			dummyTextView(pictureText);
			break;
		case 14:
			dummyTextView(exitPlateText);
			break;
		case 15:
			// BGMの読み込み
			AudioAsset(U"BGM").setVolume(0.0);
			AudioAsset(U"BGM").play();
			AudioAsset(U"BGM").stop();
			break;
		case 16:
			AudioAsset(U"footsteps1").setVolume(0.0);
			AudioAsset(U"footsteps1").play();
			AudioAsset(U"footsteps1").stop();
			break;
		case 17:
			AudioAsset(U"footsteps2").setVolume(0.0);
			AudioAsset(U"footsteps2").play();
			AudioAsset(U"footsteps2").stop();
			break;
		case 18:
			AudioAsset(U"Item").setVolume(0.0);
			AudioAsset(U"Item").play();
			AudioAsset(U"Item").stop();
			break;
		case 19:
			AudioAsset(U"ExitDoorOpen").setVolume(0.0);
			AudioAsset(U"ExitDoorOpen").play();
			AudioAsset(U"ExitDoorOpen").stop();
			break;
		default:
			bLoaded = true;	// リソースが読み込まれた
			break;
	}
	loadCount++;
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

		// パンをスキップ
		scenario = 3;
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

	Print << U"ChairState=" << chairStates;
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
		case PrideRelic:
			// 傲慢の遺物
			prideRelicUiSprite.resized(inventoryMini, inventoryMini).draw(x, y);
			break;
		case GreedRelic:
			// 強欲の遺物
			greedRelicUiSprite.resized(inventoryMini, inventoryMini).draw(x, y);
			break;
		case EnvyRelic:
			// 嫉妬の遺物
			envyRelicUiSprite.resized(inventoryMini, inventoryMini).draw(x, y);
			break;
		case WrathRelic:
			// 憤怒の遺物
			wrathRelicUiSprite.resized(inventoryMini, inventoryMini).draw(x, y);
			break;
		case LustRelic:
			// 色欲の遺物
			lustRelicUiSprite.resized(inventoryMini, inventoryMini).draw(x, y);
			break;
		case GluttonyRelic:
			// 暴食の遺物
			gluttonyRelicUiSprite.resized(inventoryMini, inventoryMini).draw(x, y);
			break;
		case SlothRelic:
			// 怠惰の遺物
			slothRelicUiSprite.resized(inventoryMini, inventoryMini).draw(x, y);
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
		case PrideRelic:
			// 傲慢の遺物
			prideRelicUiSprite.resized(inventoryBig, inventoryBig).draw(x, y);
			break;
		case GreedRelic:
			// 強欲の遺物
			greedRelicUiSprite.resized(inventoryBig, inventoryBig).draw(x, y);
			break;
		case EnvyRelic:
			// 嫉妬の遺物
			envyRelicUiSprite.resized(inventoryBig, inventoryBig).draw(x, y);
			break;
		case WrathRelic:
			// 憤怒の遺物
			wrathRelicUiSprite.resized(inventoryBig, inventoryBig).draw(x, y);
			break;
		case LustRelic:
			// 色欲の遺物
			lustRelicUiSprite.resized(inventoryBig, inventoryBig).draw(x, y);
			break;
		case GluttonyRelic:
			// 暴食の遺物
			gluttonyRelicUiSprite.resized(inventoryBig, inventoryBig).draw(x, y);
			break;
		case SlothRelic:
			// 怠惰の遺物
			slothRelicUiSprite.resized(inventoryBig, inventoryBig).draw(x, y);
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

	// ロックオンをオフ
	bLockon = false;

	if (!bClear)
	{
		playSE(U"Inventory");
	}
	
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
		if (priorityMessageCount > 0)
		{
			// 優先メッセージを表示
			message = priorityMessage;
			priorityMessageCount -= deltaTime;
			messagePattern = 0;
		}
		else if (bLockon)
		{
			// 何か見ているときは更新しない
			if (bookingMessage != message)
			{
				// 違うオブジェクトにカーソルが移動したら、いったんメッセージを非表示にする
				message = 3;
				messagePattern = -1;
			}
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
	else if (bWatchPicture || bWatchExitPlate)
	{
		// 絵画かパネルを見ている
		if (Window::GetState().focused)
		{
			// マウスカーソルを非表示
			Cursor::RequestStyle(CursorStyle::Hidden);

			// カーソルを強制的に中央に戻す
			Cursor::SetPos(center.x, center.y);
		}
	}
	else if (!bInventory)
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

		// ロックオン
		lockon();

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
	if ((!bWatchPicture && !bWatchExitPlate && !bPlayEpilogue) && (KeySpace.down() || xboxController.buttonY.down()))
	{
		inventoryOnOff();
	}

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
		for (const auto& object : roomModel->objects())
		{
			const std::array<Vec3, 8> cube = object.boundingBox.getCorners();

			for (int i = 0; i < 12; i++)
			{
				// Triangle_ tri = {
				// 	Vec3_{cube[collisionTriangle[i][0]].x / 100, cube[collisionTriangle[i][0]].y / 100, cube[collisionTriangle[i][0]].z / 100},
				// 	Vec3_{cube[collisionTriangle[i][1]].x / 100, cube[collisionTriangle[i][1]].y / 100, cube[collisionTriangle[i][1]].z / 100},
				// 	Vec3_{cube[collisionTriangle[i][2]].x / 100, cube[collisionTriangle[i][2]].y / 100, cube[collisionTriangle[i][2]].z / 100}
				// };

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
					Vec2 tangent = Vec2(wallNormal.y, -wallNormal.x); // 法線に直交するベクトル

					// 今の速度を壁に沿ってスライド（内積で投影）
					Vec2 resultVelocity = tangent * (velocityNormal.dot(tangent));

					Vec2 resultVelocity2 = Vec2(
						resultVelocity.y,
						resultVelocity.x
					);

					double dot = (velocityNormal.dot(resultVelocity2));
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

					const auto& object2Temp = roomModel->objects();
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

	// ライト位置
	lightPos = curCameraPosition;

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

	// UI
	if (bInventory)
	{
		// インベントリ表示
		viewInventory();
	}
	else if (bWatchPicture || bWatchExitPlate)
	{
		// テキストを開いてから1フレーム待つ
		if (pictureOpenFrameCount > 0)
		{
			pictureOpenFrameCount--;
		}
		else if (MouseL.down() || xboxController.buttonA.down())
		{
			// 閉じる
			bWatchPicture = false;
			bWatchExitPlate = false;
		}
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

		if (prologueCount > prologueTiming)
		{
			bPrologueMessageEnd = true;
		}
	}

	// エピローグ
	if(bPlayEpilogue && !bEpilogueMessageEnd)
	{
		epilogueCount += deltaTime;

		if (MouseL.down() || xboxController.buttonA.down())
		{
			if (epilogueCount < 28)
			{
				float epilogueIndex = epilogueCount * 5;
				epilogueCount = 0;
				for (int i = 0; i < epilogueText.size(); ++i)
				{
					epilogueCount += (float)epilogueText[i].size() / 5.0f;
					epilogueIndex -= epilogueText[i].size();

					if (epilogueIndex <= 0)
					{
						break;
					}
				}
			}
		}

		if (epilogueCount > epilogueTiming)
		{
			bEpilogueMessageEnd = true;

			// message = 35;
		}
	}


	if (bPrologueMessageEnd == true && bStartPlaying == false)
	{
		// プロローグカウンター2
		
		// 2倍速
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

		bStartPlaying = true;
		messageCount = 9999;
	}

	if (bStartPlaying)
	{
		// 止まっているBGMを再度鳴らす
		{
			if (bgmStopCount <= 0.0f)
			{
				// BGMの再開
				if (!AudioAsset(U"BGM").isPlaying())
				{
					AudioAsset(U"BGM").setVolume(BGMVolume);
					AudioAsset(U"BGM").play();
				}
			}
			else {
				bgmStopCount -= deltaTime;
			}
		}
	}

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
		double adjustLineSpacing = 64; // 2行連続の場合の行間

		// 今、表示するための文字数
		int prologueIndex = prologueCount*5;

		// テキストのアルファ値
		float a = prologueCount * 5 - (float)prologueIndex;

		// 描画位置の高さ
		double height = 0;

		int emptyLineCount = 1;

		for (int i = 0; i < prologueText.size(); ++i)
		{
			if (prologueIndex <= 0)
			{
				break;
			}

			// 空行はスキップ
			if (prologueText[i].isEmpty())
			{
				emptyLineCount++;
				continue;
			}

			double x = center.x;
			double spacing = adjustLineSpacing;

			if (i > 0 && emptyLineCount >= 1)
			{
				// 改行が含まれる場合
				spacing = lineSpacing * emptyLineCount;
			}

			if (i == 0)
			{
				// 最初の行は中央基準で計算
				// 空文字列を除いた行数を計算
				int nonEmptyLines = 0;
				for (const auto& text : prologueText)
				{
					if (!text.isEmpty()) nonEmptyLines++;
				}
				
				// 全体の高さを計算（文字がある行の間は半分の行間）
				double totalHeight = lineSpacing * (nonEmptyLines - 1) / 2.0;
				double topMargin = 40;
				height = center.y - totalHeight + topMargin;
			}
			else if (i > 0)
			{
				height += spacing;
			}

			emptyLineCount = 0;
			double y = height;

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

	// エピローグ暗転
	if (bPlayEpilogue)
	{
		float epilogueAlpha = epilogueCount;
		if (epilogueAlpha > 1)
		{
			epilogueAlpha = 1;
		}

		// 画面全体を黒で描画
		Rect{ 0, 0, Scene::Width(), Scene::Height() }.draw(ColorF{ 0.0, epilogueAlpha });

		// TODO 共通化する
		double lineSpacing = 96; // 行間（フォントサイズより少し大きめ）
		double adjustLineSpacing = 64; // 2行連続の場合の行間

		// 今、表示するための文字数
		int epilogueIndex = epilogueCount*5;

		// テキストのアルファ値
		float a = epilogueCount * 5 - (float)epilogueIndex;

		// 描画位置の高さ
		double height = 0;

		int emptyLineCount = 1;

		for (int i = 0; i < epilogueText.size(); ++i)
		{
			if (epilogueIndex <= 0)
			{
				break;
			}

			// 空行はスキップ
			if (epilogueText[i].isEmpty())
			{
				emptyLineCount++;
				continue;
			}

			double x = center.x;
			double spacing = adjustLineSpacing;

			if (i > 0 && emptyLineCount >= 1)
			{
				// 改行が含まれる場合
				spacing = lineSpacing * emptyLineCount;
			}

			if (i == 0)
			{
				// 最初の行は中央基準で計算
				// 空文字列を除いた行数を計算
				int nonEmptyLines = 0;
				for (const auto& text : epilogueText)
				{
					if (!text.isEmpty()) nonEmptyLines++;
				}
				
				// 全体の高さを計算（文字がある行の間は半分の行間）
				double totalHeight = lineSpacing * (nonEmptyLines - 1) / 2.0;
				double topMargin = 40;
				height = center.y - totalHeight + topMargin;
			}
			else if (i > 0)
			{
				height += spacing;
			}

			emptyLineCount = 0;
			double y = height;

			int num = 0;
			int sub = 0;

			if (epilogueIndex >= epilogueText[i].size())
			{
				// すべて表示
				num = epilogueText[i].size();
				epilogueIndex -= epilogueText[i].size();
			}
			else
			{
				// 一部表示
				num = epilogueIndex;
				sub = epilogueText[i].size() - num;
				epilogueIndex = 0;
			}

			String tmp = epilogueText[i].substr(0, num);
			tmp.append(sub, U'　');	// 後ろをスペースで埋める

			boldFont(tmp).drawAt(
				28,
				{ x, y },
				ColorF{ 1, 1, 1, epilogueAlpha }
			);

			// 半透明文字
			if (num+1 < epilogueText[i].size())
			{
				String tmp2;
				tmp2.append(num, U'　');		// 後ろをスペースで埋める
				tmp2.append(1, epilogueText[i][num]);
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
		// TODO 共通化する
		double lineSpacing = 40.0; // 行間（フォントサイズより少し大きめ）

		for (int i = 0; i < itemText[itemMessage].size(); ++i)
		{
			double x = center.x;
			double y = center.y + 150 + lineSpacing * i;
			int fontSize = 28;

			// 長すぎる説明文だった場合はフォントサイズを小さくする
			if (itemText[itemMessage][0].size() > 16)
			{
				fontSize = 20;
			}

			boldFont(itemText[itemMessage][i]).drawAt(
				fontSize,
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

	// 絵画
	if (bWatchPicture)
	{
		// 半透明の黒い画像
		Rect{ 0, 0, Scene::Width(), Scene::Height() }.draw(ColorF{ 0.0, 0.5 });

		// TODO 共通化する
		double lineSpacing = 40.0; // 行間（フォントサイズより少し大きめ）

		for (int i = 0; i < pictureText[pictureIndex].size(); ++i)
		{
			double x = center.x;
			double y = center.y + lineSpacing * (i - (int)pictureText[pictureIndex].size() / 2);

			boldFont(pictureText[pictureIndex][i]).drawAt(
				28,
				{ x, y },
				ColorF{ 1, 1, 1, 1 }
			);
		}
	}

	if (bWatchExitPlate)
	{
		// 半透明の黒い画像
		Rect{ 0, 0, Scene::Width(), Scene::Height() }.draw(ColorF{ 0.0, 0.5 });

		// TODO 共通化する
		double lineSpacing = 40.0; // 行間（フォントサイズより少し大きめ）

		for (int i = 0; i < exitPlateText.size(); ++i)
		{
			double x = center.x;
			double y = center.y + lineSpacing * (i - (int)exitPlateText.size() / 2);

			boldFont(exitPlateText[i]).drawAt(
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
	//AudioAsset(U"bonfire").stop();
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
		const auto& objectTmp = roomModel->objects();
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

	Print << U"m_focusPosition" << camera.getFocusPosition();
	modelExclamationMark->draw(camera.getFocusPosition());

}

// インベントリ表示
void CameraTest::viewInventory()
{
	// エピローグ中は表示しない
	if (bPlayEpilogue)
	{
		return;
	}

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

		// 現在選択中のアイテムに枠を表示
		Rect rect(itemMiniX, itemMiniY, itemMiniWidth, itemMiniHeight);

		if (KeyMode == false)
		{
			if (rect.mouseOver())
			{
				selectItem = i;

				Color rectColor = Palette::Skyblue;

				// 枠線
				rect.drawFrame(2, rectColor);

				// コメント
				itemMessage = items[selectItem];
			}
			else
			{
				Color rectColor = Palette::Black;

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

				// 枠線
				rect.drawFrame(2, rectColor);

				// コメント
				itemMessage = items[selectItem];
			}
			else
			{
				Color rectColor = Palette::Black;

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
	}

	// アイテムを使う
	if (MouseL.down() || xboxController.buttonA.down())
	{
		if (items.size() <= selectItem)
		{
			// アイテムのない場所は処理しない
		}
		else if (items[selectItem] == PrideRelic)
		{
			// 椅子の前で使う
			if (bFrontOfChair)
			{
				auto chairState = getChairState(frontOfChairIndex);
				if (chairState == ChairState::Placed || chairState == ChairState::Collect)
				{
					// すでにレリックが置かれている場合は置けない
					priorityMessage = chairAlreadySetRelicMessage;
					priorityMessageCount = priorityMessageCountMax;
					return;
				}

				int prideRelicNum = static_cast<int>(PrideRelic);
				// レリックと椅子のNumが同じか確認
				ChairState setCurrentChairState = prideRelicNum == frontOfChairIndex ? ChairState::Collect : ChairState::Placed;
				setChairState(frontOfChairIndex, setCurrentChairState, PrideRelic);
				// 再描画処理とインベントリから削除適用
				bShowRelicArray[prideRelicNum] = true;
				bRelicHaveArray[prideRelicNum] = false;
				bRelicPlacedArray[prideRelicNum] = true;
				items.remove_at(selectItem);
				selectItem = -1; // アイテムがなくなったので選択解除

				// 対象の椅子の位置に置く
				prideRelicCurrentPos = relicOnChairPos[frontOfChairIndex];
				// 王冠のみ90度ずれているので調整
				relicRotY[prideRelicNum] = chairRotY[frontOfChairIndex] + 90_deg;

				// クリア判定
				bClear = clearCheck();

				// インベントリを閉じる
				inventoryOnOff();
			}
			else
			{
				// 椅子の前でない場合
				priorityMessage = relicNotFrontOfChairMessage;
				priorityMessageCount = priorityMessageCountMax;
			}
		}
		else if (items[selectItem] == GreedRelic)
		{
			// 椅子の前で使う
			if (bFrontOfChair)
			{
				auto chairState = getChairState(frontOfChairIndex);
				if (chairState == ChairState::Placed || chairState == ChairState::Collect)
				{
					// すでにレリックが置かれている場合は置けない
					priorityMessage = chairAlreadySetRelicMessage;
					priorityMessageCount = priorityMessageCountMax;
					return;
				}

				int greedRelicNum = static_cast<int>(GreedRelic);
				// レリックと椅子のNumが同じか確認
				ChairState setCurrentChairState = greedRelicNum == frontOfChairIndex ? ChairState::Collect : ChairState::Placed;
				setChairState(frontOfChairIndex, setCurrentChairState, GreedRelic);
				// 再描画処理とインベントリから削除適用
				bShowRelicArray[greedRelicNum] = true;
				bRelicHaveArray[greedRelicNum] = false;
				bRelicPlacedArray[greedRelicNum] = true;
				items.remove_at(selectItem);
				selectItem = -1; // アイテムがなくなったので選択解除

				// 対象の椅子の位置に置く
				greedRelicCurrentPos = relicOnChairPos[frontOfChairIndex];
				relicRotY[greedRelicNum] = chairRotY[frontOfChairIndex];

				// クリア判定
				bClear = clearCheck();

				// インベントリを閉じる
				inventoryOnOff();
			}
			else
			{
				// 椅子の前でない場合
				priorityMessage = relicNotFrontOfChairMessage;
				priorityMessageCount = priorityMessageCountMax;
			}
		}
		else if (items[selectItem] == EnvyRelic)
		{
			// 椅子の前で使う
			if (bFrontOfChair)
			{
				auto chairState = getChairState(frontOfChairIndex);
				if (chairState == ChairState::Placed || chairState == ChairState::Collect)
				{
					// すでにレリックが置かれている場合は置けない
					priorityMessage = chairAlreadySetRelicMessage;
					priorityMessageCount = priorityMessageCountMax;
					return;
				}

				int envyRelicNum = static_cast<int>(EnvyRelic);
				// レリックと椅子のNumが同じか確認
				ChairState setCurrentChairState = envyRelicNum == frontOfChairIndex ? ChairState::Collect : ChairState::Placed;
				setChairState(frontOfChairIndex, setCurrentChairState, EnvyRelic);
				// 再描画処理とインベントリから削除適用
				bShowRelicArray[envyRelicNum] = true;
				bRelicHaveArray[envyRelicNum] = false;
				bRelicPlacedArray[envyRelicNum] = true;
				items.remove_at(selectItem);
				selectItem = -1; // アイテムがなくなったので選択解除

				// 対象の椅子の位置に置く
				envyRelicCurrentPos = relicOnChairPos[frontOfChairIndex];
				relicRotY[envyRelicNum] = chairRotY[frontOfChairIndex];

				// クリア判定
				bClear = clearCheck();

				// インベントリを閉じる
				inventoryOnOff();
			}
			else
			{
				// 椅子の前でない場合
				priorityMessage = relicNotFrontOfChairMessage;
				priorityMessageCount = priorityMessageCountMax;
			}
		}
		else if (items[selectItem] == WrathRelic)
		{
			// 椅子の前で使う
			if (bFrontOfChair)
			{
				auto chairState = getChairState(frontOfChairIndex);
				if (chairState == ChairState::Placed || chairState == ChairState::Collect)
				{
					// すでにレリックが置かれている場合は置けない
					priorityMessage = chairAlreadySetRelicMessage;
					priorityMessageCount = priorityMessageCountMax;
					return;
				}

				int wrathRelicNum = static_cast<int>(WrathRelic);
				// レリックと椅子のNumが同じか確認
				ChairState setCurrentChairState = wrathRelicNum == frontOfChairIndex ? ChairState::Collect : ChairState::Placed;
				setChairState(frontOfChairIndex, setCurrentChairState, WrathRelic);
				// 再描画処理とインベントリから削除適用
				bShowRelicArray[wrathRelicNum] = true;
				bRelicHaveArray[wrathRelicNum] = false;
				bRelicPlacedArray[wrathRelicNum] = true;
				items.remove_at(selectItem);
				selectItem = -1; // アイテムがなくなったので選択解除

				// 対象の椅子の位置に置く
				wrathRelicCurrentPos = relicOnChairPos[frontOfChairIndex];
				relicRotY[wrathRelicNum] = chairRotY[frontOfChairIndex];

				// クリア判定
				bClear = clearCheck();

				// インベントリを閉じる
				inventoryOnOff();
			}
			else
			{
				// 椅子の前でない場合
				priorityMessage = relicNotFrontOfChairMessage;
				priorityMessageCount = priorityMessageCountMax;
			}
		}
		else if (items[selectItem] == LustRelic)
		{
			// 椅子の前で使う
			if (bFrontOfChair)
			{
				auto chairState = getChairState(frontOfChairIndex);
				if (chairState == ChairState::Placed || chairState == ChairState::Collect)
				{
					// すでにレリックが置かれている場合は置けない
					priorityMessage = chairAlreadySetRelicMessage;
					priorityMessageCount = priorityMessageCountMax;
					return;
				}

				int lustRelicNum = static_cast<int>(LustRelic);
				// レリックと椅子のNumが同じか確認
				ChairState setCurrentChairState = lustRelicNum == frontOfChairIndex ? ChairState::Collect : ChairState::Placed;
				setChairState(frontOfChairIndex, setCurrentChairState, LustRelic);
				// 再描画処理とインベントリから削除適用
				bShowRelicArray[lustRelicNum] = true;
				bRelicHaveArray[lustRelicNum] = false;
				bRelicPlacedArray[lustRelicNum] = true;
				items.remove_at(selectItem);
				selectItem = -1; // アイテムがなくなったので選択解除

				// 対象の椅子の位置に置く
				lustRelicCurrentPos = relicOnChairPos[frontOfChairIndex];
				relicRotY[lustRelicNum] = chairRotY[frontOfChairIndex];

				// クリア判定
				bClear = clearCheck();

				// インベントリを閉じる
				inventoryOnOff();
			}
			else
			{
				// 椅子の前でない場合
				priorityMessage = relicNotFrontOfChairMessage;
				priorityMessageCount = priorityMessageCountMax;
			}
		}
		else if (items[selectItem] == GluttonyRelic)
		{
			// 椅子の前で使う
			if (bFrontOfChair)
			{
				auto chairState = getChairState(frontOfChairIndex);
				if (chairState == ChairState::Placed || chairState == ChairState::Collect)
				{
					// すでにレリックが置かれている場合は置けない
					priorityMessage = chairAlreadySetRelicMessage;
					priorityMessageCount = priorityMessageCountMax;
					return;
				}

				int gluttonyRelicNum = static_cast<int>(GluttonyRelic);
				// レリックと椅子のNumが同じか確認
				ChairState setCurrentChairState = gluttonyRelicNum == frontOfChairIndex ? ChairState::Collect : ChairState::Placed;
				setChairState(frontOfChairIndex, setCurrentChairState, GluttonyRelic);
				// 再描画処理とインベントリから削除適用
				bShowRelicArray[gluttonyRelicNum] = true;
				bRelicHaveArray[gluttonyRelicNum] = false;
				bRelicPlacedArray[gluttonyRelicNum] = true;
				items.remove_at(selectItem);
				selectItem = -1; // アイテムがなくなったので選択解除

				// 対象の椅子の位置に置く
				gluttonyRelicCurrentPos = relicOnChairPos[frontOfChairIndex];
				relicRotY[gluttonyRelicNum] = chairRotY[frontOfChairIndex];

				// クリア判定
				bClear = clearCheck();

				// インベントリを閉じる
				inventoryOnOff();
			}
			else
			{
				// 椅子の前でない場合
				priorityMessage = relicNotFrontOfChairMessage;
				priorityMessageCount = priorityMessageCountMax;
			}
		}
		else if (items[selectItem] == SlothRelic)
		{
			// 椅子の前で使う
			if (bFrontOfChair)
			{
				auto chairState = getChairState(frontOfChairIndex);
				if (chairState == ChairState::Placed || chairState == ChairState::Collect)
				{
					// すでにレリックが置かれている場合は置けない
					priorityMessage = chairAlreadySetRelicMessage;
					priorityMessageCount = priorityMessageCountMax;
					return;
				}

				int slothRelicNum = static_cast<int>(SlothRelic);
				// レリックと椅子のNumが同じか確認
				ChairState setCurrentChairState = slothRelicNum == frontOfChairIndex ? ChairState::Collect : ChairState::Placed;
				setChairState(frontOfChairIndex, setCurrentChairState, SlothRelic);
				// 再描画処理とインベントリから削除適用
				bShowRelicArray[slothRelicNum] = true;
				bRelicHaveArray[slothRelicNum] = false;
				bRelicPlacedArray[slothRelicNum] = true;
				items.remove_at(selectItem);
				selectItem = -1; // アイテムがなくなったので選択解除

				// 対象の椅子の位置に置く
				slothRelicCurrentPos = relicOnChairPos[frontOfChairIndex];
				relicRotY[slothRelicNum] = chairRotY[frontOfChairIndex];

				// クリア判定
				bClear = clearCheck();

				// インベントリを閉じる
				inventoryOnOff();
			}
			else
			{
				// 椅子の前でない場合
				priorityMessage = relicNotFrontOfChairMessage;
				priorityMessageCount = priorityMessageCountMax;
			}
		}
	}
}

void CameraTest::viewModel()
{
	const ScopedRenderTarget3D target{ renderTexture.clear(backgroundColor) };

	// 点光源を設定する
	constantBuffer->setPointLight(0, lightPos, ColorF{ 1.0, 1.0, 1.0 }, lightStrong);

	const ScopedCustomShader3D shader(vs3D, ps3D);

	// ピクセルシェーダに定数バッファを渡す
	Graphics3D::SetPSConstantBuffer(4, constantBuffer);

	constantBuffer->drawPointLightAsEmissiveSphere(0, 0);
	constantBuffer->drawPointLightAsEmissiveSphere(1, 0.1);

	// モデルを描画
	if (bDebugviewModel)
	{
		Transformer3D t{ Mat4x4::RotateY(0_deg).scaled(roomScale).translated(roomPos) };
		roomModel->draw();
	}

	// 傲慢のレリック
	if (bShowRelicArray[static_cast<int>(PrideRelic)])
	{
		Transformer3D t{ Mat4x4::RotateY(relicRotY[static_cast<int>(PrideRelic)]).scaled(0.007).translated(prideRelicCurrentPos) };
		prideRelicModel->draw();
	}

	// 強欲のレリック
	if (bShowRelicArray[static_cast<int>(GreedRelic)])
	{
		Transformer3D t{ Mat4x4::RotateY(relicRotY[static_cast<int>(GreedRelic)]).scaled(0.01).translated(greedRelicCurrentPos) };
		greedRelicModel->draw();
	}

	// 嫉妬のレリック
	if (bShowRelicArray[static_cast<int>(EnvyRelic)])
	{
		Transformer3D t{ Mat4x4::RotateY(relicRotY[static_cast<int>(EnvyRelic)]).scaled(0.01).translated(envyRelicCurrentPos) };
		envyRelicModel->draw();
	}
	
	// 憤怒のレリック
	if (bShowRelicArray[static_cast<int>(WrathRelic)])
	{
		Transformer3D t{ Mat4x4::RotateY(relicRotY[static_cast<int>(WrathRelic)]).scaled(0.01).translated(wrathRelicCurrentPos) };
		wrathRelicModel->draw();
	}

	// 色欲のレリック
	if (bShowRelicArray[static_cast<int>(LustRelic)])
	{
		Transformer3D t{ Mat4x4::RotateY(relicRotY[static_cast<int>(LustRelic)]).scaled(0.01).translated(lustRelicCurrentPos) };
		lustRelicModel->draw();
	}

	// 暴食のレリック
	if (bShowRelicArray[static_cast<int>(GluttonyRelic)])
	{
		Transformer3D t{ Mat4x4::RotateY(relicRotY[static_cast<int>(GluttonyRelic)]).scaled(0.01).translated(gluttonyRelicCurrentPos) };
		gluttonyRelicModel->draw();
	}

	// 怠惰のレリック
	if (bShowRelicArray[static_cast<int>(SlothRelic)])
	{
		Transformer3D t{ Mat4x4::RotateY(relicRotY[static_cast<int>(SlothRelic)]).scaled(0.01).translated(slothRelicCurrentPos) };
		slothRelicModel->draw();
	}

	// ビックリマークのビルボード
	if (bLockon)
	{
		// ビックリマークとカメラの距離
		double distance = curCameraPosition.distanceFrom(markPosition);

		float tempSize = distance * markSize;
#ifdef _DEBUG
		//Print << U"distance" << distance;
		//Print << U"tempSize" << tempSize;
#endif
		const ScopedRenderStates3D blend{ BlendState::NonPremultiplied };
		// ビルボードの表示
		const Mat4x4 billboardMat = camera.getInvView();
		billboard.draw(camera.billboard(markPosition, tempSize), uvChecker);
	}

	// デバッグ表示
#if _DEBUG
	debugViewCollision();
#endif
}

void CameraTest::setChairState(int chairIndex, ChairState state, ItemID relic)
{
	// 指定した椅子の状態を設定
	unsigned int chairMask = 0b11 << (chairIndex * BITS_PER_CHAIR_CELL);
	chairStates = ( chairStates & ~chairMask ) | ( (static_cast<unsigned int>(state) << (chairIndex * BITS_PER_CHAIR_CELL)) );

	relicsOnChairs[chairIndex] = relic;
}

ChairState CameraTest::getChairState(int chairIndex) const
{
	return static_cast<ChairState>(( chairStates >> (chairIndex * BITS_PER_CHAIR_CELL) ) & 0b11);
}

ItemID CameraTest::getRelicOnChair(int chairIndex) const
{
	return relicsOnChairs[chairIndex];
}

bool CameraTest::clearCheck()
{
    // すべてのビットが1かチェック
    bool isClear = (chairStates & ALL_COLLECTED_MASK) == ALL_COLLECTED_MASK;

	// クリアメッセージ表示
	if (isClear)
	{
		priorityMessage = allRelicPlacedMessage;
		priorityMessageCount = priorityMessageCountMax;

		// SEを鳴らす
		if (AudioAsset(U"ExitDoorOpen").isPlaying())
		{
			AudioAsset(U"ExitDoorOpen").stop();
		}
		AudioAsset(U"ExitDoorOpen").setVolume(1.0);
		AudioAsset(U"ExitDoorOpen").play();
	}

    return isClear;
}


void CameraTest::lockon()
{
	// エピローグ中は処理しない
	if (bPlayEpilogue)
	{
		return;
	}

	// アイテムのロックオンフラグのリセット
	bLockon = false;
	bFrontOfChair = false;
	// ロックオン中の椅子のindexリセット?
	frontOfChairIndex = -1;

	// 傲慢の椅子
	if(!bLockon)
	{
		Vec3 temp = prideChairPos;
		int prideChairIndex = static_cast<int>(PrideRelic);

		auto [isHave, isLockon, bgmStopCount, isClick] = prideChairController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false,
			bWatchPicture
		);

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = prideChairMessage;
			bFrontOfChair = true;
			frontOfChairIndex = prideChairIndex;
		}
		if (isClick)
		{
			auto chairState = getChairState(prideChairIndex);
			auto relicOnChair = getRelicOnChair(prideChairIndex);
			
			if (relicOnChair == ItemID::ItemIdMAX)
			{
				// 何も置かれていない
				message = prideChairMessage;
				priorityMessageCount = 0;
			}
			else if(!bClear && bRelicPlacedArray[relicOnChair] && (chairState == ChairState::Placed || chairState == ChairState::Collect))
			{
				// レリックが配置されている
				bRelicPlacedArray[relicOnChair] = false;

				// アイテムを取った
				items << relicOnChair;
				// 下の二つも対象のレリックに変更する
				bShowRelicArray[relicOnChair] = false;
				bRelicHaveArray[relicOnChair] = true;
				bgmStopCount = bgmStopCount;
				setChairState(prideChairIndex, ChairState::Empty, ItemID::ItemIdMAX);

				// 対象のレリックのメッセージにする
				priorityMessage = relicGetMessageArray[relicOnChair];
				priorityMessageCount = priorityMessageCountMax;
			}
		}
	}

	// 傲慢の絵
	if(!bLockon)
	{
		Vec3 temp = pridePicturePos;

		auto [isHave, isLockon, bgmStopCount, isClick] = pridePictureController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false,
			bWatchPicture
		);

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = 0;
		}
		if (isClick)
		{
			bWatchPicture = true;
			pictureIndex = 0;
			priorityMessageCount = 0;
			pictureOpenFrameCount = 1; // 1フレーム待つ
		}
	}

	// 強欲の椅子
	if(!bLockon)
	{
		Vec3 temp = greedChairPos;
		int greedChairIndex = static_cast<int>(GreedRelic);

		auto [isHave, isLockon, bgmStopCount, isClick] = greedChairController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false,
			bWatchPicture
		);

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = greedChairMessage;
			bFrontOfChair = true;
			frontOfChairIndex = greedChairIndex;
		}
		if (isClick)
		{
			auto chairState = getChairState(greedChairIndex);
			auto relicOnChair = getRelicOnChair(greedChairIndex);
			
			if (relicOnChair == ItemID::ItemIdMAX)
			{
				// 何も置かれていない
				message = greedChairMessage;
				priorityMessageCount = 0;
			}
			else if(!bClear && bRelicPlacedArray[relicOnChair] && (chairState == ChairState::Placed || chairState == ChairState::Collect))
			{
				// レリックが配置されている	
				bRelicPlacedArray[relicOnChair] = false;

				// アイテムを取った
				items << relicOnChair;
				// 下の二つも対象のレリックに変更する
				bShowRelicArray[relicOnChair] = false;
				bRelicHaveArray[relicOnChair] = true;
				bgmStopCount = bgmStopCount;
				setChairState(greedChairIndex, ChairState::Empty, ItemID::ItemIdMAX);

				// 対象のレリックのメッセージにする
				priorityMessage = relicGetMessageArray[relicOnChair];
				priorityMessageCount = priorityMessageCountMax;
			}
		}
	}

	// 強欲の絵
	if(!bLockon)
	{
		Vec3 temp = greedPicturePos;

		auto [isHave, isLockon, bgmStopCount, isClick] = greedPictureController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false,
			bWatchPicture
		);

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = 0;
		}
		if (isClick)
		{
			bWatchPicture = true;
			pictureIndex = 1;
			priorityMessageCount = 0;
			pictureOpenFrameCount = 1; // 1フレーム待つ
		}
	}

	// 嫉妬の椅子
	if(!bLockon)
	{
		Vec3 temp = envyChairPos;
		int envyChairIndex = static_cast<int>(EnvyRelic);

		auto [isHave, isLockon, bgmStopCount, isClick] = envyChairController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false,
			bWatchPicture
		);

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = envyChairMessage;
			bFrontOfChair = true;
			frontOfChairIndex = envyChairIndex;
		}
		if (isClick)
		{
			auto chairState = getChairState(envyChairIndex);
			auto relicOnChair = getRelicOnChair(envyChairIndex);

			if (relicOnChair == ItemID::ItemIdMAX)
			{
				// 何も置かれていない
				message = envyChairMessage;
				priorityMessageCount = 0;
			}
			else if(!bClear && bRelicPlacedArray[relicOnChair] && (chairState == ChairState::Placed || chairState == ChairState::Collect))
			{
				// レリックが配置されている	
				bRelicPlacedArray[relicOnChair] = false;

				// アイテムを取った
				items << relicOnChair;
				// 下の二つも対象のレリックに変更する
				bShowRelicArray[relicOnChair] = false;
				bRelicHaveArray[relicOnChair] = true;
				bgmStopCount = bgmStopCount;
				setChairState(envyChairIndex, ChairState::Empty, ItemID::ItemIdMAX);

				// 対象のレリックのメッセージにする
				priorityMessage = relicGetMessageArray[relicOnChair];
				priorityMessageCount = priorityMessageCountMax;
			}
		}
	}

	// 嫉妬の絵
	if(!bLockon)
	{
		Vec3 temp = envyPicturePos;

		auto [isHave, isLockon, bgmStopCount, isClick] = envyPictureController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false,
			bWatchPicture
		);

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = 0;
		}
		if (isClick)
		{
			bWatchPicture = true;
			pictureIndex = 2;
			priorityMessageCount = 0;
			pictureOpenFrameCount = 1; // 1フレーム待つ
		}
	}

	// 憤怒の椅子
	if(!bLockon)
	{
		Vec3 temp = wrathChairPos;
		int wrathChairIndex = static_cast<int>(WrathRelic);

		auto [isHave, isLockon, bgmStopCount, isClick] = wrathChairController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false,
			bWatchPicture
		);

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = wrathChairMessage;
			bFrontOfChair = true;
			frontOfChairIndex = wrathChairIndex;
		}
		if (isClick)
		{
			auto chairState = getChairState(wrathChairIndex);
			auto relicOnChair = getRelicOnChair(wrathChairIndex);

			if (relicOnChair == ItemID::ItemIdMAX)
			{
				// 何も置かれていない
				message = wrathChairMessage;
				priorityMessageCount = 0;
			}
			else if(!bClear && bRelicPlacedArray[relicOnChair] && (chairState == ChairState::Placed || chairState == ChairState::Collect))
			{
				// レリックが配置されている	
				bRelicPlacedArray[relicOnChair] = false;

				// アイテムを取った
				items << relicOnChair;
				// 下の二つも対象のレリックに変更する
				bShowRelicArray[relicOnChair] = false;
				bRelicHaveArray[relicOnChair] = true;
				bgmStopCount = bgmStopCount;
				setChairState(wrathChairIndex, ChairState::Empty, ItemID::ItemIdMAX);

				// 対象のレリックのメッセージにする
				priorityMessage = relicGetMessageArray[relicOnChair];
				priorityMessageCount = priorityMessageCountMax;
			}
		}
	}

	// 憤怒の絵
	if(!bLockon)
	{
		Vec3 temp = wrathPicturePos;

		auto [isHave, isLockon, bgmStopCount, isClick] = wrathPictureController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false,
			bWatchPicture
		);

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = 0;
		}
		if (isClick)
		{
			bWatchPicture = true;
			pictureIndex = 3;
			priorityMessageCount = 0;
			pictureOpenFrameCount = 1; // 1フレーム待つ
		}
	}

	// 色欲の椅子
	if(!bLockon)
	{
		Vec3 temp = lustChairPos;
		int lustChairIndex = static_cast<int>(LustRelic);

		auto [isHave, isLockon, bgmStopCount, isClick] = lustChairController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false,
			bWatchPicture
		);

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = lustChairMessage;
			bFrontOfChair = true;
			frontOfChairIndex = lustChairIndex;
		}
		if (isClick)
		{
			auto chairState = getChairState(lustChairIndex);
			auto relicOnChair = getRelicOnChair(lustChairIndex);

			if (relicOnChair == ItemID::ItemIdMAX)
			{
				// 何も置かれていない
				message = lustChairMessage;
				priorityMessageCount = 0;
			}
			else if(!bClear && bRelicPlacedArray[relicOnChair] && (chairState == ChairState::Placed || chairState == ChairState::Collect))
			{
				// レリックが配置されている	
				bRelicPlacedArray[relicOnChair] = false;

				// アイテムを取った
				items << relicOnChair;
				// 下の二つも対象のレリックに変更する
				bShowRelicArray[relicOnChair] = false;
				bRelicHaveArray[relicOnChair] = true;
				bgmStopCount = bgmStopCount;
				setChairState(lustChairIndex, ChairState::Empty, ItemID::ItemIdMAX);

				// 対象のレリックのメッセージにする
				priorityMessage = relicGetMessageArray[relicOnChair];
				priorityMessageCount = priorityMessageCountMax;
			}
		}
	}

	// 色欲の絵
	if(!bLockon)
	{
		Vec3 temp = lustPicturePos;

		auto [isHave, isLockon, bgmStopCount, isClick] = lustPictureController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false,
			bWatchPicture
		);

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = 0;
		}
		if (isClick)
		{
			bWatchPicture = true;
			pictureIndex = 4;
			priorityMessageCount = 0;
			pictureOpenFrameCount = 1; // 1フレーム待つ
		}
	}

	// 暴食の椅子
	if(!bLockon)
	{
		Vec3 temp = gluttonyChairPos;
		int gluttonyChairIndex = static_cast<int>(GluttonyRelic);

		auto [isHave, isLockon, bgmStopCount, isClick] = gluttonyChairController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false,
			bWatchPicture
		);

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = gluttonyChairMessage;
			bFrontOfChair = true;
			frontOfChairIndex = gluttonyChairIndex;
		}
		if (isClick)
		{
			auto chairState = getChairState(gluttonyChairIndex);
			auto relicOnChair = getRelicOnChair(gluttonyChairIndex);

			if (relicOnChair == ItemID::ItemIdMAX)
			{
				// 何も置かれていない
				message = gluttonyChairMessage;
				priorityMessageCount = 0;
			}
			else if(!bClear && bRelicPlacedArray[relicOnChair] && (chairState == ChairState::Placed || chairState == ChairState::Collect))
			{
				// レリックが配置されている	
				bRelicPlacedArray[relicOnChair] = false;

				// アイテムを取った
				items << relicOnChair;
				// 下の二つも対象のレリックに変更する
				bShowRelicArray[relicOnChair] = false;
				bRelicHaveArray[relicOnChair] = true;
				bgmStopCount = bgmStopCount;
				setChairState(gluttonyChairIndex, ChairState::Empty, ItemID::ItemIdMAX);

				// 対象のレリックのメッセージにする
				priorityMessage = relicGetMessageArray[relicOnChair];
				priorityMessageCount = priorityMessageCountMax;
			}
		}
	}

	// 暴食の絵
	if(!bLockon)
	{
		Vec3 temp = gluttonyPicturePos;

		auto [isHave, isLockon, bgmStopCount, isClick] = gluttonyPictureController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false,
			bWatchPicture
		);

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = 0;
		}
		if (isClick)
		{
			bWatchPicture = true;
			pictureIndex = 5;
			priorityMessageCount = 0;
			pictureOpenFrameCount = 1; // 1フレーム待つ
		}
	}

	// 怠惰の椅子
	if(!bLockon)
	{
		Vec3 temp = slothChairPos;
		int slothChairIndex = static_cast<int>(SlothRelic);

		auto [isHave, isLockon, bgmStopCount, isClick] = slothChairController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false,
			bWatchPicture
		);

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = slothChairMessage;
			bFrontOfChair = true;
			frontOfChairIndex = slothChairIndex;
		}
		if (isClick)
		{
			auto chairState = getChairState(slothChairIndex);
			auto relicOnChair = getRelicOnChair(slothChairIndex);

			if (relicOnChair == ItemID::ItemIdMAX)
			{
				// 何も置かれていない
				message = slothChairMessage;
				priorityMessageCount = 0;
			}
			else if(!bClear && bRelicPlacedArray[relicOnChair] && (chairState == ChairState::Placed || chairState == ChairState::Collect))
			{
				// レリックが配置されている	
				bRelicPlacedArray[relicOnChair] = false;

				// アイテムを取った
				items << relicOnChair;
				// 下の二つも対象のレリックに変更する
				bShowRelicArray[relicOnChair] = false;
				bRelicHaveArray[relicOnChair] = true;
				bgmStopCount = bgmStopCount;
				setChairState(slothChairIndex, ChairState::Empty, ItemID::ItemIdMAX);

				// 対象のレリックのメッセージにする
				priorityMessage = relicGetMessageArray[relicOnChair];
				priorityMessageCount = priorityMessageCountMax;
			}
		}
	}

	// 怠惰の絵
	if(!bLockon)
	{
		Vec3 temp = slothPicturePos;

		auto [isHave, isLockon, bgmStopCount, isClick] = slothPictureController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false,
			bWatchPicture
		);

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = 0;
		}
		if (isClick)
		{
			bWatchPicture = true;
			pictureIndex = 6;
			priorityMessageCount = 0;
			pictureOpenFrameCount = 1; // 1フレーム待つ
		}
	}
	
	// 出口の扉
	if (!bLockon)
	{
		Vec3 temp = exitDoorPos;

		auto [isHave, isLockon, bgmStopCount, isClick] = exitDoorController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false,
			false
		);
		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = exitDoorMessage;
		}
		if (isClick)
		{
			if (bClear){
				// クリア判定
				bPlayEpilogue = true;
				bStartPlaying = false;
				message = 3;
			}
			else
			{
				message = exitDoorMessage;
				priorityMessageCount = 0;
			}
		}
	}

	// 出口の扉のプレート
	if (!bLockon)
	{
		Vec3 temp = exitDoorPlatePos;

		auto [isHave, isLockon, bgmStopCount, isClick] = exitDoorPlateController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			-1,
			false,
			bWatchExitPlate
		);
		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = 0;
		}
		if (isClick)
		{
			priorityMessageCount = 0;
			bWatchExitPlate = true;
			pictureOpenFrameCount = 1; // 1フレーム待つ
		}
	}

	// 傲慢のレリック
	if (!bLockon)
	{
		Vec3 temp = prideRelicCurrentPos;
		temp.y += markHigh;
		int prideRelicIndex = static_cast<int>(PrideRelic);

		auto [isHave, isLockon, bgmStopCount, isClick] = prideRelicController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			0,
			true,
			false
		);

		if (isHave && !bRelicHaveArray[prideRelicIndex] && !bRelicPlacedArray[prideRelicIndex])
		{
			// アイテムを取った
			items << PrideRelic;
			bRelicHaveArray[prideRelicIndex] = isHave;
			bShowRelicArray[prideRelicIndex] = false;
			bgmStopCount = bgmStopCount;

			priorityMessage = relicGetMessageArray[prideRelicIndex];
			priorityMessageCount = priorityMessageCountMax;
		}

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = 0;
		}
	}

	// 強欲のレリック
	if (!bLockon)
	{
		Vec3 temp = greedRelicCurrentPos;
		temp.y += markHigh;
		int greedRelicIndex = static_cast<int>(GreedRelic);

		auto [isHave, isLockon, bgmStopCount, isClick] = greedRelicController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			0,
			true,
			false
		);

		if (isHave && !bRelicHaveArray[greedRelicIndex] && !bRelicPlacedArray[greedRelicIndex])
		{
			// アイテムを取った
			items << GreedRelic;
			bRelicHaveArray[greedRelicIndex] = isHave;
			bShowRelicArray[greedRelicIndex] = false;
			bgmStopCount = bgmStopCount;

			priorityMessage = relicGetMessageArray[greedRelicIndex];
			priorityMessageCount = priorityMessageCountMax;
		}

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = 0;
		}
	}

	// 嫉妬のレリック
	if (!bLockon)
	{
		Vec3 temp = envyRelicCurrentPos;
		temp.y += markHigh;
		int envyRelicIndex = static_cast<int>(EnvyRelic);

		auto [isHave, isLockon, bgmStopCount, isClick] = envyRelicController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			0,
			true,
			false
		);

		if (isHave && !bRelicHaveArray[envyRelicIndex] && !bRelicPlacedArray[envyRelicIndex])
		{
			// アイテムを取った
			items << EnvyRelic;
			bRelicHaveArray[envyRelicIndex] = isHave;
			bShowRelicArray[envyRelicIndex] = false;
			bgmStopCount = bgmStopCount;

			priorityMessage = relicGetMessageArray[envyRelicIndex];
			priorityMessageCount = priorityMessageCountMax;
		}

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = 0;
		}
	}

	// 憤怒のレリック
	if (!bLockon)
	{
		Vec3 temp = wrathRelicCurrentPos;
		temp.y += markHigh;
		int wrathRelicIndex = static_cast<int>(WrathRelic);

		auto [isHave, isLockon, bgmStopCount, isClick] = wrathRelicController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			0,
			true,
			false
		);

		if (isHave && !bRelicHaveArray[wrathRelicIndex] && !bRelicPlacedArray[wrathRelicIndex])
		{
			// アイテムを取った
			items << WrathRelic;
			bRelicHaveArray[wrathRelicIndex] = isHave;
			bShowRelicArray[wrathRelicIndex] = false;
			bgmStopCount = bgmStopCount;

			priorityMessage = relicGetMessageArray[wrathRelicIndex];
			priorityMessageCount = priorityMessageCountMax;
		}

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = 0;
		}
	}

	// 色欲のレリック
	if (!bLockon)
	{
		Vec3 temp = lustRelicCurrentPos;
		temp.y += markHigh;
		int lustRelicIndex = static_cast<int>(LustRelic);

		auto [isHave, isLockon, bgmStopCount, isClick] = lustRelicController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			0,
			true,
			false
		);

		if (isHave && !bRelicHaveArray[lustRelicIndex] && !bRelicPlacedArray[lustRelicIndex])
		{
			// アイテムを取った
			items << LustRelic;
			bRelicHaveArray[lustRelicIndex] = isHave;
			bShowRelicArray[lustRelicIndex] = false;
			bgmStopCount = bgmStopCount;

			priorityMessage = relicGetMessageArray[lustRelicIndex];
			priorityMessageCount = priorityMessageCountMax;
		}

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = 0;
		}
	}

	// 暴食のレリック
	if (!bLockon)
	{
		Vec3 temp = gluttonyRelicCurrentPos;
		temp.y += markHigh;
		int gluttonyRelicIndex = static_cast<int>(GluttonyRelic);

		auto [isHave, isLockon, bgmStopCount, isClick] = gluttonyRelicController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			0,
			true,
			false
		);

		if (isHave && !bRelicHaveArray[gluttonyRelicIndex] && !bRelicPlacedArray[gluttonyRelicIndex])
		{
			// アイテムを取った
			items << GluttonyRelic;
			bRelicHaveArray[gluttonyRelicIndex] = isHave;
			bShowRelicArray[gluttonyRelicIndex] = false;
			bgmStopCount = bgmStopCount;

			priorityMessage = relicGetMessageArray[gluttonyRelicIndex];
			priorityMessageCount = priorityMessageCountMax;
		}

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = 0;
		}
	}

	// 怠惰のレリック
	if (!bLockon)
	{
		Vec3 temp = slothRelicCurrentPos;
		temp.y += markHigh;
		int slothRelicIndex = static_cast<int>(SlothRelic);

		auto [isHave, isLockon, bgmStopCount, isClick] = slothRelicController.update(
			temp,
			camera,
			curCameraPosition,
			markPosition,
			0,
			true,
			false
		);

		if (isHave && !bRelicHaveArray[slothRelicIndex] && !bRelicPlacedArray[slothRelicIndex])
		{
			// アイテムを取った
			items << SlothRelic;
			bRelicHaveArray[slothRelicIndex] = isHave;
			bShowRelicArray[slothRelicIndex] = false;
			bgmStopCount = bgmStopCount;

			priorityMessage = relicGetMessageArray[slothRelicIndex];
			priorityMessageCount = priorityMessageCountMax;
		}

		if (isLockon)
		{
			// 見ている
			bLockon = isLockon;
			bookingMessage = 0;
		}
	}
}