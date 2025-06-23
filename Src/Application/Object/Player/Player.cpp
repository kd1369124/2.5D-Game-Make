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

	m_polygon.SetPivot(KdSquarePolygon::PivotType::Center_Bottom);

	m_pos = {0,0,0};

	m_speed = 0.3f;

	m_anime = 0.0f;

	m_gravity = 0;

	m_mWorld = Math::Matrix::Identity;

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	m_WalkkeyFlg = false;
	m_DashkeyFlg = false;
	m_AttackkeyFlg = false;
	m_AttackkeyFlg2 = false;
	m_AttackkeyFlg3 = false;
	m_JumpkeyFlg = false;
	m_DashkeyFlg = false;

	m_matelialType = NowMatelialType::Idle;
}

void Player::Update()
{
	m_dir = {0,0,0};

	NowMatelialType oldMatelialType = m_matelialType;

	MatelialType(m_WalkkeyFlg, m_DashkeyFlg, m_AttackkeyFlg, m_AttackkeyFlg2, m_AttackkeyFlg3, m_JumpkeyFlg);
	if(oldMatelialType != m_matelialType)
	{
		m_anime = 0.0f; //アニメーションをリセット
	}
	int WalkSlide[8] = { 0,1,2,3,4,5,6,7 };
	int IdleSlide[6] = { 0,1,2,3,4,5 };
	int DashSlide[8] = { 0,1,2,3,4,5,6,7 };
	if (NowMatelialType::Walk)m_polygon.SetUVRect(WalkSlide[(int)m_anime]);
	if (NowMatelialType::Idle)m_polygon.SetUVRect(IdleSlide[(int)m_anime]);
	if (NowMatelialType::Dash)m_polygon.SetUVRect(DashSlide[(int)m_anime]);
	
	if(!(GetAsyncKeyState('A')&0x8000) && !(GetAsyncKeyState('D')&0x8000))
	{
		m_WalkkeyFlg = false;
		
	}
	if ((GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState('D') & 0x8000) &&
		(GetAsyncKeyState(VK_SHIFT) & 0x8000))
	{
		m_DashkeyFlg = true;
		m_WalkkeyFlg = false;
	}
	else
	{
		m_DashkeyFlg = false;
	}

	
	m_anime += 0.1f;
	if (m_anime >= 5 )
	{
		m_anime = 0;
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_dir += {-1, 0, 0};
		m_WalkkeyFlg = true;
		m_direction = false; //左向き
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_dir += {1, 0, 0};
		m_WalkkeyFlg = true;
		m_direction = true; //右向き
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_gravity = -0.05f;
	}
	m_pos.y -= m_gravity;
	m_gravity += 0.005f;

	m_dir.Normalize();

	if (m_DashkeyFlg)
	{
		m_speed = 0.6f; //ダッシュ時の速度
	}
	else
	{
		m_speed = 0.3f; //通常時の速度
	}
	m_pos += m_dir * m_speed;

	int Rot = 0;
	if (!m_direction)
	{
		Rot = 180; //左向き
	}


	//拡縮行列
	Math::Matrix _mScale = Math::Matrix::CreateScale(7.0f);

	//回転行列
	Math::Matrix _mXRotation = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(0));
	Math::Matrix _mYRotation = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(Rot));
	Math::Matrix _mZRotation = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(0));

	//移動行列
	Math::Matrix _mTrans = Math::Matrix::CreateTranslation(m_pos);

	m_mWorld = _mScale * (_mXRotation * _mYRotation * _mZRotation) * _mTrans;



}

void Player::PostUpdate()
{


	//========================
//当たり判定はここ
//========================
//当たる側　
//========================
//レイ判定（全ての面に対して行うので重い処理）
//========================
//レイ判定用の変数
	KdCollider::RayInfo rayInfo;
	//レイの発射位置（座標）を設定
	rayInfo.m_pos = m_pos;

	//レイをちょっと上からの位置にする
	rayInfo.m_pos.y += 0.1f;

	//段差の許容範囲を設定
	float enableStepHigh = 0.2f;
	rayInfo.m_pos.y += enableStepHigh;

	//レイの発射方向を設定
	rayInfo.m_dir = { 0,-1,0 };//下方向
	//レイの長さを設定
	rayInfo.m_range = m_gravity + enableStepHigh;

	//当たり判定したいタイプを設定
	rayInfo.m_type = KdCollider::TypeGround;

	//デバッグ
	m_pDebugWire->AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range);

	//レイと当たり判定を行う
	//レイ判定は　貫通型なのでレイ当たったオブジェクト情報を格納するリスト作成
	std::list<KdCollider::CollisionResult> retRayList;

	//レイ判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(rayInfo, &retRayList);
	}

	//レイに当たったリストから一番近いオブジェクトを検知
	float maxOverLap = 0;	//はみ出た分を管理する変数
	Math::Vector3 hitPos;	//当たった座標
	bool hit = false;		//当たったフラグ
	for (auto& ret : retRayList)
	{
		//レイを遮断しオーバーした長さが一番長いものを探す
		if (maxOverLap < ret.m_overlapDistance)
		{
			//更新
			maxOverLap = ret.m_overlapDistance;
			hitPos = ret.m_hitPos;
			hit = true;
		}
	}

	if (hit)
	{
		//地面に当たってる
		m_pos = hitPos + Math::Vector3(0, -0.1f, 0);
		m_gravity = 0;
		m_JumpkeyFlg = false; // 地上にいたらジャンプフラグをリセット
	}

	//球判定---------------------------------------------
	//球判定用の変数を作成
	KdCollider::SphereInfo sphereInfo;
	//球の中心座標を設定
	sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 0.4f, 0);

	//球の半径を設定
	sphereInfo.m_sphere.Radius = 0.3f;

	//当たり判定したいタイプ設定
	sphereInfo.m_type = KdCollider::TypeGround;

	m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

	//球に当たったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retSphereList;

	//球と当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(sphereInfo, &retSphereList);
	}

	//球に当たったオブジェクトから一番近いオブジェクトを検出
	maxOverLap = 0;		//レイで使った変数を使い回し
	hit = false;		//レイで使った変数を使い回し
	Math::Vector3 hitDir;	//当たった方向
	for (auto& ret : retSphereList)
	{
		//球にめりこんだ長さが一番長いものを探す
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hitDir = ret.m_hitDir;		//ここがレイと違う
			hit = true;
		}
	}

	if (hit)
	{
		//当たってる
		//Z方向には押し返さないようにする
		hitDir.z = 0;
		//正規化（ベクトルの長さを１にする）
		hitDir.Normalize();
		//方向をベクトルで管理する時は長さ絶対「１」にする必要がある！！

		//押し戻し
		m_pos += hitDir * maxOverLap;
	}


	//球判定---------------------------------------------

}

void Player::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(m_polygon, m_mWorld);
}

void Player::DrawUnLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(m_polygon, m_mWorld);
}

void Player::MatelialType(bool walk, bool dash, bool atk1, bool atk2, bool atk3, bool jump)
{
	if (m_WalkkeyFlg)
	{

		m_polygon.SetMaterial(TexturePath::Walk);
		m_matelialType = NowMatelialType::Walk;
		m_polygon.SetSplit(SlideType::Walkslide, 1);

	}
	else if (m_DashkeyFlg)
	{
		
		m_polygon.SetMaterial(TexturePath::Run);
		m_matelialType = NowMatelialType::Dash;
		m_polygon.SetSplit(SlideType::Dashslide, 1);
	}
	else if (atk1)
	{
		m_polygon.SetMaterial(TexturePath::Attack1);
		m_matelialType = NowMatelialType::Atk1;
		m_polygon.SetSplit(SlideType::Atk1slide, 1);
		
	}
	else if (atk2)
	{
		m_polygon.SetMaterial(TexturePath::Attack2);
		m_matelialType = NowMatelialType::Atk2;
		m_polygon.SetSplit(SlideType::Atk2slide, 1);
	}
	else if (atk3)
	{
		m_polygon.SetMaterial(TexturePath::Attack3);
		m_matelialType = NowMatelialType::Atk3;
		m_polygon.SetSplit(SlideType::Atk3slide, 1);
	}
	else if (jump)
	{
		m_polygon.SetMaterial(TexturePath::Jump);
		m_matelialType = NowMatelialType::Jump;
		m_polygon.SetSplit(SlideType::Jumpslide, 1);
	}
	else
	{
		m_polygon.SetMaterial(TexturePath::Idle);
		m_matelialType = NowMatelialType::Idle;
		m_polygon.SetSplit(SlideType::Idleslide, 1);
	}
}


