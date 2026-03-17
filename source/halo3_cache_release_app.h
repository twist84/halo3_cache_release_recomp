// halo3_cache_release - ReXGlue Recompiled Project
//
// This file is yours to edit. 'rexglue migrate' will NOT overwrite it.
// Customize your app by overriding virtual hooks from rex::ReXApp.

#pragma once

#include <rex/rex_app.h>

#include <rex/filesystem/devices/host_path_device.h>
#include <rex/filesystem/vfs.h>
#include <rex/graphics/flags.h>
#include <rex/input/flags.h>

class Halo3CacheReleaseApp : public rex::ReXApp {
 public:
  using rex::ReXApp::ReXApp;

  static std::unique_ptr<rex::ui::WindowedApp> Create(
      rex::ui::WindowedAppContext& ctx) {
    return std::unique_ptr<Halo3CacheReleaseApp>(new Halo3CacheReleaseApp(ctx, "halo3_cache_release",
        PPCImageConfig));
  }

protected:
	// Override virtual hooks for customization:
	// void OnPreSetup(rex::RuntimeConfig& config) override {}
	// void OnPostSetup() override {}
	// void OnCreateDialogs(rex::ui::ImGuiDrawer* drawer) override {}
	// void OnShutdown() override {}
	// void OnConfigurePaths(rex::PathConfig& paths) override {}

	virtual void OnPreSetup(rex::RuntimeConfig& config) override;
	virtual void OnLoadXexImage(std::string& xex_image) override; // https://github.com/twist84/rexglue-sdk/tree/on-load-xex-image/
	virtual void OnPostSetup() override;
	virtual void OnConfigurePaths(rex::PathConfig& paths) override;
};

// BLAM!

#include "main/main.h"
#include "render/screen_postprocess.h"

void Halo3CacheReleaseApp::OnPreSetup(rex::RuntimeConfig& config)
{
	REXCVAR_SET(gpu_allow_invalid_fetch_constants, true);
	REXCVAR_SET(input_backend, "xinput");
	//REXCVAR_SET(fullscreen, true);
	//REXCVAR_SET(vsync, false);
	//REXCVAR_SET(resolution_scale, 2);
}

void Halo3CacheReleaseApp::OnLoadXexImage(std::string& xex_image)
{
	xex_image = "game:\\halo3_cache_release.xex";
}

void Halo3CacheReleaseApp::OnPostSetup()
{
	{
		c_screen_postprocess::x_settings_internal.m_postprocess = false;
		disable_main_loop_throttle = true;
	}

	rex::Runtime* _runtime = rex::ReXApp::ReXApp::runtime();
	rex::filesystem::VirtualFileSystem* fs = _runtime->file_system();

	auto cache_device = std::make_unique<rex::filesystem::HostPathDevice>(
		"\\CACHE", _runtime->game_data_root(), false);
	if (!cache_device->Initialize())
	{
		REXFS_ERROR("Unable to scan cache path");
	}
	else
	{
		if (!fs->RegisterDevice(std::move(cache_device)))
		{
			REXFS_ERROR("Unable to register cache path");
		}
		else
		{
			fs->RegisterSymbolicLink("cache:", "\\CACHE");
		}
	}

	auto xstorage_device = std::make_unique<rex::filesystem::HostPathDevice>(
		"\\XSTORAGE", _runtime->game_data_root() / "xstorage", false);
	if (!xstorage_device->Initialize())
	{
		REXFS_ERROR("Unable to scan xstorage path");
	}
	else
	{
		if (!fs->RegisterDevice(
			std::move(xstorage_device)))
		{
			REXFS_ERROR("Unable to register xstorage path");
		}
		else
		{
			fs->RegisterSymbolicLink("xstorage:",
				"\\XSTORAGE");
		}
	}
}

void Halo3CacheReleaseApp::OnConfigurePaths(rex::PathConfig& paths)
{
	if (IsDebuggerPresent() == FALSE)
	{
		// for user deployments not a developer debugging
		paths.game_data_root = ".";
		paths.user_data_root = ".";
		paths.update_data_root = ".";
	}
}
