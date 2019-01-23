//===============================================================================================================
//
// model.h
// author : Haruyuki Neko
//
//===============================================================================================================

/* インクルードガードの先頭 */
#ifndef _MODEL_H_
#define _MODEL_H_

//============================================================================
//
// マクロ定義
//
//============================================================================

// モデル
#define MODEL_FILE           ( "data/MODEL/pig.x" )			// Xファイル
#define MODEL_TEXTURE_FILE   ( "data/MODEL/wing.png" )
#define MODEL_MOVE_SPEED     ( 3.f )						// モデルの移動量
#define MODEL_SIZE           ( 50.0f )

// 行動
#define	MOVE_SPD_ACL      ( 0.4f )	// 移動加速度
#define	MOVE_SPD_ACL_AIR  ( 0.75f )	// 移動加速度（空中）
#define	MOVE_SPD_MAX	  ( 7.f )	// 移動最高速度
#define MOVE_SPD_MAX_AIR  ( 10.f )	// 移動最高速度（空中）
#define VALUE_JUMP        ( 15.f )	// ジャンプ量
#define VALUE_RESIST      ( 0.5f )	// 移動時の抵抗（空中）
#define VALUE_GRAVITY     ( 0.06f )	// 重力（G）

//============================================================================
//
// 構造体定義
//
//============================================================================

/* モデル用 構造体 */
typedef struct
{
	D3DXMATRIX mtxWorld;		// ワールドマトリックス

	LPDIRECT3DTEXTURE9  pD3DTextureModel;	// モデルのテクスチャ
	LPD3DXMESH          pMeshModel;			// モデルのメッシュ情報のポインタ
	LPD3DXBUFFER        pD3DXMatBuff;		// モデルのマテリアル情報のポインタ
	DWORD               nNumMat;			// マテリアル情報の総数

	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 oldpos;		// 前回の位置
	D3DXVECTOR3 rot;		// 向き
	D3DXVECTOR3 scl;		// 大きさ

	// 慣性
	float fAccelerationUp;		// 上
	float fAccelerationDown;	// 下
	float fAccelerationLeft;	// 左
	float fAccelerationRight;	// 右

	// ジャンプ
	float fValueResist;		// 移動時の抵抗を変数入れる
	float fValueJump;       // ジャンプ量を変数入れる
	float fValueGravity;   	// 重力（G）を変数入れる
	bool bJump;				// ジャンプフラグ

	// 向きたい角度
	float fDirection;		// 向きたい方向

	// 影
	int nShadowNumber;
	float fShadowSize;

	// ゴールしている
	bool bGoal;

}MODEL;

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

HRESULT InitModel(void);	// モデルの初期化
void UpdataModel(void);		// モデルの更新処理
void DrawModel(void);		// モデルの描画処理
void UnInitModel(void);		// モデルの開放処理

MODEL* GetModel(void);		// モデルの情報取得
D3DXMATRIX* CalcLookAtMatrix( D3DXMATRIX* pout, D3DXVECTOR3* pPos, D3DXVECTOR3* pLook, D3DXVECTOR3* pUp );

/* インクルードガードの最終行 */
#endif	// _MODEL_H_