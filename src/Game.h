#pragma once
#include "structs.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <memory>

#include "Camera.h"

#include "FlyFish.h"
#include "Car.h" 

#include "Camera.h"

class Game
{
public:
	explicit Game( const Window& window );
	Game( const Game& other ) = delete;
	Game& operator=( const Game& other ) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;

	~Game();

	void Run( );

	void Update(float elapsedSec);

	void Draw() const;

	// Event handling
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
	{

	}
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
	{

	}
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
	{
		
	}
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
	{
		
	}
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
	{
		
	}

	const Rectf& GetViewPort() const
	{
		return m_Viewport;
	}

private:
	// DATA MEMBERS
	std::unique_ptr<Car> m_CarUPtr;
	std::unique_ptr<Camera> m_CameraUPtr;

	std::vector<Point2f> m_MapPointsRight;
	std::vector<Point2f> m_MapPointsLeft;

	std::vector<Point2f> m_MapPoints;
	std::vector<Point2f> m_MapPointsLocalSpace;


	float m_MaxMapWidth;
	float m_MaxMapHeight;

	bool m_IsDrifting = false;
	ThreeBlade m_OrbitPoint;

	// The window properties
	const Window m_Window;
	const Rectf m_Viewport;
	// The window we render to
	SDL_Window* m_pWindow;
	// OpenGL context
	SDL_GLContext m_pContext;
	// Init info
	bool m_Initialized;
	// Prevent timing jumps when debugging
	const float m_MaxElapsedSeconds;
	
	
	// FUNCTIONS
	void InitializeGameEngine( );
	void CleanupGameEngine( );
};
