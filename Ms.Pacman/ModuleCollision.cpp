#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModulePlayer.h"
#include "ModuleBlinky.h"
#include "ModulePinky.h"
#include "ModuleSue.h"
#include "ModuleInky.h"

ModuleCollision::ModuleCollision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_PELLET] = false;
	matrix[COLLIDER_WALL][COLLIDER_POWERPELLET] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PINKY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_INKY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_BLINKY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_SUE] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PELLET] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_POWERPELLET] = true;

	matrix[COLLIDER_PINKY][COLLIDER_WALL] = false;
	matrix[COLLIDER_PINKY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_PINKY][COLLIDER_PINKY] = false;
	matrix[COLLIDER_PINKY][COLLIDER_PELLET] = false;
	matrix[COLLIDER_PINKY][COLLIDER_POWERPELLET] = false;

	matrix[COLLIDER_INKY][COLLIDER_WALL] = false;
	matrix[COLLIDER_INKY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_INKY][COLLIDER_INKY] = false;
	matrix[COLLIDER_INKY][COLLIDER_PELLET] = false;
	matrix[COLLIDER_INKY][COLLIDER_POWERPELLET] = false;

	matrix[COLLIDER_BLINKY][COLLIDER_WALL] = false;
	matrix[COLLIDER_BLINKY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_BLINKY][COLLIDER_BLINKY] = false;
	matrix[COLLIDER_BLINKY][COLLIDER_PELLET] = false;
	matrix[COLLIDER_BLINKY][COLLIDER_POWERPELLET] = false;

	matrix[COLLIDER_SUE][COLLIDER_WALL] = false;
	matrix[COLLIDER_SUE][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_SUE][COLLIDER_SUE] = false;
	matrix[COLLIDER_SUE][COLLIDER_PELLET] = false;
	matrix[COLLIDER_SUE][COLLIDER_POWERPELLET] = false;
	
	matrix[COLLIDER_PELLET][COLLIDER_WALL] = false;
	matrix[COLLIDER_PELLET][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_PELLET][COLLIDER_PINKY] = false;
	matrix[COLLIDER_PELLET][COLLIDER_INKY] = false;
	matrix[COLLIDER_PELLET][COLLIDER_BLINKY] = false;
	matrix[COLLIDER_PELLET][COLLIDER_SUE] = false;
	matrix[COLLIDER_PELLET][COLLIDER_PELLET] = false;
	matrix[COLLIDER_PELLET][COLLIDER_POWERPELLET] = false;

	matrix[COLLIDER_POWERPELLET][COLLIDER_WALL] = false;
	matrix[COLLIDER_POWERPELLET][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_POWERPELLET][COLLIDER_PINKY] = false;
	matrix[COLLIDER_POWERPELLET][COLLIDER_INKY] = false;
	matrix[COLLIDER_POWERPELLET][COLLIDER_SUE] = false;
	matrix[COLLIDER_POWERPELLET][COLLIDER_BLINKY] = false;
	matrix[COLLIDER_POWERPELLET][COLLIDER_PELLET] = false;
	matrix[COLLIDER_POWERPELLET][COLLIDER_POWERPELLET] = false;
}

// Destructor
ModuleCollision::~ModuleCollision()
{}

update_status ModuleCollision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return UPDATE_CONTINUE;
}

// Called before render is available
update_status ModuleCollision::Update()
{
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}

	DebugDraw();

	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()
{
	if (App->input->keyboard[SDL_SCANCODE_F1] == KEY_DOWN)
		debug = !debug;

	if (App->input->keyboard[SDL_SCANCODE_F2] == KEY_DOWN)
		App->player->god = !App->player->god;

	if (App->input->keyboard[SDL_SCANCODE_F3] == KEY_DOWN){
		App->inky->debug = !App->inky->debug;
		App->blinky->debug = !App->blinky->debug;
		App->pinky->debug = !App->pinky->debug;
		App->sue->debug = !App->sue->debug;
	}

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_WALL: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_PINKY: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_BLINKY: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_INKY: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_SUE: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_PELLET: // yellow
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		case COLLIDER_POWERPELLET: // magenta
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 255, alpha);
			break;
		}
	}
}

// Called before quitting
bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* ModuleCollision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

bool ModuleCollision::EraseCollider(Collider* collider)
{
	if (collider != nullptr)
	{
		// we still search for it in case we received a dangling pointer
		for (uint i = 0; i < MAX_COLLIDERS; ++i)
		{
			if (colliders[i] == collider)
			{
				collider->to_delete = true;
				break;
			}
		}
	}


	return false;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}