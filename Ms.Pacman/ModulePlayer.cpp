#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer::ModulePlayer()
{
	
	position.x = 208;
	position.y = 407;
	// idle animation (arcade sprite sheet)
	idle.PushBack({ 32, 32, 32, 32}); 


	// Right
	right.PushBack({ 32, 32, 32, 32 });
	right.PushBack({ 64, 32, 32, 32 });
	right.PushBack({ 96, 32, 32, 32 });
	right.speed = 0.15f;

	// Left
	left.PushBack({ 192, 0, 32, 32 });
	left.PushBack({ 224, 0, 32, 32 });
	left.PushBack({ 0, 32, 32, 32 });
	
	left.speed = 0.15f;

	// Up
	up.PushBack({ 0, 0, 32, 32 });
	up.PushBack({ 32, 0, 32, 32 });
	up.PushBack({ 64, 0, 32, 32 });
	up.speed = 0.15f;

	//Down
	down.PushBack({ 98, 0, 32, 32 });
	down.PushBack({ 130, 0, 32, 32 });
	down.PushBack({ 162, 0, 32, 32 });
	down.speed = 0.15f;

}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{

	
	LOG("Loading player textures");
	bool ret = true;
	graphics = App->textures->Load("characters.png"); // arcade version
	

	col = App->collision->AddCollider({ position.x, position.y, 32, 32 }, COLLIDER_PLAYER, this);
	return ret;
}
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	//App->textures->Unload(graphics);
	App->collision->EraseCollider(col);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	Animation* current_animation = &idle;

	float speed = 3;
	static int direction = -1;
	if (App->input->keyboard[SDL_SCANCODE_RIGHT] == 1)
	{
		direction = 0;
		//current_animation = &right;
		//position.x += speed;
	}
	if (direction == 0){
		current_animation = &right;
		position.x += speed;
				
	}
	
	
	if (App->input->keyboard[SDL_SCANCODE_LEFT] == 1)
	{
		direction = 1;
		//current_animation = &left;
		//position.x -= speed;
	}
	if(direction == 1){
		current_animation = &left;
		position.x -= speed;
	}

	if (App->input->keyboard[SDL_SCANCODE_UP] == 1)
	{
		direction = 2;
		//current_animation = &up;
		//position.y -= speed;
	}
	if (direction == 2){
		current_animation = &up;
		position.y -= speed;
	}

	if (App->input->keyboard[SDL_SCANCODE_DOWN] == 1)
	{
		direction = 3;
		//current_animation = &down;
		//position.y += speed;
	}
	if(direction == 3){
		current_animation = &down;
		position.y += speed;
	}

	//Position max
	
	if (position.y >= 503){
		position.y = 503;
	}
	if (position.y <= 56){
		position.y = 56;
	}
	if (position.x <= 9){
		position.x = 9;
	}
	if (position.x >= 409){
		position.x = 409;
	}

	// Collider--------------

col->SetPos(position.x, position.y);
	if (destroyed == false)
		App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));


	// Draw everything --------------------------------------
	SDL_Rect r = current_animation->GetCurrentFrame();

	//App->render->Blit(graphics, position.x, position.y - r.h, &r);

	return UPDATE_CONTINUE;
}


void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	bool colision = false;
	if (c1 == col && destroyed == false )
	{
		colision = true;

		//destroyed = true;
	}
}