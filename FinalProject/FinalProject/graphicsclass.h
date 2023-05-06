////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "inputclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "textureshaderclass.h"
#include "textureclass.h"
#include "Collision.h"
#include "textclass.h"
#include "bitmapclass.h"
#include "FireModelClass.h"
#include "FireShaderClass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:

	enum EStage {Title, Main, Clear, Over};
	
	EStage GetStage() { return m_stage; }
	void SetStage(EStage stage) { m_stage = stage; }

public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();


	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, float);
	void MovingCamera(int n);
	void MovingMario(int n);
	void CheckMarioJump();
	void CameraRotation(float, float, float);
	void CheckCollision();
	void CheckLife();
	void Timer(float);
	XMFLOAT3 GetCameraRotation();

	void OnOffLight(int);
	void AddTitleNum();
	void SetObjPolyCnt();
	void EnemyMovePlus();
	void ShellMovePlus();
	bool am_on;
	bool di_on;
	bool sp_on;
	bool is_top, is_bottom;
	bool is_jump;
	bool is_mario_grounded;
	bool isGoal;

	float forward_speed;
	float back_speed;
	float gravity;
	float saveY;

	XMFLOAT3 startPos;
	int titleNum;

	bool gameClear;
	bool gameOver;

	float m_Timer;
	int m_Life;

	int m_ScreenWidth;
	int m_ScreenHeight;

	int m_PolyCnt;
	int m_ObjCnt;

	float m_enemyMove;
	float dir;
	float shelldir;
	bool ischange;
	bool shellGround;
	float shellGravity;

	XMMATRIX baseViewMatrix;
private:
	bool Render();

private:
	D3DClass* m_D3D;
	InputClass* m_Input;
	CameraClass* m_Camera;

	TextClass* m_Text;
	BitmapClass* m_Title[3];
	BitmapClass* m_GameClear;
	BitmapClass* m_GameOver;
	BitmapClass* m_1up;

	//Fire
	FireModelClass* m_Fire;
	FireShaderClass* m_Fireshader;

	//mario
	ModelClass* m_Mario;
	Collision* m_Mario_Col;
	Collision* m_MarioFoot_Col;
	XMFLOAT3 m_Mario_Pos;
	XMFLOAT3 m_Mario_ColPos;
	int m_MarioPolyCnt;
	int m_MarioObjCnt;
	bool getMush;

	//MushRoom
	ModelClass* m_Mush;
	XMFLOAT3 m_MushPos;
	Collision* m_MushCol;
	bool m_MushEatten;
	bool Qboxhit;


	//pipe
	ModelClass* m_Pipe_s;
	Collision* m_Pipe_s_Col[3];

	ModelClass* m_Pipe_m;
	Collision* m_Pipe_m_Col;

	ModelClass* m_Pipe_l;
	Collision* m_Pipe_l_Col[2];
	int m_PipePolyCnt;
	int m_PipeObjCnt;

	//cube
	ModelClass* m_BrickCube;
	Collision* m_CubeCol[10];
	int CubeNum;
	ModelClass* m_Stair;
	Collision* m_SStairCol[16];
	ModelClass* m_Stair2;
	ModelClass* m_Stair3;
	Collision* m_LStairCol[8];
	ModelClass* m_QCube;
	Collision* m_QBoxHitCol;

	int m_BrickCubePolyCnt;
	int m_QCubePolyCnt;
	int m_StairPolyCnt;
	int m_BrickCubeObjCnt;
	int m_QCubeObjCnt;
	int m_StairObjCnt;
	
	//enemy
	ModelClass* m_Goomba;
	int m_GoombaNum;
	bool m_GoombaAlive[12];
	XMFLOAT3 m_GoombaPos[12];
	Collision* m_GoombaCol[12];
	int m_GoombaPolyCnt;
	int m_GoombaObjCnt;

	ModelClass* m_KoopaTroopa;
	bool m_KoopaTroopaAlive;
	XMFLOAT3 m_KoopaTroopaPos;
	Collision* m_KoopaTroopaCol;
	int m_KoopaTroopaPolyCnt;
	int m_KoopaTroopaObjCnt;

	ModelClass* m_Shell;
	Collision* m_ShellCol;
	bool m_shellHit;
	float shellMove;

	//castle
	ModelClass* m_Castle;
	int m_CastlePolyCnt;
	int m_CastleObjCnt;
	Collision* m_CastleCol;

	//flag
	ModelClass* m_Flag;
	ModelClass* m_Flagcube;
	Collision* m_FlagCol;
	int m_FlagPolyCnt;
	int m_FlagObjCnt;

	//ground
	ModelClass* m_Ground;
	Collision* m_Ground_Col[3];
	ModelClass* m_Ground_s;
	Collision* m_Ground_s_Col;
	int m_GroundPolyCnt;
	int m_GroundObjCnt;

	Collision* m_FallGroundCol;

	//skybox
	ModelClass* m_Skybox;

	LightClass* m_Light;
	LightShaderClass* m_LightShader;
	LightClass* m_PointLight1, * m_PointLight2, * m_PointLight3, * m_PointLight4;

	TextureClass* m_Texture;
	TextureShaderClass* m_TextureShader;

	EStage m_stage;
};

#endif