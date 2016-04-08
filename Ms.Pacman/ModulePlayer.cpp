#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer::ModulePlayer()
{
	position.x = 208;
	position.y = 439;

	// idle animation (arcade sprite sheet)
	idle.PushBack({ 32, 32, 32, 32}); 


	// Right
	right.PushBack({ 32, 32, 32, 32 });
	right.PushBack({ 64, 32, 32, 32 });
	right.PushBack({ 96, 32, 32, 32 });
	right.speed = 0.2f;

	// Left
	left.PushBack({ 192, 0, 32, 32 });
	left.PushBack({ 224, 0, 32, 32 });
	left.PushBack({ 0, 32, 32, 32 });
	
	left.speed = 0.2f;

	// Up
	up.PushBack({ 0, 0, 32, 32 });
	up.PushBack({ 32, 0, 32, 32 });
	up.PushBack({ 64, 0, 32, 32 });
	up.speed = 0.2f;

	//Down
	down.PushBack({ 98, 0, 32, 32 });
	down.PushBack({ 130, 0, 32, 32 });
	down.PushBack({ 162, 0, 32, 32 });
	down.speed = 0.2f;

}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	graphics = App->textures->Load("characters.png"); // arcade version
	return ret;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	Animation* current_animation = &idle;

	float speed = 2.5;

	if (App->input->keyboard[SDL_SCANCODE_RIGHT] == 1)
	{
		current_animation = &right;
		position.x += speed;
	}

	if (App->input->keyboard[SDL_SCANCODE_LEFT] == 1)
	{
		current_animation = &left;
		position.x -= speed;
	}

	if (App->input->keyboard[SDL_SCANCODE_UP] == 1)
	{
		current_animation = &up;
		position.y -= speed;
	}

	if (App->input->keyboard[SDL_SCANCODE_DOWN] == 1)
	{
		current_animation = &down;
		position.y += speed;
	}

	// Draw everything --------------------------------------
	SDL_Rect r = current_animation->GetCurrentFrame();

	App->render->Blit(graphics, position.x, position.y - r.h, &r);

	return UPDATE_CONTINUE;
}