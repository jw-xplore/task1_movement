#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "EntityManager.h"
#include "Agent.h"
#include "Path.h"

EntityManager* entityManager;
Agent* agent;
Path* path;

// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );

	// Setup entity manager and entities
	entityManager = new EntityManager();
	agent = new Agent();

	entityManager->AddEntity(agent);

	Play::CentreAllSpriteOrigins();

	// Setup path
	path = new Path({ { 150, 40 }, { 280, 100 }, { 400, 50 }, { 510, 200 } });
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	Play::ClearDrawingBuffer( Play::cBlack );
	entityManager->UpdateEntities(elapsedTime);

	//agent->FollowPath(*path, 10);

	path->DrawPath();
	//path->PosInPath(agent->GetPosition());

	Play::PresentDrawingBuffer();
	return Play::KeyDown( KEY_ESCAPE );
}

// Gets called once when the player quits the game 
int MainGameExit( void )
{
	Play::DestroyManager();
	return PLAY_OK;
}

