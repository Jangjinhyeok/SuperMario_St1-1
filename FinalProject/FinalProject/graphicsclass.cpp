////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include "windows.h"


GraphicsClass::GraphicsClass()
{
	getMush = false;
	forward_speed = 0.0f;
	back_speed = 0.0f;
	gravity = 0.0f;
	saveY = 0.0f;

	m_Text = 0;
	for (int i = 0; i < 3; i++)
	{
		m_Title[i] = 0;
	}
	m_GameClear = 0;
	m_GameOver = 0;
	m_D3D = 0;
	m_Camera = 0;

	m_Fire = 0;
	m_Fireshader = 0;

	m_Mario_Pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Mario = 0;
	m_Mario_Col = 0;
	m_MarioFoot_Col = 0;
	m_Mario_ColPos = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_Mush = 0;
	m_MushPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_MushCol = 0;
	m_MushEatten = false;
	Qboxhit = false;

	m_Pipe_s = 0;
	for (int i = 0; i < 3; i++)
	{
		m_Pipe_s_Col[i] = 0;
	}

	m_Pipe_m = 0;
	m_Pipe_m_Col = 0;

	m_Pipe_l = 0;
	for (int i = 0; i < 2; i++)
	{
		m_Pipe_l_Col[i] = 0;
	}

	m_BrickCube = 0;
	for (int i = 0; i < 10; i++)
	{
		m_CubeCol[i] = 0;
	}

	m_Stair = 0;
	for (int i = 0; i < 16; i++)
	{
		m_SStairCol[i] = 0;
	}
	m_Stair2 = 0;
	m_Stair3 = 0;
	for (int i = 0; i < 8; i++)
	{
		m_LStairCol[i] = 0;
	}
	m_QCube = 0;
	m_QBoxHitCol = 0;
	m_Goomba = 0;
	m_GoombaNum = 12;
	for (int i = 0; i < m_GoombaNum; i++)
	{
		m_GoombaPos[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_GoombaAlive[i] = true;
		m_GoombaCol[i] = 0;
	}

	m_KoopaTroopa = 0;
	m_KoopaTroopaAlive = true;
	m_KoopaTroopaPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_KoopaTroopaCol = 0;

	m_Shell = 0;
	m_ShellCol = 0;
	m_shellHit = false;
	shellMove = 0;
	shelldir = 1.0f;
	ischange = false;
	shellGround = true;

	m_Castle = 0;
	m_Flag = 0;
	m_Flagcube = 0;
	m_FlagCol = 0;

	m_Ground = 0;
	for (int i = 0; i < 3; i++)
	{
		m_Ground_Col[i] = 0;
	}
	m_Ground_s = 0;
	m_Ground_s_Col = 0;

	m_Skybox = 0;

	m_LightShader = 0;
	m_Light = 0;
	m_PointLight1 = 0;
	m_PointLight2 = 0;
	m_PointLight3 = 0;
	m_PointLight4 = 0;

	m_TextureShader = 0;

	am_on = false;
	di_on = false;
	sp_on = false;

	is_top = false;
	is_bottom = true;

	is_jump = false;
	is_mario_grounded = false;

	m_stage = EStage::Title;
	titleNum = 0;

	gameClear = false;
	gameOver = false;

	m_Timer = 0;
	m_Life = 4;
	isGoal = false;

	m_PipePolyCnt = 0;
	m_BrickCubePolyCnt = 0;
	m_StairPolyCnt = 0;
	m_GoombaPolyCnt = 0;
	m_FlagPolyCnt = 0;
	m_GroundPolyCnt = 0;

	m_enemyMove = 0;
	dir = 1.0f;

}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;
	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}
	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -50.0f);
	m_Camera->Render();

	m_Camera->GetViewMatrix(baseViewMatrix);

	//fire
	m_Fire = new FireModelClass;
	if (!m_Fire) { return false; }


	result = m_Fire->Initialize(m_D3D->GetDevice(), "./data/square.txt", L"./data/fire01.dds", L"./data/noise01.dds", L"./data/alpha01.dds");
	if(!m_Fire)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Fireshader = new FireShaderClass;
	if (!m_Fireshader) { return false; }

	result = m_Fireshader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!m_Fireshader)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	//mario
	{
		m_Mario = new ModelClass;
		if (!m_Mario)
		{
			return false;
		}

		result = m_Mario->Initialize_Instance(m_D3D->GetDevice(), L"./data/mario.obj", L"./data/mario.dds", 1, "Mario");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_MarioPolyCnt = m_Mario->GetIndexCount();
		m_MarioObjCnt = m_Mario->GetInstanceCount();
		startPos = XMFLOAT3(3.0f, 1.0f, -130.0f);
   		m_Mario_Pos = startPos;
		//m_Mario_Pos = XMFLOAT3(7.0f, 10.0f, 295.0f);

		m_Mario_Col = new Collision;
		m_MarioFoot_Col = new Collision;
		if (!m_Mario_Col) { return false; }
		m_Mario_Col->InitAABB(m_Mario_ColPos, XMFLOAT3(0.7f, 0.7f, 0.7f));
		m_MarioFoot_Col->InitAABB(XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT3(0.7f, 0.35f, 0.7f));
	}

	//mushroom
	{
		m_Mush = new ModelClass;
		if (!m_Mush)
		{
			return false;
		}

		result = m_Mush->Initialize_Instance(m_D3D->GetDevice(), L"./data/Mushroom.obj", L"./data/Mushroom.dds", 1, "Mario");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_MushPos = XMFLOAT3(0.0f, 9.0f, -106.0f);
		m_MushCol = new Collision;
		if (!m_MushCol) { return false; }
		m_MushCol->InitAABB(m_MushPos, XMFLOAT3(1.0f, 1.0f, 1.0f));

	}

	//pipe_s
	{
		m_Pipe_s = new ModelClass;
		if (!m_Pipe_s)
		{
			return false;
		}

		result = m_Pipe_s->Initialize_Instance(m_D3D->GetDevice(), L"./data/Pipe.obj", L"./data/Pipe.dds", 3, "Pipe_s");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_PipeObjCnt = m_Pipe_s->GetInstanceCount();
		m_PipePolyCnt += m_Pipe_s->GetIndexCount() * m_Pipe_s->GetInstanceCount();

		for (int i = 0; i < 4; i++)
		{
			m_Pipe_s_Col[i] = new Collision;
			if (!m_Pipe_s_Col[i]) { return false; }
			if (i == 0)
				m_Pipe_s_Col[i]->InitAABB(XMFLOAT3(0.0f, 0.0f, -70.0f), XMFLOAT3(1.0f, 3.7f, 2.20f));
			else if (i == 1)
				m_Pipe_s_Col[i]->InitAABB(XMFLOAT3(0.0f, 0.0f, 215.0f), XMFLOAT3(1.0f, 3.7f, 2.20f));
			else if (i == 2)
				m_Pipe_s_Col[i]->InitAABB(XMFLOAT3(0.0f, 0.0f, 250.0f), XMFLOAT3(1.0f, 3.7f, 2.20f));
		}
	}

	//pipe_m
	{
		m_Pipe_m = new ModelClass;
		if (!m_Pipe_m)
		{
			return false;
		}

		result = m_Pipe_m->Initialize_Instance(m_D3D->GetDevice(), L"./data/Pipe.obj", L"./data/Pipe.dds", 1, "Pipe_m");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_PipePolyCnt += m_Pipe_m->GetIndexCount() * m_Pipe_m->GetInstanceCount();
		m_PipeObjCnt += m_Pipe_m->GetInstanceCount();


		m_Pipe_m_Col = new Collision;
		if (!m_Pipe_m_Col) { return false; }
		m_Pipe_m_Col->InitAABB(XMFLOAT3(0.0f, 0.0f, -60.0f), XMFLOAT3(1.0f, 4.5f, 2.25f));

	}

	//pipe_l
	{
		m_Pipe_l = new ModelClass;
		if (!m_Pipe_l)
		{
			return false;
		}

		result = m_Pipe_l->Initialize_Instance(m_D3D->GetDevice(), L"./data/Pipe.obj", L"./data/Pipe.dds", 2, "Pipe_l");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_PipeObjCnt += m_Pipe_l ->GetInstanceCount();
		m_PipePolyCnt += m_Pipe_l->GetIndexCount() * m_Pipe_l->GetInstanceCount();

		for (int i = 0; i < 2; i++)
		{
			m_Pipe_l_Col[i] = new Collision;
			if (!m_Pipe_l_Col[i]) { return false; }
			if (i == 0)
				m_Pipe_l_Col[i]->InitAABB(XMFLOAT3(0.0f, 0.0f, -49.5f), XMFLOAT3(1.0f, 7.0f, 2.0f));
			else if (i == 1)
				m_Pipe_l_Col[i]->InitAABB(XMFLOAT3(0.0f, 0.0f, -37.0f), XMFLOAT3(1.0f, 7.0f, 2.25f));
		}
	}

	//BrickCube
	{
		m_BrickCube = new ModelClass;
		if (!m_BrickCube)
		{
			return false;
		}

		result = m_BrickCube->Initialize_Instance(m_D3D->GetDevice(), L"./data/EmptyBlock.obj", L"./data/RedBrick.dds", 28, "BrickCube");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_BrickCubeObjCnt = m_BrickCube->GetInstanceCount();
		m_BrickCubePolyCnt = m_BrickCube->GetIndexCount() * m_BrickCube->GetInstanceCount();

	}

	//stair
	{
		m_Stair = new ModelClass;
		if (!m_Stair)
		{
			return false;
		}

		result = m_Stair->Initialize_Instance(m_D3D->GetDevice(), L"./data/EmptyBlock.obj", L"./data/EmptyBlock.dds", 40, "Stair");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_StairObjCnt = m_Stair->GetInstanceCount();
		m_StairPolyCnt += m_Stair->GetIndexCount() * m_Stair->GetInstanceCount();

		for (int i = 0; i < 16; i++)
		{
			m_SStairCol[i] = new Collision;
			if (!m_SStairCol[i]) { return false; }

			//stair1
			if (i == 0)
				m_SStairCol[i]->InitAABB(XMFLOAT3(0.0f, 0.0f, 142.0f), XMFLOAT3(7.0f, 2.1f, 1.8f));
			else if (i == 1)
				m_SStairCol[i]->InitAABB(XMFLOAT3(0.0f, 2.1f, 144.0f), XMFLOAT3(7.0f, 2.1f, 1.0f));
			else if (i == 2)
				m_SStairCol[i]->InitAABB(XMFLOAT3(0.0f, 4.2f, 146.0f), XMFLOAT3(7.0f, 2.1f, 1.0f));
			else if (i == 3)
				m_SStairCol[i]->InitAABB(XMFLOAT3(0.0f, 6.3f, 147.6f), XMFLOAT3(7.0f, 2.1f, 0.8f));

			//stair2
			else if (i == 4)
				m_SStairCol[i]->InitAABB(XMFLOAT3(0.0f, 6.3f, 153.0f), XMFLOAT3(7.0f, 2.1f, 0.05f));
			else if (i == 5)
				m_SStairCol[i]->InitAABB(XMFLOAT3(0.0f, 4.2f, 154.3f), XMFLOAT3(7.0f, 2.1f, 0.2f));
			else if (i == 6)
				m_SStairCol[i]->InitAABB(XMFLOAT3(0.0f, 2.1f, 157.3f), XMFLOAT3(7.0f, 2.1f, 0.2f));
			else if (i == 7)
				m_SStairCol[i]->InitAABB(XMFLOAT3(0.0f, 0.0f, 158.6f), XMFLOAT3(7.0f, 2.2f, 0.2f));

			//stair3
			else if (i == 8)
				m_SStairCol[i]->InitAABB(XMFLOAT3(0.0f, 0.0f, 174.0f), XMFLOAT3(7.0f, 2.2f, 0.2f));
			else if (i == 9)
				m_SStairCol[i]->InitAABB(XMFLOAT3(0.0f, 2.1f, 176.0f), XMFLOAT3(7.0f, 2.2f, 0.3f));
			else if (i == 10)
				m_SStairCol[i]->InitAABB(XMFLOAT3(0.0f, 4.2f, 177.6f), XMFLOAT3(7.0f, 2.2f, 0.3f));
			else if (i == 11)
				m_SStairCol[i]->InitAABB(XMFLOAT3(0.0f, 6.3f, 180.7f), XMFLOAT3(7.0f, 2.2f, 1.0f));

			//stair4
			else if (i == 12)
				m_SStairCol[i]->InitAABB(XMFLOAT3(0.0f, 6.3f, 195.6f), XMFLOAT3(7.0f, 2.2f, 0.3f));
			else if (i == 13)
				m_SStairCol[i]->InitAABB(XMFLOAT3(0.0f, 4.2f, 198.0f), XMFLOAT3(7.0f, 2.2f, 0.3f));
			else if (i == 14)
				m_SStairCol[i]->InitAABB(XMFLOAT3(0.0f, 2.1f, 200.0f), XMFLOAT3(7.0f, 2.2f, 0.3f));
			else if (i == 15)
				m_SStairCol[i]->InitAABB(XMFLOAT3(0.0f, 0.0f, 202.0f), XMFLOAT3(7.0f, 2.2f, 0.3f));
		}
	}

	//stair2
	{
		m_Stair2 = new ModelClass;
		if (!m_Stair2)
		{
			return false;
		}

		result = m_Stair2->Initialize_Instance(m_D3D->GetDevice(), L"./data/EmptyBlock.obj", L"./data/EmptyBlock.dds", 16, "Stair2");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_StairObjCnt += m_Stair2->GetInstanceCount();
		m_StairPolyCnt += m_Stair2->GetIndexCount() * m_Stair2->GetInstanceCount();
	}

	//stair3
	{
		m_Stair3 = new ModelClass;
		if (!m_Stair3)
		{
			return false;
		}

		result = m_Stair3->Initialize_Instance(m_D3D->GetDevice(), L"./data/EmptyBlock.obj", L"./data/EmptyBlock.dds", 135, "Stair3");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_StairObjCnt += m_Stair3->GetInstanceCount();
		m_StairPolyCnt += m_Stair3->GetIndexCount() * m_Stair3->GetInstanceCount();

		for (int i = 0; i < 8; i++)
		{
			m_LStairCol[i] = new Collision;
			if (!m_LStairCol[i]) { return false; }
			
			if(i == 0)
				m_LStairCol[i]->InitAABB(XMFLOAT3(0.0f, 0.0f, 253.0f), XMFLOAT3(7.0f, 2.1f, 0.7f));
			else if(i == 1)
				m_LStairCol[i]->InitAABB(XMFLOAT3(0.0f, 2.1f, 255.0f), XMFLOAT3(7.0f, 2.1f, 0.7f));
			else if(i == 2)
				m_LStairCol[i]->InitAABB(XMFLOAT3(0.0f, 4.2f, 257.0f), XMFLOAT3(7.0f, 2.1f, 0.7f));
			else if(i == 3)
				m_LStairCol[i]->InitAABB(XMFLOAT3(0.0f, 6.3f, 259.0f), XMFLOAT3(7.0f, 2.1f, 0.7f));
			else if(i == 4)
				m_LStairCol[i]->InitAABB(XMFLOAT3(0.0f, 8.4f, 261.0f), XMFLOAT3(7.0f, 2.1f, 0.6f));
			else if(i == 5)
				m_LStairCol[i]->InitAABB(XMFLOAT3(0.0f, 10.5f, 263.0f), XMFLOAT3(7.0f, 2.1f, 0.6f));
			else if(i == 6)
				m_LStairCol[i]->InitAABB(XMFLOAT3(0.0f, 12.6f, 265.0f), XMFLOAT3(7.0f, 2.1f, 0.6f));
			else if(i == 7)
				m_LStairCol[i]->InitAABB(XMFLOAT3(0.0f, 14.7f, 268.0f), XMFLOAT3(7.0f, 2.1f, 1.0f));
		}
	}

	//Qcube
	{
		m_QCube = new ModelClass;
		if (!m_QCube)
		{
			return false;
		}

		result = m_QCube->Initialize_Instance(m_D3D->GetDevice(), L"./data/cube.obj", L"./data/Qmark.dds", 13, "QCube");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_QCubeObjCnt = m_QCube->GetInstanceCount();
		m_QCubePolyCnt = m_QCube->GetIndexCount() * m_QCube->GetInstanceCount();

		m_QBoxHitCol = new Collision;
		m_QBoxHitCol->InitAABB(XMFLOAT3(0.0f, 3.0f, -107.0f), XMFLOAT3(2.0f, 1.0f, 2.0f));
		for (int i = 0; i < 10; i++)
		{
			m_CubeCol[i] = new Collision;
			if (!m_CubeCol[i]) { return false; }

			if (i == 0)
				m_CubeCol[i]->InitAABB(XMFLOAT3(0.0f, 3.0f, -107.0f), XMFLOAT3(2.0f, 6.0f, 2.0f));
			else if(i == 1)
				m_CubeCol[i]->InitAABB(XMFLOAT3(0.0f, 3.0f, -90.0f), XMFLOAT3(2.0f, 6.0f, 8.0f));			
			else if(i == 2)
				m_CubeCol[i]->InitAABB(XMFLOAT3(0.0f, 3.0f, -27.0f), XMFLOAT3(2.0f, 6.0f, 2.0f));
			else if(i == 3)
				m_CubeCol[i]->InitAABB(XMFLOAT3(0.0f, 3.0f, 9.5f), XMFLOAT3(2.0f, 6.0f, 6.0f));
			else if(i == 4)
				m_CubeCol[i]->InitAABB(XMFLOAT3(0.0f, 12.0f, 25.0f), XMFLOAT3(2.0f, 6.0f, 9.0f));
			else if(i == 5)
				m_CubeCol[i]->InitAABB(XMFLOAT3(0.0f, 3.0f, 50.0f), XMFLOAT3(2.0f, 6.0f, 2.0f));
			else if(i == 6)
				m_CubeCol[i]->InitAABB(XMFLOAT3(0.0f, 12.0f, 45.0f), XMFLOAT3(2.0f, 6.0f, 6.0f));
			else if(i == 7)
				m_CubeCol[i]->InitAABB(XMFLOAT3(0.0f, 3.0f, 61.0f), XMFLOAT3(2.0f, 6.0f, 2.0f));
			else if(i == 8)
				m_CubeCol[i]->InitAABB(XMFLOAT3(0.0f, 3.0f, 102.0f), XMFLOAT3(2.0f, 6.0f, 2.0f));
			else if(i == 9)
				m_CubeCol[i]->InitAABB(XMFLOAT3(0.0f, 3.0f, 226.0f), XMFLOAT3(2.0f, 6.0f, 7.5f));


		}
	}

	//koopatroopa
	{
		m_KoopaTroopa = new ModelClass;
		if (!m_KoopaTroopa)
		{
			return false;
		}

		result = m_KoopaTroopa->Initialize_Instance(m_D3D->GetDevice(), L"./data/KoopaTroopa.obj", L"./data/KoopaTroopa.dds",1,"KoopaTroopa");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_KoopaTroopaObjCnt = m_KoopaTroopa->GetInstanceCount();
		m_KoopaTroopaPolyCnt = m_KoopaTroopa->GetIndexCount();

		m_KoopaTroopaPos = XMFLOAT3(0.0f, -1.5f, 90.0f);
		m_KoopaTroopaCol = new Collision;
		if (!m_KoopaTroopaCol) { return false; }
		m_KoopaTroopaCol->InitAABB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.7f, 2.0f, 0.7f));
	}

	//shell
	{
		m_Shell = new ModelClass;
		if (!m_Shell)
		{
			return false;
		}

		result = m_Shell->Initialize_Instance(m_D3D->GetDevice(), L"./data/shell.obj", L"./data/shell.dds", 1, "KoopaTroopa");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_ShellCol = new Collision;
		m_ShellCol->InitAABB(m_KoopaTroopaPos, XMFLOAT3(0.7f, 2.0f, 0.7f));
	}

	//goomba
	{
		m_Goomba = new ModelClass;
		if (!m_Goomba)
		{
			return false;
		}

		result = m_Goomba->Initialize_Instance(m_D3D->GetDevice(), L"./data/goomba.obj", L"./data/goomba.dds",1,"Goomba");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_GoombaObjCnt = m_GoombaNum;
		m_GoombaPolyCnt += m_Goomba->GetIndexCount() * m_GoombaNum;
		for (int i = 0; i < m_GoombaNum; i++)
		{
			m_GoombaCol[i] = new Collision;
			if (!m_GoombaCol[i]) { return false; }
			if (i == 0)
			{
				m_GoombaPos[i] = XMFLOAT3(0.0f, 0.0f, -120.0f);
				m_GoombaCol[i]->InitAABB(m_GoombaPos[i], XMFLOAT3(0.7f, 1.0f, 0.7f));
			}

			else if (i == 1)
			{
				m_GoombaPos[i] = XMFLOAT3(0.0f, 0.0f, -55.0f);
				m_GoombaCol[i]->InitAABB(m_GoombaPos[i], XMFLOAT3(0.7f, 1.0f, 0.7f));
			}

			else if (i == 2)
			{
				m_GoombaPos[i] = XMFLOAT3(0.0f, 0.0f, -45.0f);
				m_GoombaCol[i]->InitAABB(m_GoombaPos[i], XMFLOAT3(0.7f, 1.0f, 0.7f));
			}
			
			else if (i == 3)
			{
				m_GoombaPos[i] = XMFLOAT3(0.0f, 0.0f, -40.0f);
				m_GoombaCol[i]->InitAABB(m_GoombaPos[i], XMFLOAT3(0.7f, 1.0f, 0.7f));
			}
			
			else if (i == 4)
			{
				m_GoombaPos[i] = XMFLOAT3(0.0f, 0.0f, 50.0f);
				m_GoombaCol[i]->InitAABB(m_GoombaPos[i], XMFLOAT3(0.7f, 1.0f, 0.7f));
			}
			
			else if (i == 5)
			{
				m_GoombaPos[i] = XMFLOAT3(0.0f, 0.0f, 55.0f);
				m_GoombaCol[i]->InitAABB(m_GoombaPos[i], XMFLOAT3(0.7f, 1.0f, 0.7f));
			}
			
			else if (i == 6)
			{
				m_GoombaPos[i] = XMFLOAT3(0.0f, 0.0f, 105.0f);
				m_GoombaCol[i]->InitAABB(m_GoombaPos[i], XMFLOAT3(0.7f, 1.0f, 0.7f));
			}
			
			else if (i == 7)
			{
				m_GoombaPos[i] = XMFLOAT3(0.0f, 0.0f, 110.0f);
				m_GoombaCol[i]->InitAABB(m_GoombaPos[i], XMFLOAT3(0.7f, 1.0f, 0.7f));
			}
			
			else if (i == 8)
			{
				m_GoombaPos[i] = XMFLOAT3(0.0f, 0.0f, 115.0f);
				m_GoombaCol[i]->InitAABB(m_GoombaPos[i], XMFLOAT3(0.7f, 1.0f, 0.7f));
			}
			
			else if (i == 9)
			{
				m_GoombaPos[i] = XMFLOAT3(0.0f, 0.0f, 120.0f);
				m_GoombaCol[i]->InitAABB(m_GoombaPos[i], XMFLOAT3(0.7f, 1.0f, 0.7f));
			}
			
			else if (i == 10)
			{
				m_GoombaPos[i] = XMFLOAT3(0.0f, 0.0f, 220.0f);
				m_GoombaCol[i]->InitAABB(m_GoombaPos[i], XMFLOAT3(0.7f, 1.0f, 0.7f));
			}
			
			else if (i == 11)
			{
				m_GoombaPos[i] = XMFLOAT3(0.0f, 0.0f, 225.0f);
				m_GoombaCol[i]->InitAABB(m_GoombaPos[i], XMFLOAT3(0.7f, 1.0f, 0.7f));
			}
		}
	}

	//castle
	{
		m_Castle = new ModelClass;
		if (!m_Castle)
		{
			return false;
		}

		result = m_Castle->Initialize_Instance(m_D3D->GetDevice(), L"./data/Castle.obj", L"./data/Castle.dds", 1, "Castle");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_CastlePolyCnt = m_Castle->GetIndexCount() * m_Castle->GetInstanceCount();
		m_CastleObjCnt = m_Castle->GetInstanceCount();

		m_CastleCol = new Collision;
		if (!m_CastleCol) { return false; }
		m_CastleCol->InitAABB(XMFLOAT3(0.0f, 0.0f, 310.0f), XMFLOAT3(10.0f, 1.0f, 1.0f));
	}

	//flag
	{
		m_Flag = new ModelClass;
		if (!m_Flag)
		{
			return false;
		}

		result = m_Flag->Initialize_Instance(m_D3D->GetDevice(), L"./data/flag.obj", L"./data/flag.dds", 1, "Flag");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_FlagObjCnt = m_Flag->GetInstanceCount();
		m_FlagPolyCnt += m_Flag->GetIndexCount();
		m_FlagCol = new Collision;
		if (!m_FlagCol) { return false; }
		m_FlagCol->InitAABB(XMFLOAT3(0.0f, 0.0f, 295.0f), XMFLOAT3(2.0f, 10.0f, 2.0f));
	}

	//flagcube
	{
		m_Flagcube = new ModelClass;
		if (!m_Flagcube)
		{
			return false;
		}
		result = m_Flagcube->Initialize_Instance(m_D3D->GetDevice(), L"./data/EmptyBlock.obj", L"./data/EmptyBlock.dds", 1, "Flagcube");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_FlagObjCnt += m_Flagcube->GetInstanceCount();
		m_FlagPolyCnt += m_Flagcube->GetIndexCount();
	}

	//ground
	{
		m_FallGroundCol = new Collision;
		if (!m_FallGroundCol) { return false; }
		m_FallGroundCol->InitAABB(XMFLOAT3(0.0f, -25.0f, 0.0f), XMFLOAT3(100.0f, 2.0f, 400.0f));

		m_Ground = new ModelClass;
		if (!m_Ground)
		{
			return false;
		}

		result = m_Ground->Initialize_Instance(m_D3D->GetDevice(), L"./data/cube.obj", L"./data/Ground.dds", 3, "Ground");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_GroundObjCnt = m_Ground->GetInstanceCount();
		m_GroundPolyCnt += m_Ground->GetIndexCount() * m_Ground->GetInstanceCount();
		for (int i = 0; i < 3; i++)
		{
			m_Ground_Col[i] = new Collision;
			if (!m_Ground_Col[i]) { return false; }
			if (i == 0)
				m_Ground_Col[i]->InitAABB(XMFLOAT3(0.0f, -2.0f, -90.0f), XMFLOAT3(10.0f, 2.2f, 75.0f));
			else if (i == 1)
				m_Ground_Col[i]->InitAABB(XMFLOAT3(0.0f, -2.0f, 105.0f), XMFLOAT3(10.0f, 2.2f, 70.0f));
			else if (i == 2)
				m_Ground_Col[i]->InitAABB(XMFLOAT3(0.0f, -2.0f, 260.0f), XMFLOAT3(10.0f, 2.2f, 65.0f));
		}
	}

	//ground_s
	{
		m_Ground_s = new ModelClass;
		if (!m_Ground_s)
		{
			return false;
		}
		result = m_Ground_s->Initialize_Instance(m_D3D->GetDevice(), L"./data/cube.obj", L"./data/Ground.dds", 1, "Ground_s");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_GroundObjCnt += m_Ground_s->GetInstanceCount();
		m_GroundPolyCnt += m_Ground_s->GetIndexCount();
		m_Ground_s_Col = new Collision;
		if (!m_Ground_s_Col) { return false; }
		m_Ground_s_Col->InitAABB(XMFLOAT3(0.0f, -2.0f, 7.0f), XMFLOAT3(10.0f, 2.2f, 19.0f));
	}

	//skybox
	{
		m_Skybox = new ModelClass;
		if (!m_Skybox)
		{
			return false;
		}

		result = m_Skybox->Initialize_Instance(m_D3D->GetDevice(), L"./data/skybox.obj", L"./data/skybox.dds", 1, "Skybox");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
	}

	// Create the light shader object.
 	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	// 색을 다르게 주고 싶을 때는 diffuse이용
	m_Light->SetDirection(0.0f, -2.0f, 1.0f);
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(128.0f);

	m_PointLight1 = new LightClass;
	if (!m_PointLight1)
	{
		return false;
	}

	m_PointLight1->SetDiffuseColor(1.0f, 0.0f, 0.0f, 0.0f);
	m_PointLight1->SetPosition(-3.0f, 1.0f, 3.0f);

	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	for (int i = 0; i < 3; ++i)
	{
		m_Title[i] = new BitmapClass;
		if (!m_Title[i]) return false;
	}

	// Initialize the bitmap object.
	result = m_Title[0]->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/Title1.dds", 800, 600);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}
	result = m_Title[1]->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/Title2.dds", 800, 600);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}
	result = m_Title[2]->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/Title3.dds", 800, 600);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_GameClear = new BitmapClass;
	if (!m_GameClear) return false;

	result = m_GameClear->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/GameClear.dds", 800, 600);
	if (!result) return false;

	m_GameOver = new BitmapClass;
	if (!m_GameOver) return false;

	result = m_GameOver->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/GameOver.dds", 800, 600);
	if (!result) return false;
	
	m_1up = new BitmapClass;
	if (!m_1up) return false;

	result = m_1up->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/Life.dds", 35, 35);
	if (!result) return false;

	

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the model object.
	if (m_Fire)
	{
		m_Fire->Shutdown();
		delete m_Fire;
		m_Fire = 0;
	}

	if (m_Fireshader)
	{
		m_Fireshader->Shutdown();
		delete m_Fireshader;
		m_Fireshader = 0;
	}

	if (m_Mario)
	{
		m_Mario->Shutdown();
		delete m_Mario;
		m_Mario = 0;
	}

	if (m_Mario_Col)
	{
		delete m_Mario_Col;
		m_Mario_Col = 0;
	}
	
	if (m_Pipe_s)
	{
		m_Pipe_s->Shutdown();
		delete m_Pipe_s;
		m_Pipe_s = 0;
	}

	if (m_Pipe_m)
	{
		m_Pipe_m->Shutdown();
		delete m_Pipe_m;
		m_Pipe_m = 0;
	}

	if (m_Pipe_l)
	{
		m_Pipe_l->Shutdown();
		delete m_Pipe_l;
		m_Pipe_l = 0;
	}

	if (m_BrickCube)
	{
		m_BrickCube->Shutdown();
		delete m_BrickCube;
		m_BrickCube = 0;
	}

	if (m_Stair)
	{
		m_Stair->Shutdown();
		delete m_Stair;
		m_Stair = 0;
	}
	
	if (m_Stair2)
	{
		m_Stair2->Shutdown();
		delete m_Stair2;
		m_Stair2 = 0;
	}
	
	if (m_Stair3)
	{
		m_Stair3->Shutdown();
		delete m_Stair3;
		m_Stair3 = 0;
	}

	if (m_QCube)
	{
		m_QCube->Shutdown();
		delete m_QCube;
		m_QCube = 0;
	}

	if (m_Goomba)
	{
		m_Goomba->Shutdown();
		delete m_Goomba;
		m_Goomba = 0;
	}

	if (m_KoopaTroopa)
	{
		m_KoopaTroopa->Shutdown();
		delete m_KoopaTroopa;
		m_KoopaTroopa = 0;
	}

	if (m_Castle)
	{
		m_Castle->Shutdown();
		delete m_Castle;
		m_Castle = 0;
	}
	
	if (m_Flag)
	{
		m_Flag->Shutdown();
		delete m_Flag;
		m_Flag = 0;
	}

	if (m_Ground)
	{
		m_Ground->Shutdown();
		delete m_Ground;
		m_Ground = 0;
	}

	if (m_Ground_s)
	{
		m_Ground_s->Shutdown();
		delete m_Ground_s;
		m_Ground_s = 0;
	}
	
	if (m_Skybox)
	{
		m_Skybox->Shutdown();
		delete m_Skybox;
		m_Skybox = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the bitmap object.
	for (int i = 0; i < 3; ++i)
	{
		if (m_Title[i])
		{
			m_Title[i]->Shutdown();
			delete m_Title[i];
			m_Title[i] = 0;
		}
	}

	if (m_GameClear)
	{
		m_GameClear->Shutdown();
		delete m_GameClear;
		m_GameClear = 0;
	}

	if (m_GameOver)
	{
		m_GameOver->Shutdown();
		delete m_GameOver;
		m_GameOver = 0;
	}
	
	if (m_1up)
	{
		m_1up->Shutdown();
		delete m_1up;
		m_1up = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	
	return;
}

bool GraphicsClass::Frame(int fps, int cpu, float frametime)
{
	bool result;
	forward_speed = 0.2f;

	switch (m_stage)
	{
	case GraphicsClass::Title:
	case GraphicsClass::Clear:
	case GraphicsClass::Over:
		break;
	case GraphicsClass::Main:
		Timer(frametime);
		CheckLife();
		CheckMarioJump();
		CheckCollision();
		SetObjPolyCnt();
		EnemyMovePlus();
		ShellMovePlus();
		// Set the frames per second.
		result = m_Text->SetFPS(fps, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		// Set the cpu usage.
		result = m_Text->SetCPU(cpu, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		//set the screen size
		result = m_Text->SetScreenSize(m_ScreenWidth, m_ScreenHeight, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		result = m_Text->SetPolyCount(m_PolyCnt, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		result = m_Text->SetObjCount(m_ObjCnt, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		result = m_Text->SetHP(m_Life, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		result = m_Text->SetTimer(m_Timer, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}
		break;

	default:
		break;
	}

	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render()
{

	static float frameTime = 0.0f;


	// 프레임 시간 카운터를 증가시킵니다.
	frameTime += 0.01f;
	if (frameTime > 1000.0f)
	{
		frameTime = 0.0f;
	}

	// 세 가지 다른 노이즈 텍스처에 대해 세 가지 스크롤 속도를 설정합니다.
	XMFLOAT3 scrollSpeeds = XMFLOAT3(1.3f, 2.1f, 2.3f);

	// 세 개의 서로 다른 노이즈 옥타브 텍스처를 만드는 데 사용할 세 개의 스케일을 설정합니다.
	XMFLOAT3 scales = XMFLOAT3(1.0f, 2.0f, 3.0f);

	// 세 가지 다른 노이즈 텍스처에 대해 세 가지 다른 x 및 y 왜곡 인수를 설정합니다.
	XMFLOAT2 distortion1 = XMFLOAT2(0.1f, 0.2f);
	XMFLOAT2 distortion2 = XMFLOAT2(0.1f, 0.5f);
	XMFLOAT2 distortion3 = XMFLOAT2(0.1f, 0.1f);

	// 텍스처 좌표 샘플링 섭동의 스케일과 바이어스.
	float distortionScale = 0.8f;
	float distortionBias = 0.5f;

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	XMMATRIX matTrans, matRot, matScale, matRot2;
	bool result;
	int mouseX, mouseY;
	float enemySpeed = 0.0f;
	bool temp = false;
	bool temp2 = false;

	
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	switch (m_stage)
	{
	case EStage::Title:
	{
		m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
		// Generate the view matrix based on the camera's position.
		m_Camera->Render();

		// Get the world, view, and projection matrices from the camera and d3d objects.
		m_Camera->GetViewMatrix(viewMatrix);
		m_D3D->GetWorldMatrix(worldMatrix);
		m_D3D->GetProjectionMatrix(projectionMatrix);
		m_D3D->GetOrthoMatrix(orthoMatrix);

		// Turn off the Z buffer to begin all 2D rendering.
		m_D3D->TurnZBufferOff();

		// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		result = m_Title[titleNum]->Render(m_D3D->GetDeviceContext(), 0, 0);
		if (!result)
		{
			return false;
		}

		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Title[titleNum]->GetIndexCount(), 1, worldMatrix, viewMatrix, orthoMatrix, m_Title[titleNum]->GetTexture());
		if (!result)
		{
			return false;
		}

		// Turn the Z buffer back on now that all 2D rendering has completed.
		m_D3D->TurnZBufferOn();
		break;
	}
	case EStage::Main:
	{

		m_Camera->SetPosition(m_Mario_Pos.x + 4, m_Mario_Pos.y + 5, m_Mario_Pos.z - 10);
		m_Camera->Render();

		m_Camera->GetViewMatrix(viewMatrix);
		m_D3D->GetWorldMatrix(worldMatrix);
		m_D3D->GetProjectionMatrix(projectionMatrix);
		m_D3D->GetOrthoMatrix(orthoMatrix);

		// Turn the Z buffer back on now that all 2D rendering has completed.
		m_D3D->TurnZBufferOn();

		if (isGoal)
		{
			m_D3D->TurnOnAlphaBlending();

			// 정사각형 모델의 정점과 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
			m_Fire->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(2.0f, 15.0f, 3.0f);
			matTrans = XMMatrixTranslation(6.0f, 0.0f, 305.0f);
			worldMatrix = matScale * matTrans;

			// 화재 쉐이더를 사용하여 사각형 모델을 렌더링합니다.
			if (!m_Fireshader->Render(m_D3D->GetDeviceContext(), m_Fire->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Fire->GetTexture1(), m_Fire->GetTexture2(), m_Fire->GetTexture3(), frameTime, scrollSpeeds,
				scales, distortion1, distortion2, distortion3, distortionScale, distortionBias))
			{
				return false;
			}

			// 정사각형 모델의 정점과 인덱스 버퍼를 그래픽 파이프 라인에 배치하여 그리기를 준비합니다.
			m_Fire->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(2.0f, 15.0f, 3.0f);
			matTrans = XMMatrixTranslation(-6.0f, 0.0f, 305.0f);
			worldMatrix = matScale * matTrans;

			// 화재 쉐이더를 사용하여 사각형 모델을 렌더링합니다.
			if (!m_Fireshader->Render(m_D3D->GetDeviceContext(), m_Fire->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Fire->GetTexture1(), m_Fire->GetTexture2(), m_Fire->GetTexture3(), frameTime, scrollSpeeds,
				scales, distortion1, distortion2, distortion3, distortionScale, distortionBias))
			{
				return false;
			}
			m_D3D->TurnOffAlphaBlending();
		}

		//mario
		{
			m_Mario->Render(m_D3D->GetDeviceContext());
			m_Mario_Pos = XMFLOAT3(m_Mario_Pos.x, m_Mario_Pos.y + gravity, m_Mario_Pos.z);
			//mario body collision pos
			if (!getMush)
			{
				matScale = XMMatrixScaling(1.5f, 1.5f, 1.5f);
				m_Mario_ColPos = XMFLOAT3(m_Mario_Pos.x, m_Mario_Pos.y + 0.5f, m_Mario_Pos.z);
			}
			else
			{
				matScale = XMMatrixScaling(2.0f, 2.0f, 2.0f);
				m_Mario_ColPos = XMFLOAT3(m_Mario_Pos.x, m_Mario_Pos.y + 2.0f, m_Mario_Pos.z - 0.1f);

			}
			matRot = XMMatrixRotationX(float(XMConvertToRadians(90.0f)));
			if(!getMush)
				matTrans = XMMatrixTranslation(m_Mario_Pos.x, m_Mario_Pos.y, m_Mario_Pos.z);			
			else if(getMush)
				matTrans = XMMatrixTranslation(m_Mario_Pos.x, m_Mario_Pos.y + 1.0f, m_Mario_Pos.z);
			worldMatrix = matScale * matRot * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Mario->GetVertexCount(), m_Mario->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Mario->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			m_Mario_Col->UpdateAABB(m_Mario_ColPos);
			m_MarioFoot_Col->UpdateAABB(XMFLOAT3(m_Mario_Pos.x, m_Mario_Pos.y, m_Mario_Pos.z));

			if (!result)
			{
				return false;
			}
		}

		//mushroom
		{
			if (Qboxhit)
			{
				if (!m_MushEatten)
				{
					m_Mush->Render(m_D3D->GetDeviceContext());
					matScale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
					matTrans = XMMatrixTranslation(m_MushPos.x, m_MushPos.y, m_MushPos.z);
					worldMatrix = matScale * matTrans;

					result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Mush->GetVertexCount(), m_Mush->GetInstanceCount(),
						worldMatrix, viewMatrix, projectionMatrix,
						m_Mush->GetTexture(),
						m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
						m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
				}
			}
		}

		//small pipe
		{
			m_Pipe_s->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(10.0f, 15.0f, 10.0f);
			worldMatrix = matScale;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Pipe_s->GetVertexCount(), m_Pipe_s->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Pipe_s->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//medium pipe
		{
			m_Pipe_m->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(10.0f, 18.5f, 10.0f);
			matTrans = XMMatrixTranslation(0.0f, 0.4f, 0.0f);
			worldMatrix = matScale * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Pipe_m->GetVertexCount(), m_Pipe_m->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Pipe_m->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//large pipe
		{
			m_Pipe_l->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(10.0f, 35.0f, 10.0f);
			matTrans = XMMatrixTranslation(0.0f, 1.5f, 0.0f);
			worldMatrix = matScale * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Pipe_l->GetVertexCount(), m_Pipe_l->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Pipe_l->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//brickcube
		{
			m_BrickCube->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.03f, 0.03f, 0.03f);
			worldMatrix = matScale;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_BrickCube->GetVertexCount(), m_BrickCube->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_BrickCube->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//stair1_1
		{
			m_Stair->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
			matTrans = XMMatrixTranslation(3.5f, -0.5f, 45.0f);

			worldMatrix = matScale * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stair->GetVertexCount(), m_Stair->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Stair->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//stair1_2
		{
			m_Stair->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
			matTrans = XMMatrixTranslation(-4.5f, -0.5f, 45.0f);

			worldMatrix = matScale * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stair->GetVertexCount(), m_Stair->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Stair->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//stair2_1
		{
			m_Stair->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
			matTrans = XMMatrixTranslation(5.5f, -0.5f, 255.0f);
			matRot = XMMatrixRotationY(float(XMConvertToRadians(180.0f)));

			worldMatrix = matScale * matRot * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stair->GetVertexCount(), m_Stair->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Stair->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//stair2_2
		{
			m_Stair->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
			matTrans = XMMatrixTranslation(-2.5f, -0.5f, 255.0f);
			matRot = XMMatrixRotationY(float(XMConvertToRadians(180.0f)));

			worldMatrix = matScale * matRot * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stair->GetVertexCount(), m_Stair->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Stair->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//stair3_1
		{
			m_Stair->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
			matTrans = XMMatrixTranslation(3.5f, -0.5f, 78.0f);

			worldMatrix = matScale * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stair->GetVertexCount(), m_Stair->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Stair->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//stair3_2
		{
			m_Stair->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
			matTrans = XMMatrixTranslation(-4.5f, -0.5f, 78.0f);

			worldMatrix = matScale * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stair->GetVertexCount(), m_Stair->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Stair->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//stair3_3
		{
			m_Stair2->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
			matTrans = XMMatrixTranslation(-4.5f, -0.5f, 46.0f);

			worldMatrix = matScale * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stair2->GetVertexCount(), m_Stair2->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Stair2->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//stair3_4
		{
			m_Stair2->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
			matTrans = XMMatrixTranslation(3.5f, -0.5f, 46.0f);

			worldMatrix = matScale * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stair2->GetVertexCount(), m_Stair2->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Stair2->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//stair4_1
		{
			m_Stair->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
			matTrans = XMMatrixTranslation(5.5f, -0.5f, 298.0f);
			matRot = XMMatrixRotationY(float(XMConvertToRadians(180.0f)));

			worldMatrix = matScale * matRot * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stair->GetVertexCount(), m_Stair->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Stair->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//stair4_2
		{
			m_Stair->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
			matTrans = XMMatrixTranslation(-2.5f, -0.5f, 298.0f);
			matRot = XMMatrixRotationY(float(XMConvertToRadians(180.0f)));

			worldMatrix = matScale * matRot * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stair->GetVertexCount(), m_Stair->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Stair->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//stair5_1
		{
			m_Stair->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
			matTrans = XMMatrixTranslation(3.5f, -0.5f, 157.0f);

			worldMatrix = matScale * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stair->GetVertexCount(), m_Stair->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Stair->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//stair5_2
		{
			m_Stair->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
			matTrans = XMMatrixTranslation(-4.5f, -0.5f, 157.0f);

			worldMatrix = matScale * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stair->GetVertexCount(), m_Stair->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Stair->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//stair5_3
		{
			m_Stair3->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
			matTrans = XMMatrixTranslation(3.5f, -0.5f, 157.0f);

			worldMatrix = matScale * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stair3->GetVertexCount(), m_Stair3->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Stair3->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//stair5_4
		{
			m_Stair3->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
			matTrans = XMMatrixTranslation(-4.5f, -0.5f, 157.0f);

			worldMatrix = matScale * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Stair3->GetVertexCount(), m_Stair3->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Stair3->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//qcube
		{
			m_QCube->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(3.15f, 3.15f, 3.15f);
			worldMatrix = matScale;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_QCube->GetVertexCount(), m_QCube->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_QCube->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result)
			{
				return false;
			}
		}

		//goomba
		{
			for (int i = 0; i < m_GoombaNum; i++)
			{
				if (m_GoombaAlive[i])
				{
					m_Goomba->Render(m_D3D->GetDeviceContext());
					m_GoombaPos[i] = XMFLOAT3(m_GoombaPos[i].x + m_enemyMove, m_GoombaPos[i].y, m_GoombaPos[i].z);
					matScale = XMMatrixScaling(0.07f, 0.07f, 0.07f);
					matTrans = XMMatrixTranslation(m_GoombaPos[i].x, m_GoombaPos[i].y, m_GoombaPos[i].z);
					matRot = XMMatrixRotationY(float(XMConvertToRadians(90.0f)));
					worldMatrix = matScale * matRot * matTrans;

					result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Goomba->GetVertexCount(), m_Goomba->GetInstanceCount(),
						worldMatrix, viewMatrix, projectionMatrix,
						m_Goomba->GetTexture(),
						m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
						m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

					m_GoombaCol[i]->UpdateAABB(XMFLOAT3(m_GoombaPos[i]));
				}
			}
			/*result = m_Goomba->UpdateGoombaInstanceBuffers(m_D3D->GetDevice());
			if (!result) { return false; }

			matScale = XMMatrixScaling(0.07f, 0.07f, 0.07f);
			matTrans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
			matRot = XMMatrixRotationY(float(XMConvertToRadians(90.0f)));
			worldMatrix = matScale * matTrans * matRot;

			m_Goomba->Render(m_D3D->GetDeviceContext());
			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Goomba->GetVertexCount(), m_Goomba->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Goomba->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			for (int i = 0; i < 14; i++)
			{
				m_GoombaCol[i]->UpdateAABB(XMFLOAT3(m_GoombaPos[i].x, m_GoombaPos[i].y, m_GoombaPos[i].z));
			}
			if (!result) { return false; }*/
		}

		//koopatroopa
		{
			if (m_KoopaTroopaAlive)
			{
				m_KoopaTroopa->Render(m_D3D->GetDeviceContext());
				m_KoopaTroopaPos = XMFLOAT3(m_KoopaTroopaPos.x + m_enemyMove, m_KoopaTroopaPos.y, m_KoopaTroopaPos.z);
				matScale = XMMatrixScaling(0.018f, 0.018f, 0.018f);
				matTrans = XMMatrixTranslation(m_KoopaTroopaPos.x, m_KoopaTroopaPos.y, m_KoopaTroopaPos.z);
				worldMatrix = matScale * matTrans;

				result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_KoopaTroopa->GetVertexCount(), m_KoopaTroopa->GetInstanceCount(),
					worldMatrix, viewMatrix, projectionMatrix,
					m_KoopaTroopa->GetTexture(),
					m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
					m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

				if (!result) { return false; }

				m_KoopaTroopaCol->UpdateAABB(m_KoopaTroopaPos);
			}
			
			else
			{
				m_Shell->Render(m_D3D->GetDeviceContext());
				matScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
				m_KoopaTroopaPos = XMFLOAT3(m_KoopaTroopaPos.x, m_KoopaTroopaPos.y + shellGravity, m_KoopaTroopaPos.z + shellMove);
				matTrans = XMMatrixTranslation(m_KoopaTroopaPos.x, m_KoopaTroopaPos.y, m_KoopaTroopaPos.z);
				worldMatrix = matScale * matTrans;

				result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Shell->GetVertexCount(), m_Shell->GetInstanceCount(),
					worldMatrix, viewMatrix, projectionMatrix,
					m_Shell->GetTexture(),
					m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
					m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

				if (!result) { return false; }

				m_ShellCol->UpdateAABB(m_KoopaTroopaPos);
			}
		}

		//castle
		{
			m_Castle->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.23f, 0.23f, 0.23f);
			matTrans = XMMatrixTranslation(0.3f, -3.0f, 320.0f);
			worldMatrix = matScale * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Castle->GetVertexCount(), m_Castle->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Castle->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result) { return false; }
		}

		//flag
		{
			m_Flag->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.1f, 0.1f, 0.1f);
			matTrans = XMMatrixTranslation(0.0f, 0.0f, 278.0f);
			worldMatrix = matScale * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Flag->GetVertexCount(), m_Flag->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Flag->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result) { return false; }
		}

		//flagcube
		{
			m_Flagcube->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
			matTrans = XMMatrixTranslation(0.0f, -0.5f, 295.0f);

			worldMatrix = matScale * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Flagcube->GetVertexCount(), m_Flagcube->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Flagcube->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result) { return false; }
		}

		//ground
		{
			m_Ground->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(150.0f, 4.0f, 20.0f);
			matRot = XMMatrixRotationY(float(XMConvertToRadians(90.0f)));
			matTrans = XMMatrixTranslation(0.0f, 1.5f, 0.0f);
			worldMatrix = matScale * matRot * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Ground->GetVertexCount(), m_Ground->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Ground->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result) { return false; }
		}

		//ground_s
		{
			m_Ground_s->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(40.0f, 4.0f, 20.0f);
			matRot = XMMatrixRotationY(float(XMConvertToRadians(90.0f)));
			matTrans = XMMatrixTranslation(0.0f, 1.5f, 0.0f);
			worldMatrix = matScale * matRot * matTrans;

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Ground_s->GetVertexCount(), m_Ground_s->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix,
				m_Ground_s->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

			if (!result) { return false; }
		}

		//skybox
		{
			//skybox1
			m_Skybox->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(1000.0f, 1000.0f, 1.0f);
			matRot = XMMatrixRotationY(float(XMConvertToRadians(90.0f)));
			matTrans = XMMatrixTranslation(-300.0f, 0.0f, 0.0f);
			worldMatrix = matScale * matRot * matTrans;
			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Skybox->GetVertexCount(), m_Skybox->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix, m_Skybox->GetTexture());
			if (!result)
			{
				return false;
			}

			//skybox2
			m_Skybox->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(1000.0f, 1000.0f, 1.0f);
			matRot = XMMatrixRotationY(float(XMConvertToRadians(90.0f)));
			matTrans = XMMatrixTranslation(300.0f, 0.0f, 0.0f);
			worldMatrix = matScale * matRot * matTrans;
			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Skybox->GetVertexCount(), m_Skybox->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix, m_Skybox->GetTexture());
			if (!result)
			{
				return false;
			}

			//skybox3
			m_Skybox->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(1000.0f, 1000.0f, 1.0f);
			matTrans = XMMatrixTranslation(0.0f, 0.0f, 700.0f);
			worldMatrix = matScale * matTrans;
			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Skybox->GetVertexCount(), m_Skybox->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix, m_Skybox->GetTexture());
			if (!result)
			{
				return false;
			}

			//skybox4
			m_Skybox->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(1000.0f, 1000.0f, 1.0f);
			matTrans = XMMatrixTranslation(0.0f, 0.0f, -700.0f);
			worldMatrix = matScale * matTrans;
			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Skybox->GetVertexCount(), m_Skybox->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix, m_Skybox->GetTexture());
			if (!result)
			{
				return false;
			}
			
			//skybox5
			m_Skybox->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(1000.0f, 1000.0f, 1.0f);
			matRot = XMMatrixRotationX(float(XMConvertToRadians(90.0f)));
			matTrans = XMMatrixTranslation(0.0f, 700.0f, 0.0f);
			worldMatrix = matScale * matRot * matTrans;
			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Skybox->GetVertexCount(), m_Skybox->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix, m_Skybox->GetTexture());
			if (!result)
			{
				return false;
			}
			
			//skybox6
			m_Skybox->Render(m_D3D->GetDeviceContext());
			matScale = XMMatrixScaling(1000.0f, 1000.0f, 1.0f);
			matRot = XMMatrixRotationX(float(XMConvertToRadians(90.0f)));
			matTrans = XMMatrixTranslation(0.0f, -700.0f, 0.0f);
			worldMatrix = matScale * matRot * matTrans;
			result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Skybox->GetVertexCount(), m_Skybox->GetInstanceCount(),
				worldMatrix, viewMatrix, projectionMatrix, m_Skybox->GetTexture());
			if (!result)
			{
				return false;
			}
		}

		m_D3D->GetWorldMatrix(worldMatrix);
		// Turn off the Z buffer to begin all 2D rendering.
		m_D3D->TurnZBufferOff();

		// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		result = m_1up->Render(m_D3D->GetDeviceContext(), 720, 0);
		if (!result)
		{
			return false;
		}

		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_1up->GetIndexCount(), 1, worldMatrix, baseViewMatrix, orthoMatrix, m_1up->GetTexture());
		if (!result)
		{
			return false;
		}

		// Turn on the alpha blending before rendering the text.
		m_D3D->TurnOnAlphaBlending();

		// Render the text strings.
		result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
		if (!result)
		{
			return false;
		}

		// Turn off alpha blending after rendering the text.
		m_D3D->TurnOffAlphaBlending();
		break;
	}
	case GraphicsClass::Clear:
	{
		m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
		// Generate the view matrix based on the camera's position.
		m_Camera->Render();

		// Get the world, view, and projection matrices from the camera and d3d objects.
		m_Camera->GetViewMatrix(viewMatrix);
		m_D3D->GetWorldMatrix(worldMatrix);
		m_D3D->GetProjectionMatrix(projectionMatrix);
		m_D3D->GetOrthoMatrix(orthoMatrix);

		// Turn off the Z buffer to begin all 2D rendering.
		m_D3D->TurnZBufferOff();

		// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		result = m_GameClear->Render(m_D3D->GetDeviceContext(), 0, 0);
		if (!result)
		{
			return false;
		}

		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_GameClear->GetIndexCount(), 1, worldMatrix, viewMatrix, orthoMatrix, m_GameClear->GetTexture());
		if (!result)
		{
			return false;
		}

		// Turn the Z buffer back on now that all 2D rendering has completed.
		m_D3D->TurnZBufferOn();
		break;
	}
	case GraphicsClass::Over:
	{
		m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
		// Generate the view matrix based on the camera's position.
		m_Camera->Render();

		// Get the world, view, and projection matrices from the camera and d3d objects.
		m_Camera->GetViewMatrix(viewMatrix);
		m_D3D->GetWorldMatrix(worldMatrix);
		m_D3D->GetProjectionMatrix(projectionMatrix);
		m_D3D->GetOrthoMatrix(orthoMatrix);

		// Turn off the Z buffer to begin all 2D rendering.
		m_D3D->TurnZBufferOff();

		// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
		result = m_GameOver->Render(m_D3D->GetDeviceContext(), 0, 0);
		if (!result)
		{
			return false;
		}

		// Render the bitmap with the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_GameOver->GetIndexCount(), 1, worldMatrix, viewMatrix, orthoMatrix, m_GameOver->GetTexture());
		if (!result)
		{
			return false;
		}

		// Turn the Z buffer back on now that all 2D rendering has completed.
		m_D3D->TurnZBufferOn();
		break;
	}
	default:
		break;
	}	
	
	m_D3D->EndScene();

	return true;
}

void GraphicsClass::ShellMovePlus()
{
	if (m_shellHit)
	{
		shellMove += 0.0005f * shelldir;
		if (shelldir <= 0.0f && !ischange)
		{
			OutputDebugString(L"work\n");

			shellMove = 0.0f;
			ischange = true;
		}
	}
}

void GraphicsClass::EnemyMovePlus()
{
	if (m_enemyMove >= 0.05f)
	{
		dir = dir * -1.0f;
	}

	else if (m_enemyMove <= -0.05f)
	{
		dir = dir * -1.0f;
	}
	
	m_enemyMove += 0.0003f * dir;

}

void GraphicsClass::SetObjPolyCnt()
{
	m_PolyCnt = m_MarioPolyCnt / 3 + m_PipePolyCnt / 3 + m_BrickCubePolyCnt / 3 + m_StairPolyCnt / 3 + m_QCubePolyCnt / 3
		+ m_GoombaPolyCnt / 3 + m_KoopaTroopaPolyCnt / 3 + m_CastlePolyCnt / 3 + m_FlagPolyCnt / 3 + m_GroundPolyCnt / 3;

	m_ObjCnt = m_MarioObjCnt + m_PipeObjCnt + m_BrickCubeObjCnt + m_StairObjCnt + m_QCubeObjCnt + m_GoombaObjCnt + m_KoopaTroopaObjCnt
		+ m_CastleObjCnt + m_FlagObjCnt + m_GroundObjCnt;
}

void GraphicsClass::CheckLife()
{
	if (m_Life <= 0)
	{
		gameOver = true;
		m_Camera->SetRotation(0.0f, 0.0f, 0.0f);
		m_stage = EStage::Over;
	}
}

void GraphicsClass::Timer(float frameTime)
{
	m_Timer += frameTime / 1000.0f;
}

void GraphicsClass::CheckMarioJump()
{
	if (is_mario_grounded)
		gravity = 0.0f;
	else
		gravity = -0.25f;

	if (shellGround)
		shellGravity = 0.0f;
	else
		shellGravity = -0.25f;

	if (is_jump)
	{
		m_Mario_Pos.y += 0.45f;
		is_mario_grounded = false;
	}

	if (m_Mario_Pos.y >= saveY + 12.0f)
	{
		is_jump = false;
	}
}

void GraphicsClass::CheckCollision()
{
	is_mario_grounded = false;
	shellGround = false;
	back_speed = 0.5f;

	////////////////////
	///MUSH COLLISION///
	////////////////////
	if (Qboxhit)
	{
		if (m_Mario_Col->CheckAABB(m_MushCol))
		{
			getMush = true;
			m_MushEatten = true;
		}
	}

	/////////////////////
	///ENEMY COLLISION///
	/////////////////////

	//goomba
	for (int i = 0; i < m_GoombaNum; i++)
	{
		if (!m_GoombaAlive[i]) continue;

		if (m_Mario_Col->CheckAABB(m_GoombaCol[i]))
		{
			if (getMush)
				getMush = false;
			else
			{
				m_Life -= 1;
				m_Mario_Pos = startPos;
				OutputDebugString(L"mario hit by Goomba\n");
			}
		}
		else if (m_MarioFoot_Col->CheckAABB(m_GoombaCol[i]))
		{
			m_GoombaAlive[i] = false;
			is_jump = true;
			OutputDebugString(L"kill goomba\n");
		}
		else if (m_ShellCol->CheckAABB(m_GoombaCol[i]))
		{
			m_GoombaAlive[i] = false;
		}
	}

	//turtle
	if (m_KoopaTroopaAlive)
	{
		if (m_Mario_Col->CheckAABB(m_KoopaTroopaCol))
		{
			if (getMush)
				getMush = false;
			else
			{
				m_Life -= 1;
				m_Mario_Pos = startPos;
				OutputDebugString(L"mario hit by turtle\n");
			}
		}

		else if (m_MarioFoot_Col->CheckAABB(m_KoopaTroopaCol))
		{
			is_jump = true;
			m_KoopaTroopaAlive = false;
			OutputDebugString(L"turtle\n");

		}
	}

	//shell
	if (m_MarioFoot_Col->CheckAABB(m_ShellCol))
	{
		m_shellHit = true;
		OutputDebugString(L"shellhit\n");
	}
	else if (m_Mario_Col->CheckAABB(m_ShellCol))
	{
		if (getMush)
			getMush = false;
		else
		{
			m_Life -= 1;
			m_Mario_Pos = startPos;
		}
	}

	////////////////////
	///PIPE COLLISION///
	////////////////////
	for (int i = 0; i < 3; i++)
	{
		if (m_Mario_Col->CheckAABB(m_Pipe_s_Col[i]))
		{
			is_mario_grounded = true;

			forward_speed = 0.001f;
			back_speed = 0.5f;
		}

	}
	if (m_Mario_Col->CheckAABB(m_Pipe_m_Col))
	{
		is_mario_grounded = true;

		forward_speed = 0.001f;
		back_speed = 0.5f;
	}

	for (int i = 0; i < 2; i++)
	{
		if (m_Mario_Col->CheckAABB(m_Pipe_l_Col[i]))
		{
			is_mario_grounded = true;

			forward_speed = 0.001f;
			back_speed = 0.5f;
		}
	}

	/////////////////////
	///GROUND COLLSION///
	/////////////////////
	for (int i = 0; i < 3; i++)
	{
		if (m_MarioFoot_Col->CheckAABB(m_Ground_Col[i]))
		{
			is_mario_grounded = true;
		}

		else if (m_MarioFoot_Col->CheckAABB(m_Ground_Col[i]))
		{
			is_mario_grounded = true;
		}

		if (m_ShellCol->CheckAABB(m_Ground_Col[i]))
		{
			shellGround = true;
		}
		
	}

	if (m_Mario_Col->CheckAABB(m_Ground_s_Col) || m_MarioFoot_Col->CheckAABB(m_Ground_s_Col))
	{
		is_mario_grounded = true;

	}

	//////////////////////
	///S STAIR COLLSION///
	//////////////////////
	for (int i = 0; i < 16; i++)
	{
		if (m_MarioFoot_Col->CheckAABB(m_SStairCol[i]))
		{
			is_mario_grounded = true;
			forward_speed = 0.001f;

		}
		else if (m_Mario_Col->CheckAABB(m_SStairCol[i]))
		{
			is_mario_grounded = true;
			forward_speed = 0.001f;
			
		}

		if (m_ShellCol->CheckAABB(m_SStairCol[i]))
		{
			if(!ischange)
				shelldir = shelldir * -1.0f;

		}
	}

	//////////////////////
	///L STAIR COLLSION///
	//////////////////////
	for (int i = 0; i < 8; i++)
	{
		if (m_MarioFoot_Col->CheckAABB(m_SStairCol[i]))
		{
			is_mario_grounded = true;
			forward_speed = 0.001f;

		}
		else if (m_Mario_Col->CheckAABB(m_LStairCol[i]))
		{
			is_mario_grounded = true;
			forward_speed = 0.001f;

		}
	}

	////////////////////
	///QCUBE COLLSION///
	////////////////////
	if (m_Mario_Col->CheckAABB(m_QBoxHitCol))
	{
		Qboxhit = true;
	}
	for (int i = 0; i < 10; i++)
	{
		if (m_MarioFoot_Col->CheckAABB(m_CubeCol[i]))
		{
			is_mario_grounded = true;
			is_jump = false;
		}
		else if (m_Mario_Col->CheckAABB(m_CubeCol[i]))
		{
			is_jump = false;
		}
	}

	/////////////////////
	////FLAG COLLSION////
	/////////////////////
	if (m_Mario_Col->CheckAABB(m_FlagCol) || m_MarioFoot_Col->CheckAABB(m_FlagCol))
	{
		isGoal = true;
		//gameClear = true;
		//m_Camera->SetRotation(0.0f, 0.0f, 0.0f);
		//m_stage = EStage::Clear;
	}

	/////////////////////
	////GOAL COLLSION////
	/////////////////////
	if (m_Mario_Col->CheckAABB(m_CastleCol) || m_MarioFoot_Col->CheckAABB(m_CastleCol))
	{
		gameClear = true;
		m_Camera->SetRotation(0.0f, 0.0f, 0.0f);
		m_stage = EStage::Clear;
	}

	////////////////////////////
	////FALL GROUND COLLSION////
	////////////////////////////
	if (m_MarioFoot_Col->CheckAABB(m_FallGroundCol) || m_Mario_Col->CheckAABB(m_FallGroundCol))
	{
		getMush = false;
		m_Life -= 1;
		m_Mario_Pos = startPos;
	}
}

void GraphicsClass::MovingMario(int n)
{
	float speed2 = 0.2f;

	//w
	if (n == 1)
	{
		//m_Mario_Pos.x = m_Mario_Pos.x + speed;
		m_Mario_Pos.z = m_Mario_Pos.z + forward_speed;
	}

	//s
	else if (n == 2)
	{
		m_Mario_Pos.z = m_Mario_Pos.z - back_speed;
	}

	//a
	else if (n == 3)
	{
		m_Mario_Pos.x = m_Mario_Pos.x - forward_speed;
	}

	//d
	else if (n == 4)
	{
		m_Mario_Pos.x = m_Mario_Pos.x + forward_speed;
	}

	//wa
	else if (n == 5)
	{
		m_Mario_Pos.x = m_Mario_Pos.x - forward_speed;
		m_Mario_Pos.z = m_Mario_Pos.z + forward_speed;
	}

	//wd
	else if (n == 6)
	{
		m_Mario_Pos.x = m_Mario_Pos.x + forward_speed;
		m_Mario_Pos.z = m_Mario_Pos.z + forward_speed;
	}

	//sa
	else if (n == 7)
	{
		m_Mario_Pos.x = m_Mario_Pos.x - forward_speed;
		m_Mario_Pos.z = m_Mario_Pos.z - forward_speed;
	}

	//sd
	else if (n == 8)
	{
		m_Mario_Pos.x = m_Mario_Pos.x + forward_speed;
		m_Mario_Pos.z = m_Mario_Pos.z - forward_speed;
	}

	//up
   	else if (n == 9 && is_mario_grounded)
	{
		is_jump = true;
		saveY = m_Mario_Pos.y;
	}
}

void GraphicsClass::AddTitleNum()
{
	titleNum += 1;

	if (titleNum == 3)
		m_stage = EStage::Main;
}

/*void GraphicsClass::MovingCamera(int n)
{
	float speed = 0.3f;

	if (n == 1)
	{
		m_Camera->SetPosition(m_Camera->GetPosition().x + speed * m_Camera->GetMLookAt().x, 
							m_Camera->GetPosition().y, 
							m_Camera->GetPosition().z + speed * m_Camera->GetMLookAt().z);


	}
	else if (n == 2)
	{
		m_Camera->SetPosition(m_Camera->GetPosition().x - speed * m_Camera->GetMLookAt().x, 
							m_Camera->GetPosition().y, 
							m_Camera->GetPosition().z - speed * m_Camera->GetMLookAt().z);
	}
	else if (n == 3)
	{
		m_Camera->SetPosition(m_Camera->GetPosition().x - speed * m_Camera->GetMLookAt().z, 
							m_Camera->GetPosition().y, 
							m_Camera->GetPosition().z + speed * m_Camera->GetMLookAt().x);
	}
	else if (n == 4)
	{
		m_Camera->SetPosition(m_Camera->GetPosition().x + speed * m_Camera->GetMLookAt().z, 
							m_Camera->GetPosition().y, 
							m_Camera->GetPosition().z - speed * m_Camera->GetMLookAt().x);
	}

	else if (n == 5)
	{
		m_Camera->SetPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y + speed,
			m_Camera->GetPosition().z);
	}

	else if (n == 6)
	{
		m_Camera->SetPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y - speed,
			m_Camera->GetPosition().z);
	}

	return;
}
*/

void GraphicsClass::CameraRotation(float x, float y, float z)
{
	m_Camera->SetRotation(x, y, z);
}

XMFLOAT3 GraphicsClass::GetCameraRotation()
{
	return m_Camera->GetRotation();
}

void GraphicsClass::OnOffLight(int mode)
{
	switch (mode)
	{
		case 0:
			if (am_on == true)
			{
				m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
				am_on = false;
			}
			else
			{
				m_Light->SetAmbientColor(0.0f, 0.0f, 0.0f, 0.0f);
				am_on = true;
			}
			break;

		case 1:
			if (di_on == true)
			{
				m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
				di_on = false;
			}
			else
			{
				m_Light->SetDiffuseColor(0.0f, 0.0f, 0.0f, 0.0f);
				di_on = true;
			}
			break;

		case 2:
			if (sp_on == true)
			{
				m_Light->SetSpecularPower(32.0f);
				sp_on = false;
			}
			else
			{
				m_Light->SetSpecularPower(20000.0f);
				sp_on = true;
			}
			break;
	}
}