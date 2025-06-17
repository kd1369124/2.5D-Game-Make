#include "Player.h"
#include"../../Scene/SceneManager.h"

void Player::Init()
{
	m_polygon.SetMaterial("Asset/Textures/Player_Walk.png");
	m_polygon.SetSplit(3,2);
	m_polygon.SetPivot(KdSquarePolygon::PivotType::Center_Bottom);

	m_pos = {0,0,25};

	m_speed = 0.5f;

	m_anime = 0.0f;

	m_gravity = 0;

	m_mWorld = Math::Matrix::Identity;

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Player::Update()
{
	m_dir = {0,0,0};

	int walk[5] = { 1,2,3,4,5 };
	m_polygon.SetUVRect(walk[(int)m_anime]);

	m_anime += 0.1f;
	if (m_anime >= 5 )
	{
		m_anime = 0;
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_dir += {-1, 0, 0};
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_dir += {1, 0, 0};
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_gravity = -0.05f;
	}

	m_pos.y -= m_gravity;
	m_gravity += 0.005f;

	m_dir.Normalize();

	m_pos += m_dir * m_speed;

	//拡縮行列
	Math::Matrix _mScale = Math::Matrix::CreateScale(1.0f);

	//回転行列
	Math::Matrix _mXRotation = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(0));
	Math::Matrix _mYRotation = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(0));
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
