#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "EntityManager.h"
#include "Agent.h"
#include "Path.h"
#include "CollisionHandler.h"

EntityManager* entityManager;
Agent* agent;
Agent* agent2;
Path* path;
SteeringBehavior* steeringBehavior;
CollisionHandler* collisionHandler;

// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );

	// Setup entity manager and entities
	entityManager = new EntityManager();
	steeringBehavior = new SteeringBehavior();
	collisionHandler = new CollisionHandler();

	// Setup path
	path = new Path({ { 150, 40 }, { 280, 100 }, { 400, 50 }, { 600, 200 }, { 400, 250 }, { 300, 300} });

	agent = new Agent({100, 100}, steeringBehavior);
	agent->collisionHandler = collisionHandler;
	agent->walls.insert(agent->walls.end(), path);
	//agent2 = new Agent({ 200, 20 }, steeringBehavior);

	entityManager->AddEntity(agent);
	//entityManager->AddEntity(agent2);

	Play::CentreAllSpriteOrigins();
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

