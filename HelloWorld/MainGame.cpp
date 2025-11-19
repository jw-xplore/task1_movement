#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "EntityManager.h"
#include "Agent.h"
#include "Path.h"
#include "CollisionHandler.h"
#include "CoordinatedMovement.h"

EntityManager* entityManager;
Agent* agent;
Agent* agent2;
Agent* agent3;
Path* path;
SteeringBehavior* steeringBehavior;
CollisionHandler* collisionHandler;

CoordinatedMovement* group;

ESteeringBehavior steeringType = ESteeringBehavior::Seek;

// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );

	// Setup entity manager and entities
	entityManager = new EntityManager();
	steeringBehavior = new SteeringBehavior();
	collisionHandler = new CollisionHandler();

	// Setup path
	//path = new Path({ { 150, 40 }, { 280, 100 }, { 400, 50 }, { 600, 200 }, { 400, 250 }, { 300, 300} });

	path = new Path({ { 100, 100 }, { 400, 100 }, { 400, 300 }, { 100, 300 }, { 100, 150 } });

	agent = new Agent({100, 100}, steeringBehavior, collisionHandler);
	agent->walls.insert(agent->walls.end(), path);
	agent->followPath = path;

	agent2 = new Agent({ 200, 20 }, steeringBehavior, collisionHandler);
	agent2->walls.insert(agent2->walls.end(), path);
	agent2->followPath = path;

	agent3 = new Agent({ 250, 50 }, steeringBehavior, collisionHandler);
	agent3->walls.insert(agent3->walls.end(), path);
	agent3->followPath = path;

	entityManager->AddEntity(agent);
	entityManager->AddEntity(agent2);
	entityManager->AddEntity(agent3);

	Play::CentreAllSpriteOrigins();

	// Setup group
	group = new CoordinatedMovement();
	group->leader = agent;
	group->members.insert(group->members.end(), agent2);
	group->members.insert(group->members.end(), agent3);
}

void BehaviorSwitch()
{
	// Change controls
	if (Play::KeyPressed(Play::KeyboardButton::KEY_RIGHT))
	{
		int val = steeringType;

		if (steeringType < ESteeringBehavior::SteeringBehaviorCount - 1)
			val++;
		else
			val = 0;

		steeringType = static_cast<ESteeringBehavior>(val);
	}
	else if (Play::KeyPressed(Play::KeyboardButton::KEY_LEFT))
	{
		int val = steeringType;

		if (steeringType > ESteeringBehavior::Seek)
			val--;
		else
			val = ESteeringBehavior::SteeringBehaviorCount - 1;

		steeringType = static_cast<ESteeringBehavior>(val);
	}

	// Apply to agents
	agent->steeringType = steeringType;
	agent2->steeringType = steeringType;
	agent3->steeringType = steeringType;

	// Show UI
	std::string tmp = "Type: " + std::to_string(steeringType);
	char const* txt = tmp.c_str();
	Play::DrawDebugText({ 40,40 }, txt);
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	Play::ClearDrawingBuffer( Play::cBlack );
	entityManager->UpdateEntities(elapsedTime);

	BehaviorSwitch();

	// Coordinated movement
	agent->target->position = Play::GetMousePos();
	agent2->target->position = group->memberPosition(agent2);
	agent3->target->position = group->memberPosition(agent3);

	path->DrawPath();

	Play::PresentDrawingBuffer();
	return Play::KeyDown( KEY_ESCAPE );
}

// Gets called once when the player quits the game 
int MainGameExit( void )
{
	Play::DestroyManager();
	return PLAY_OK;
}

