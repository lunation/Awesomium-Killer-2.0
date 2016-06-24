#pragma once

// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <string>

#include "include/cef_client.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"

#include "DumbHandler.h"

// Implement application-level callbacks for the browser process.
class DumbApp : public CefApp,
	public CefBrowserProcessHandler {
public:
	DumbApp() {};

	// CefApp methods:
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
		OVERRIDE {
		return this;
	}

	// CefBrowserProcessHandler methods:
	virtual void OnContextInitialized() OVERRIDE;

private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(DumbApp);
};

void makeThatWindow() {
	// Information used when creating the native window.
	CefWindowInfo window_info;

#if defined(OS_WIN)
	// On Windows we need to specify certain flags that will be passed to
	// CreateWindowEx().
	window_info.SetAsPopup(NULL, "Not gonna happen, huh?");
#endif

	CefRefPtr<DumbHandler> handler(new DumbHandler());

	std::string url = "http://www.google.com";

	CefBrowserSettings browser_settings;

	CefBrowserHost::CreateBrowser(window_info, nullptr, url, browser_settings, NULL);
}

void DumbApp::OnContextInitialized() {
	CEF_REQUIRE_UI_THREAD();

	makeThatWindow();
}