#pragma once
class ShotEnemy : public KdGameObject
{
public:
	ShotEnemy(){}
	~ShotEnemy(){}

	enum NowMatelialType
	{
		Idle,
		Walk,
		Dash,
		Atk1,
		Atk2,
		Atk3,
		Jump,
		Death,
		Damege,
		Null
	};

	void Init() override;

	void Update() override;

	void PostUpdate() override;

	void DrawLit() override;

	void GenerateDepthMapFromLight() override;

	void SetTarget(std::shared_ptr<KdGameObject> target)
	{
		m_target = target;
	}

	void SetPos(const Math::Vector3& pos) override
	{
		m_pos = pos;
	}

	void ChangeMatelialType();

	void SetExpired(bool flg)override;


	void onHit()override;

	void kopi() 
	{

		m_dir = {};

		NowMatelialType oldMatelialType = m_matelialType;

		//========================移動処理============================
		{
			// ==========================================
			// 追尾対象が一定範囲にいるか？
			// ==========================================
			Math::Vector3 targetPos = {};
			// weak_ptrで所持している m_spTarget が有効かどうかを調べる
			// expired() … shared_ptrとして無効なアドレスならtrueを返す
			if (m_hp > 0 && !DamageFlg)
			{
				if (m_target.expired() == false)
				{
					// lock() … weakをshared_ptrに変換
					targetPos = m_target.lock()->GetPos();
					// 対象との距離(ベクトルの長さ)で判定　※球判定
					Math::Vector3 v = targetPos - m_pos;
					// ベクトルの長さが一定値以下なら追尾モードへ
					if (v.Length() < m_searchArea)
					{
						m_chaseFlg = true;
						m_matelialType = NowMatelialType::Dash;
					}
					else
					{
						m_chaseFlg = false;

						IdleFlg = true;										// 追尾対象がいないのでアイドル状態にする
						m_matelialType = NowMatelialType::Idle;				// マテリアルをアイドルに変
					}
					//追尾フラグONの場合は追尾する
					if (m_chaseFlg)
					{
						// y軸を 0 にしておく　※空飛んで追いかけるの防止
						v.y = 0.0f;
						// 対象との距離で正規化前に攻撃か移動のみか射撃か判別する
						if (v.x < m_attackarea)
						{
							AtkFlg = true; // 攻撃フラグを立てる
						}

						// 対象へのベクトルの長さを 1 に
						v.Normalize();

						// 方向を向く
						if (v.x < 0.0f)
						{
							m_direction = false;				// 左向き
						}
						else
						{
							m_direction = true;					// 右向き
						}

						// 方向確定
						m_dir = v;
					}
					else
					{
						m_dir = {};
					}
				}
			}
		}
		//============================================================

		////アニメーションの更新作業
		m_animeInfo.count += m_animeInfo.speed;    //コマ数を加算

		int animeCnt = static_cast<int>(m_animeInfo.start + m_animeInfo.count);

		// アニメーションのコマ数が終了コマを超えたら
		if (animeCnt > m_animeInfo.end)
		{
			if (m_matelialType == NowMatelialType::Death)
			{
				m_isExpired = true;							// 死亡アニメーションが終わったらオブジェクトを削除
			}
			// アニメーションのコマ数を開始コマに戻す
			m_animeInfo.count = 0.0f;
		}
		if (!(m_matelialType == NowMatelialType::Damege) && !(m_matelialType == NowMatelialType::Death))
		{
			if (oldMatelialType != m_matelialType)
			{
				// マテリアルの種類が変わったらマテリアルを変更
				ChangeMatelialType();
			}
		}



		m_pos.y -= m_gravity;
		m_gravity += 0.05f;
		m_pos += m_dir * m_speed;

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
		// プレイヤー検知範囲判定
		{
			m_pDebugWire->AddDebugSphere
			(
				m_pos + Math::Vector3(0, 0.5f, 0),	// 球の中心位置
				m_searchArea,			// 球の半径
				kRedColor			// 球の色
			);
		}
		// プレイヤー検知範囲判定
		{
			m_pDebugWire->AddDebugSphere
			(
				m_pos + Math::Vector3(0, 0.5f, 0),	// 球の中心位置
				m_dashthArea,			// 球の半径
				kBlueColor			// 球の色
			);
		}
		// プレイヤー検知範囲判定
		{
			m_pDebugWire->AddDebugSphere
			(
				m_pos + Math::Vector3(0, 0.5f, 0),	// 球の中心位置
				m_attackarea,			// 球の半径
				kGreenColor			// 球の色
			);
		}
	}

private:

	std::shared_ptr<KdSquarePolygon> m_polygon;

	std::weak_ptr<KdGameObject> m_target;

	Math::Vector3 m_pos			= {};
	Math::Vector3 m_move		= {};
	Math::Vector3 m_dir			= {};

	float m_gravity				= 0.0f;
	float m_searchArea			= 0.0f;
	float m_dashthArea			= 0.0f;	
	float m_speed				= 0.0f;
	float m_attackarea			= 0.0f;
	float m_jumpPower			= 0.0f;
	float m_ShotArea			= 0.0f;	// 射撃範囲

	int m_hp					= 0;	// HP

	bool Left					= false;
	bool Right					= false;
	bool IdleFlg				= false;
	bool AtkFlg					= false;
	bool ShotFlg				= false;
	bool GuardFlg				= false;
	bool WalkFlg				= false;
	bool RunFlg					= false;
	bool JumpFlg				= false;
	bool KeyFlg					= false;
	bool m_direction			= false;
	bool m_chaseFlg				= false;

	bool m_shotFlg				= false;	// 射撃フラグ

	bool DamageFlg				= false;	// ダメージフラグ

	bool DamageEndFlg			= false;	// ダメージ終了フラグ

	bool expired				= false;	// 有効期限フラグ

	NowMatelialType		m_matelialType	= NowMatelialType::Idle;	// 現在のマテリアルタイプ

	AnimationInfo		m_animeInfo;
	float				m_animespeed	= 0.0f;	

	int FlashTimer = 0;         // 点滅用フレームカウンター
	bool Visible = true;        // 現在表示状態か

	bool FrashFlg = false;	// フラッシュフラグ
};
