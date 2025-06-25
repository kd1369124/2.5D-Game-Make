#include "Player.h"
#include"../../Scene/SceneManager.h"

namespace TexturePath
{
	const std::string Idle		= "Asset/Textures/Samurai/Idle.png";
	const std::string Walk		= "Asset/Textures/Samurai/Walk.png";
	const std::string Jump		= "Asset/Textures/Samurai/Jump.png";
	const std::string Run		= "Asset/Textures/Samurai/Run.png";
	const std::string Dead		= "Asset/Textures/Samurai/Dead.png";
	const std::string Attack1	= "Asset/Textures/Samurai/Attack_1.png";
	const std::string Attack2	= "Asset/Textures/Samurai/Attack_2.png";
	const std::string Attack3	= "Asset/Textures/Samurai/Attack_3.png";
	const std::string Damege	= "Asset/Textures/Samurai/Hurt.png";

}

void Player::Init()
{
	m_polygon.SetMaterial(TexturePath::Idle);
	m_polygon.SetScale(2);
	m_polygon.SetPivot(KdSquarePolygon::PivotType::Center_Bottom);



	m_speed = 0.3f;

	m_anime = 0.0f;

	m_gravity = 0.5f;

	m_mWorld = Math::Matrix::Identity;

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	m_WalkkeyFlg = false;
	m_DashkeyFlg = false;
	m_AttackkeyFlg = false;
	m_AttackkeyFlg2 = false;
	m_AttackkeyFlg3 = false;
	m_JumpkeyFlg = false;
	m_DashkeyFlg = false;
	m_direction = true; //右向き
	m_matelialType = NowMatelialType::Idle;

	m_animeInfo.start = 0.0f;	//開始コマを0.0fで初期化
	m_animeInfo.end = 0.0f;		//終了コマを0.0fで初期化
	m_animeInfo.count = 0.0f;	//現在のコマ数カウントを0.0fで初期化
	m_animeInfo.speed = 0.0f;	//アニメーション速度を0.0fで初期化
}

void Player::Update()
{
	m_dir = { 0,0,0 };
	NowMatelialType oldMatelialType = m_matelialType;

	if (oldMatelialType != m_matelialType)
	{
		m_anime = 0.0f; //アニメーションをリセット
	}



	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_dir += {-1, 0, 0};
		if (!m_JumpkeyFlg)
		{
			m_matelialType = NowMatelialType::Walk;
			m_WalkkeyFlg = true; //歩行フラグをオン
		}
		m_direction = false; //左向き
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_dir += {1, 0, 0};
		if (!m_JumpkeyFlg)
		{
			m_matelialType = NowMatelialType::Walk;
			m_WalkkeyFlg = true; //歩行フラグをオン
		}
		m_direction = true; //右向き
	}
	if (!(GetAsyncKeyState('A') & 0x8000) && !(GetAsyncKeyState('D') & 0x8000))
	{
		m_WalkkeyFlg = false; //両方押されたら歩行フラグをオフ
	}
	if ((GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState('D') & 0x8000) &&
		(GetAsyncKeyState(VK_SHIFT) & 0x8000))
	{
		m_DashkeyFlg = true;
		if (!m_JumpkeyFlg)
		{
			m_matelialType = NowMatelialType::Dash;
		}
	}
	else
	{
		m_DashkeyFlg = false;
	}
	if (!m_WalkkeyFlg && !m_DashkeyFlg && !m_JumpkeyFlg && !m_AttackkeyFlg && !m_AttackkeyFlg2 && !m_AttackkeyFlg3)
	{
		m_matelialType = NowMatelialType::Idle;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !m_JumpkeyFlg)
	{
		m_gravity = -0.1f;
		m_JumpkeyFlg = true; //ジャンプフラグをオン
		m_matelialType = NowMatelialType::Jump;
		m_animeInfo.count = 0.0f; //アニメーションのコマをリセット
	}

	MatelialType();

	m_pos.y -= m_gravity;
	m_gravity += 0.005f;

	m_dir.Normalize();

	if (m_DashkeyFlg)
	{
		m_speed = 0.8f; //ダッシュ時の速度
	}
	else
	{
		m_speed = 0.3f; //通常時の速度
	}

	m_pos += m_dir * m_speed;
	
	////アニメーションの更新作業
	m_animeInfo.count += m_animeInfo.speed;    //コマ数を加算

	int animeCnt = static_cast<int>(m_animeInfo.start + m_animeInfo.count);
	//最後のコマを表示したら戻してループさせる

	if (animeCnt > m_animeInfo.end)
	{
		animeCnt = m_animeInfo.start;        //最初のコマに戻す
		m_animeInfo.count = 0.0f;            //コマ数をリセット
	}


	//UV設定
	m_polygon.SetUVRect(animeCnt);
	
	// 行列関係
	{
		int Rot = 0;
		if (!m_direction)
		{
			Rot = 180; //左向き
		}
		//拡縮行列
		Math::Matrix _mScale = Math::Matrix::CreateScale(1.0f);

		//回転行列
		Math::Matrix _mXRotation = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(0));
		Math::Matrix _mYRotation = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(Rot));
		Math::Matrix _mZRotation = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(0));

		//移動行列
		Math::Matrix _mTrans = Math::Matrix::CreateTranslation(m_pos);

		m_mWorld = _mScale * (_mXRotation * _mYRotation * _mZRotation) * _mTrans;
	}
}

void Player::PostUpdate()
{
	//========================
//当たる側の処理
//========================

//========================
//レイ判定
//========================
//レイ判定用に変数を作成
	KdCollider::RayInfo rayInfo;
	//レイの発射位置(座標)を設定
	rayInfo.m_pos = m_pos;
	//ちょっと上からの位置にする
	rayInfo.m_pos.y -= 0.01f;
	//段差の許容範囲を設定
	float enableStepHigh = 0.2f;
	rayInfo.m_pos.y += enableStepHigh;
	//レイの発射方向
	rayInfo.m_dir = { 0,-1,0 }; //今回のみ下方向
	//レイの長さを設定
	rayInfo.m_range = m_gravity + enableStepHigh;
	//当たり判定をしたいタイプを設定
	rayInfo.m_type = KdCollider::TypeGround;

	//デバック
	m_pDebugWire->AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);

	//レイに当たったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retRayList;

	//レイと当たり判定をする!!
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(rayInfo, &retRayList);
	}

	//レイに一番近い
	float maxOverlap = 0.0f;
	Math::Vector3 hitPos;
	bool hit = false;

	for (auto& ret : retRayList)
	{
		//レイを遮断しオーバーした長さが一番長いものを探す
		if (maxOverlap < ret.m_overlapDistance)
		{
			//更新
			maxOverlap = ret.m_overlapDistance;
			hitPos = ret.m_hitPos;
			hit = true;
		}
	}
	if (hit)
	{
		//当たっていたら
		m_pos = hitPos + Math::Vector3(0, -0.1f, 0);
		m_gravity = 0.0f;
		m_JumpkeyFlg = false; //ジャンプフラグをオフ

	}
	//========================
	//球判定
	//========================
	//球判定用変数
	KdCollider::SphereInfo spherInfo;
	//球の中心座標
	spherInfo.m_sphere.Center = m_pos + Math::Vector3(0, 0.75f, 0);
	//球の半径
	spherInfo.m_sphere.Radius = 0.55f;
	//当たり判定したいタイプを設定
	spherInfo.m_type = KdCollider::TypeGround;

	//デバック
	m_pDebugWire->AddDebugSphere(spherInfo.m_sphere.Center, spherInfo.m_sphere.Radius);

	//球にあたったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retSphereList;

	//球と当たり判定をする!!
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(spherInfo, &retSphereList);
	}

	//球にあたったリストから一番近いオブジェクトを検出
	float MaxOverlap = 0.0f;
	Math::Vector3 hitSphereDir;

	for (auto& ret : retSphereList)
	{
		if (MaxOverlap < ret.m_overlapDistance)
		{
			MaxOverlap = ret.m_overlapDistance;
			hitSphereDir = ret.m_hitDir;
			hit = true;
		}
	}
	if (hit)
	{
		//Z方向への押し出し
		hitSphereDir.z = 0.0f;

		hitSphereDir.Normalize();

		m_pos += hitSphereDir * MaxOverlap;
	}
}

void Player::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(m_polygon, m_mWorld);
}

void Player::DrawUnLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(m_polygon, m_mWorld);
}

void Player::MatelialType()
{
	switch (m_matelialType)
	{
	case Idle:
		m_polygon.SetMaterial(TexturePath::Idle);
		m_polygon.SetSplit(SlideType::Idleslide, 1);
		break;
	case Walk:
		m_polygon.SetMaterial(TexturePath::Walk);
		m_polygon.SetSplit(SlideType::Walkslide, 1);
		break;
	case Dash:
		m_polygon.SetMaterial(TexturePath::Run);
		m_polygon.SetSplit(SlideType::Dashslide, 1);
		break;
	case Atk1:
		m_polygon.SetMaterial(TexturePath::Attack1);
		m_matelialType = NowMatelialType::Atk1;
		m_polygon.SetSplit(SlideType::Atk1slide, 1);
		break;
	case Atk2:
		m_polygon.SetMaterial(TexturePath::Attack2);
		m_matelialType = NowMatelialType::Atk2;
		m_polygon.SetSplit(SlideType::Atk2slide, 1);
		break;
	case Atk3:
		m_polygon.SetMaterial(TexturePath::Attack3);
		m_matelialType = NowMatelialType::Atk3;
		m_polygon.SetSplit(SlideType::Atk3slide, 1);
		break;
	case Jump:
		m_polygon.SetMaterial(TexturePath::Jump);
		m_polygon.SetSplit(SlideType::Jumpslide, 1);
		break;
	case Death:
		break;
	default:
		break;
	}
}


