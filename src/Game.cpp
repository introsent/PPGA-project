#include <iostream>
#include <algorithm>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_ttf.h>
#include <chrono>
#include "Game.h"
#include "utils.h"
#include "structs.h"
#include "FlyFish.h"

Game::Game(const Window& window)
	: m_Window{ window }
	, m_Viewport{ 0,0,window.width,window.height }
	, m_pWindow{ nullptr }
	, m_pContext{ nullptr }
	, m_Initialized{ false }
	, m_MaxElapsedSeconds{ 0.1f }
{
	InitializeGameEngine();
}

Game::~Game()
{
	CleanupGameEngine();
}

void Game::InitializeGameEngine()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "BaseGame::Initialize( ), error when calling SDL_Init: " << SDL_GetError() << std::endl;
		return;
	}

	// Use OpenGL 2.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// Create window
	m_pWindow = SDL_CreateWindow(
		m_Window.title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		int(m_Window.width),
		int(m_Window.height),
		SDL_WINDOW_OPENGL);
	if (m_pWindow == nullptr)
	{
		std::cerr << "BaseGame::Initialize( ), error when calling SDL_CreateWindow: " << SDL_GetError() << std::endl;
		return;
	}

	// Create OpenGL context 
	m_pContext = SDL_GL_CreateContext(m_pWindow);
	if (m_pContext == nullptr)
	{
		std::cerr << "BaseGame::Initialize( ), error when calling SDL_GL_CreateContext: " << SDL_GetError() << std::endl;
		return;
	}

	// Set the swap interval for the current OpenGL context,
	// synchronize it with the vertical retrace
	if (m_Window.isVSyncOn)
	{
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			std::cerr << "BaseGame::Initialize( ), error when calling SDL_GL_SetSwapInterval: " << SDL_GetError() << std::endl;
			return;
		}
	}
	else
	{
		SDL_GL_SetSwapInterval(0);
	}

	// Set the Projection matrix to the identity matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set up a two-dimensional orthographic viewing region.
	glOrtho(0, m_Window.width, 0, m_Window.height, -1, 1); // y from bottom to top

	// Set the viewport to the client window area
	// The viewport is the rectangular region of the window where the image is drawn.
	glViewport(0, 0, int(m_Window.width), int(m_Window.height));

	// Set the Modelview matrix to the identity matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Enable color blending and use alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		std::cerr << "BaseGame::Initialize( ), error when calling TTF_Init: " << TTF_GetError() << std::endl;
		return;
	}

	m_Initialized = true;

	m_CarUPtr = std::make_unique<Car>(ThreeBlade(360.f, 200.f, 0.f), TwoBlade(0.f, 1.f, 0.f, 0.f, 0.f, 0.f), 0.f);

	//Left of the road
	m_MapPoints.emplace_back(132.f, 50.f);
	m_MapPoints.emplace_back(132.f, 189.f);
	m_MapPoints.emplace_back(193.f, 265.f);
	m_MapPoints.emplace_back(306.f, 288.f);
	m_MapPoints.emplace_back(375.f, 373.f);
	m_MapPoints.emplace_back(355.f, 455.f);
	m_MapPoints.emplace_back(130.f, 443.f);
	m_MapPoints.emplace_back(50.f, 1000.f - 458.f);
	m_MapPoints.emplace_back(92.f, 1000.f - 340.f);
	m_MapPoints.emplace_back(295.f, 1000.f - 320.f);
	m_MapPoints.emplace_back(470.f, 1000.f - 390.f); 
	m_MapPoints.emplace_back(510.f, 1000.f - 465.f);
	m_MapPoints.emplace_back(526.f, 1000.f - 604.f);
	m_MapPoints.emplace_back(535.f, 1000.f - 771.f);
	m_MapPoints.emplace_back(560.f, 1000.f - 803.f);
	m_MapPoints.emplace_back(632.f, 1000.f - 792.f);
	m_MapPoints.emplace_back(625.f, 1000.f - 637.f);
	m_MapPoints.emplace_back(640.f, 1000.f - 535.f);
	m_MapPoints.emplace_back(700.f, 1000.f - 470.f);
	m_MapPoints.emplace_back(795.f, 1000.f - 475.f);
	m_MapPoints.emplace_back(871.f, 1000.f - 497.f);
	m_MapPoints.emplace_back(922.f, 1000.f - 568.f);
	m_MapPoints.emplace_back(944.f, 1000.f - 660.f);
	m_MapPoints.emplace_back(944.f, 1000.f - 950.f);

	//Right of the road
	m_MapPoints.emplace_back(865.f, 1000.f - 950.f);
	m_MapPoints.emplace_back(865.f, 1000.f - 755.f);
	m_MapPoints.emplace_back(850.f, 1000.f - 600.f);
	m_MapPoints.emplace_back(820.f, 1000.f - 560.f);
	m_MapPoints.emplace_back(755.f, 1000.f - 545.f);
	m_MapPoints.emplace_back(715.f, 1000.f - 560.f);
	m_MapPoints.emplace_back(700.f, 1000.f - 655.f);
	m_MapPoints.emplace_back(700.f, 1000.f - 755.f);
	m_MapPoints.emplace_back(694.f, 1000.f - 823.f);
	m_MapPoints.emplace_back(640.f, 1000.f - 860.f);
	m_MapPoints.emplace_back(542.f, 1000.f - 860.f);
	m_MapPoints.emplace_back(472.f, 1000.f - 820.f);
	m_MapPoints.emplace_back(462.f, 1000.f - 620.f);
	m_MapPoints.emplace_back(430.f, 1000.f - 460.f);
	m_MapPoints.emplace_back(375.f, 1000.f - 400.f);
	m_MapPoints.emplace_back(225.f, 1000.f - 380.f);
	m_MapPoints.emplace_back(125.f, 1000.f - 407.f);
	m_MapPoints.emplace_back(140.f, 1000.f - 485.f);
	m_MapPoints.emplace_back(280.f, 1000.f - 485.f);
	m_MapPoints.emplace_back(380.f, 1000.f - 495.f);
	m_MapPoints.emplace_back(430.f, 1000.f - 535.f);
	m_MapPoints.emplace_back(435.f, 1000.f - 655.f);
	m_MapPoints.emplace_back(395.f, 1000.f - 735.f);
	m_MapPoints.emplace_back(240.f, 1000.f - 795.f);
	m_MapPoints.emplace_back(215.f, 1000.f - 820.f);
	m_MapPoints.emplace_back(210.f, 1000.f - 950.f);

	for (Point2f& point : m_MapPoints)
	{
		point.x *= 2.f;
		point.y *= 2.f;
	}


	m_CameraUPtr = std::make_unique<Camera>(GetViewPort().width, GetViewPort().height);

	auto maxXPoint = std::max_element(m_MapPoints.begin(), m_MapPoints.end(), [](const Point2f& a, const Point2f& b) {
		return a.x < b.x;
		});
	m_MaxMapWidth = maxXPoint->x;

	auto maxYPoint = std::max_element(m_MapPoints.begin(), m_MapPoints.end(), [](const Point2f& a, const Point2f& b) {
		return a.y < b.y;
		});
	m_MaxMapHeight =  maxYPoint->y;

}

void Game::Run()
{
	if (!m_Initialized)
	{
		std::cerr << "BaseGame::Run( ), BaseGame not correctly initialized, unable to run the BaseGame\n";
		std::cin.get();
		return;
	}

	// Main loop flag
	bool quit{ false };

	// Set start time
	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

	//The event loop
	SDL_Event e{};
	while (!quit)
	{
		// Poll next event from queue
		while (SDL_PollEvent(&e) != 0)
		{
			// Handle the polled event
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_SPACE)
				{
					m_CarUPtr->Snap();
				}
				this->ProcessKeyDownEvent(e.key);
				break;
			case SDL_KEYUP:
				if (e.key.keysym.sym == SDLK_w)
				{
					m_CarUPtr->IncreaseSpeed();
				}
				else if (e.key.keysym.sym == SDLK_s)
				{
					m_CarUPtr->DecreaseSpeed();
				}
				this->ProcessKeyUpEvent(e.key);
				break;
			case SDL_MOUSEMOTION:
				e.motion.y = int(m_Window.height) - e.motion.y;
				this->ProcessMouseMotionEvent(e.motion);
				break;
			case SDL_MOUSEBUTTONDOWN:
			{
				e.button.y = int(m_Window.height) - e.button.y;

				Point2f clickPos = m_CameraUPtr->GetWorldLocation(Point2f(float(e.button.x), float(e.button.y)));

				m_IsDrifting = true;
				if (m_IsDrifting)
				{
					m_OrbitPoint = ThreeBlade(clickPos.x, clickPos.y, 0.f);
				}

				//m_CarUPtr->Rotate(ThreeBlade(e.button.x, e.button.y, 0.f));
				this->ProcessMouseDownEvent(e.button);
				break;
			}
			case SDL_MOUSEBUTTONUP:
				m_IsDrifting = false;
				m_CarUPtr->SetStartedRotating(false);
				e.button.y = int(m_Window.height) - e.button.y;
				this->ProcessMouseUpEvent(e.button);
				break;
			}
		}

		if (!quit)
		{
			// Get current time
			std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

			// Calculate elapsed time
			float elapsedSeconds = std::chrono::duration<float>(t2 - t1).count();

			// Update current time
			t1 = t2;

			// Prevent jumps in time caused by break points
			elapsedSeconds = std::min(elapsedSeconds, m_MaxElapsedSeconds);

			// Call the BaseGame object 's Update function, using time in seconds (!)
			this->Update(elapsedSeconds);

			// Draw in the back buffer
			this->Draw();

			// Update screen: swap back and front buffer
			SDL_GL_SwapWindow(m_pWindow);
		}
	}
}

void Game::CleanupGameEngine()
{
	SDL_GL_DeleteContext(m_pContext);

	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;

	//Quit SDL subsystems
	TTF_Quit();
	SDL_Quit();

}

void Game::Update(float elapsedSec)
{
	for (int i = 0; i < m_MapPoints.size(); ++i)
	{
		ThreeBlade point1 = ThreeBlade(m_MapPoints[i % m_MapPoints.size()].x, m_MapPoints[i % m_MapPoints.size()].y, 0.f);
		ThreeBlade point2 = ThreeBlade(m_MapPoints[(i + 1) % m_MapPoints.size()].x, m_MapPoints[(i + 1) % m_MapPoints.size()].y, 0.f);

		TwoBlade borderTwoBlade = TwoBlade::LineFromPoints(point1[0], point1[1], point1[2], point2[0], point2[1], point2[2]);

		m_CarUPtr->CheckIntersectionWithMapBorders(borderTwoBlade, point1, point2);
	}

	if (m_IsDrifting)
	{
		m_CarUPtr->Orbit(m_OrbitPoint);
		m_CarUPtr->RotateLookAt();
		m_CarUPtr->UpdateSideForce(elapsedSec);
	}
	else
	{
		m_CarUPtr->UpdateSideForce(elapsedSec);
		m_CarUPtr->UpdateForwardForce(elapsedSec);
	}

	m_CameraUPtr->Aim(m_MaxMapWidth, m_MaxMapHeight, m_CarUPtr->GetCarWorldLocation());	

	m_CarUPtr->UpdateCarPointsLocalSpace(m_CameraUPtr.get());

	m_MapPointsLocalSpace.clear();
	for (const Point2f& mapPoint : m_MapPoints)
	{
		m_MapPointsLocalSpace.push_back(m_CameraUPtr->GetAppliedTransform(mapPoint));
	}
	
}

void Game::Draw() const
{
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_CarUPtr->Draw();

	utils::SetColor(Color4f(0.7f, 0.7f, 0.7f, 1.f));

	utils::DrawPolygon(m_MapPointsLocalSpace);
}
