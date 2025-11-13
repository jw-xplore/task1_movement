#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "EntityManager.h"
#include "Agent.h"
#include "Path.h"

EntityManager* entityManager;
Agent* agent;
Agent* agent2;
Path* path;
SteeringBehavior* steeringBehavior;

// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );

	// Setup entity manager and entities
	entityManager = new EntityManager();
	steeringBehavior = new SteeringBehavior();

	agent = new Agent({100, 100}, steeringBehavior);
	agent2 = new Agent({ 200, 20 }, steeringBehavior);

	entityManager->AddEntity(agent);
	entityManager->AddEntity(agent2);

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

