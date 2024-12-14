#include "3ds/imgui_citro3d.h"
#include "3ds/imgui_ctru.h"
#include "imgui/imgui.h"

#include <cstdio>
#include <cstdlib>
#include <citro3d.h>

#if ANTI_ALIAS
/// \brief Display transfer scaling
constexpr auto TRANSFER_SCALING = GX_TRANSFER_SCALE_XY;
/// \brief Framebuffer scale
constexpr auto FB_SCALE = 2.0f;
#else
/// \brief Display transfer scaling
constexpr auto TRANSFER_SCALING = GX_TRANSFER_SCALE_NO;
/// \brief Framebuffer scale
constexpr auto FB_SCALE = 1.0f;
#endif

/// \brief Top screen render target
C3D_RenderTarget *s_top = nullptr;
/// \brief Bottom screen render target
C3D_RenderTarget *s_bottom = nullptr;

/// \brief Screen width
constexpr auto SCREEN_WIDTH = 400.0f;
/// \brief Screen height
constexpr auto SCREEN_HEIGHT = 480.0f;
/// \brief Framebuffer width
constexpr auto FB_WIDTH = SCREEN_WIDTH * FB_SCALE;
/// \brief Framebuffer height
constexpr auto FB_HEIGHT = SCREEN_HEIGHT * FB_SCALE;

/// \brief Display transfer flags
constexpr auto DISPLAY_TRANSFER_FLAGS =
	GX_TRANSFER_FLIP_VERT (0) | GX_TRANSFER_OUT_TILED (0) | GX_TRANSFER_RAW_COPY (0) |
	GX_TRANSFER_IN_FORMAT (GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT (GX_TRANSFER_FMT_RGB8) |
	GX_TRANSFER_SCALING (TRANSFER_SCALING);

/// \brief Window flags
constexpr auto WINDOW_FLAGS = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

/// \brief Clear color
constexpr auto CLEAR_COLOR = 0x808080FF;

void top_window();
void bottom_window();

int main(int argc_, char *argv_[]) {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// enable New 3DS speedup
	osSetSpeedupEnable (true);

	// init services
	gfxInitDefault();
	gfxSet3D(false);

	// initialize citro3d
	C3D_Init (2 * C3D_DEFAULT_CMDBUF_SIZE);

	// create top screen render target
	s_top = C3D_RenderTargetCreate(FB_HEIGHT * 0.5f, FB_WIDTH, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(s_top, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

	// create bottom screen render target
	s_bottom = C3D_RenderTargetCreate(FB_HEIGHT * 0.5f, FB_WIDTH * 0.8f, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(s_bottom, GFX_BOTTOM, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

	if (!imgui::ctru::init())
		return false;

	imgui::citro3d::init();

	auto &io    = ImGui::GetIO();

	// disable imgui.ini file
	io.IniFilename = nullptr;

	// setup display metrics
	io.DisplaySize = ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT);
	io.DisplayFramebufferScale = ImVec2(FB_SCALE, FB_SCALE);

	while (aptMainLoop()) {

		hidScanInput();

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			return false;

		imgui::ctru::newFrame();
		ImGui::NewFrame();

		top_window();
		bottom_window();

		// render frame
		ImGui::Render();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		// clear frame/depth buffers
		C3D_RenderTargetClear(s_top, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
		C3D_RenderTargetClear(s_bottom, C3D_CLEAR_ALL, CLEAR_COLOR, 0);

		imgui::citro3d::render(s_top, s_bottom);

		C3D_FrameEnd(0);
	}

	// clean up resources
	imgui::citro3d::exit();

	// free render targets
	C3D_RenderTargetDelete(s_bottom);
	C3D_RenderTargetDelete(s_top);

	// deinitialize
	C3D_Fini();
	gfxExit();
	acExit();

	ImGui::DestroyContext();
}

void top_window() {
	ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT * 0.5f));
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Demo Top Screen", NULL, WINDOW_FLAGS)) {
   	    ImGui::End();
   	    return;
   	}

	ImGui::Text("Hello!");

	ImGui::End();
	return;
}

void bottom_window() {
	ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.5f));
	ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.5f), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Demo Bottom Screen", NULL, WINDOW_FLAGS)) {
   	    ImGui::End();
   	    return;
   	}

	ImGui::Button("Hello!");

	ImGui::End();
	return;
}