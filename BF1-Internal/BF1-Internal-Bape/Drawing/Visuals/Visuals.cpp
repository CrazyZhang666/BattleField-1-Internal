/*
* Visuals.cpp : Contains the implementation for the visuals drawing function 
*/

/* Includes for this file */
#include "Visuals.hpp"
#include "../../Global.hpp"
#include "../Functions/Draw.hpp"
#include "../../Vendors/ImGui/imgui.h"
#include "../../Vendors/ImGui/imgui_internal.h"
#include "../../SDK/sdk.hpp"
#include "../../SDK/FrostBite.hpp"
#include <ctime>

namespace Visual
{
	void Draw()
	{
		/* Push a transparent style */
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

		/* Create the window to be drawn on */
		ImGui::Begin("Visuals", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMouseInputs);

		/* Set window pos to the top left of the screen (( really zooted rn cbf think where it is )) */
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);

		/* Set the window size to span the entire screen */
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);

		/* This is the drawing scope just to keep shit looking clean */
		{
			/* Draw an initial cross hair (( Mainly for debugging but also why not )) */
			draw->XCrossHair(global->c_cross_hair);

			/* Attempt to get the local player instance */
			ClientPlayer* local_player = GetLocalPlayer();

			std::string name = local_player->name;

			/* get localtime */
			time_t curr_time;
			curr_time = time(NULL);

			tm* tm_local = localtime(&curr_time);

			/* buffers */
			char timeBuffer[64];
			char hello[64];
			char module[32];

			/* write to buffer */
		    sprintf(timeBuffer, "%i:%i:%i", tm_local->tm_hour, tm_local->tm_min, tm_local->tm_sec);
			sprintf(hello, "Hello %s, Build date: %s, %s", name, __TIME__, __DATE__);
			sprintf(module, "Base: 0x%x", GetModuleHandle(NULL));

			draw->Text(ImVec2(20, 20), ImColor(1.0f, 1.0f, 1.0f, 1.0f), "BF1 Internal - Bape - x64: " + std::string(timeBuffer), NONE);
			draw->Text(ImVec2(20, 35), ImColor(1.0f, 1.0f, 1.0f, 1.0f), std::string(hello), NONE);
			draw->Text(ImVec2(20, 50), ImColor(1.0f, 1.0f, 1.0f, 1.0f), std::string(module), NONE);

			/* Check that local player is valid */
			if (!IsValidPtr(local_player)) return;

			/* Get localplayers soldier */
			ClientSoldierEntity* local_soldier = local_player->clientSoldierEntity;

			/* Check that Client soldier is valid */
			if (!IsValidPtr(local_soldier)) return;

			/* Walk the entirety of the player list */
			for (int i = 0; i <= 64; i++)
			{
				/* Get this iterations player */
				ClientPlayer* player = GetPlayerById(i);

				/* Check that the player is valid */
				if (!IsValidPtr(player)) continue;

				bool team = false; if (player->teamId == local_player->teamId) team = true;

				/* Get this iterations soldier */
				ClientSoldierEntity* soldier = player->clientSoldierEntity;

				/* Check that Client soldier is valid */
				if (!IsValidPtr(soldier)) continue;

				/* Check that this player is valid */
				if (soldier->IsDead()) continue;

				/* World to screen the players position */
				Vec2 base_screen; if (!W2S(soldier->location, base_screen)) continue;

				/* World to screen the players head positon */
				Vec2 head_screen; if (!W2S(soldier->location + Vec3(soldier->GetAABB().max.x, soldier->GetAABB().max.y, soldier->GetAABB().max.z), head_screen)) continue;

				/* Form the information to display */
				char upper_buffer[128]; char lower_buffer[128];

				/* Create the upper text */
				sprintf(upper_buffer, "[%dHP %dM]", (int)soldier->healthcomponent->m_Health, (int)CalculateDistance(local_soldier->location, soldier->location));

				/* Create the lower text */
				sprintf(lower_buffer, "[%s]", player->name);

				draw->Text(ToImVec2(base_screen), ImColor(1.0f, 0.0f, 0.0f), lower_buffer, CENTERED);

				/* draw skeleton */
				DrawSkeleton(soldier, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}

		/* Span the draw to the entire screen */
		ImGui::GetCurrentWindow()->DrawList->PushClipRectFullScreen();

		/* End the overlay window */
		ImGui::End();

		/* Pop the transparent colour we made earlier */
		ImGui::PopStyleColor();
	}
}