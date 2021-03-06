#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleStartScreen.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"
#include "ModuleFadeToBlack.h"
#include "ModuleLevelOne.h"
#include "ModuleEnemies.h"
#include "ModuleBlinky.h"
#include "ModuleAudio.h"
#include "ModuleReady.h"
#include "ModuleLevelTwo.h"
#include "ModuleLevelThree.h"



ModuleReady::ModuleReady()
{
	ground = { 0, 0, 448, 576 };

}

ModuleReady::~ModuleReady()
{}

// Load assets
bool ModuleReady::Start()
{
	time = SDL_GetTicks();
	graphics = App->textures->Load("Game/P1Ready.png");
	App->audio->PlayMusic("Game/game-start.ogg", 1.0f);

	App->player->Disable();
	App->blinky->Disable();
	return true;
}

// UnLoad assets
bool ModuleReady::CleanUp()
{
	LOG("Unloading gameover scene");
	App->audio->Disable();
	return true;
}

// Update: draw background
update_status ModuleReady::Update()
{
	App->render->Blit(graphics, 0, 0, &ground);


	//FADE TO BLACK
	//if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN){
	if (SDL_GetTicks() - time > 3500){

		App->ready->Disable();
		App->level_one->Enable();
		//App->fade->FadeToBlack(this, App->level_one, 1);

	}


	return UPDATE_CONTINUE;
}