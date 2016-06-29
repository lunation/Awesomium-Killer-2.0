#define DllExport   __declspec( dllexport )

#include <unordered_map>
#include <vector>
#include <string>
#include <cstring>

#include <mutex>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define OS_WIN
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"

#include "include/cef_base.h"
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"

// Debug shit
#include <fstream>

std::ofstream debug_stream;

void debug_log(const char* msg) {
	if (!debug_stream.is_open()) {
		debug_stream.open("C:/Users/Mad-P/Desktop/awesome_log.txt");
		debug_stream << "Logging started!" << std::endl;
	}
	debug_stream << msg << std::endl;
}

void panic(const char* msg) {
	debug_log("PANIC!!!");
	debug_log(msg);
	exit(1);
}

//std::mutex js_call_lock;

#include "enums.h"

#include "string.h"

#include "structs.h"

#include "req_res.h"

#include "js_interface.h"
#include "javascript.h"

#include "session.h"

#include "view.h"

#include "app.h"

namespace Awesomium {

	// Empty - wat?
	class DllExport SurfaceFactory
	{
	public:
	};

	// Meant to be overridden... HURRR
	class DllExport ResourceInterceptor
	{
	public:
		virtual Awesomium::ResourceResponse* OnRequest(void* request) {
			debug_log(__FUNCTION__);
			return 0;
		}
		virtual bool OnFilterNavigation(int origin_process_id, int origin_routing_id, const Awesomium::WebString& method, const Awesomium::WebURL& url, bool is_main_frame) {
			debug_log(__FUNCTION__);
			return false;
		}
		virtual void OnWillDownload(int origin_process_id, int origin_routing_id, const Awesomium::WebURL& url) {
			debug_log(__FUNCTION__);
		}
		virtual ~ResourceInterceptor() {
			debug_log(__FUNCTION__);
		}
	};

	class GarryResourceHandler : public CefResourceHandler {
	public:

		GarryResourceHandler(WebView* owner) {
			this->owner = owner;
		}

		void FillRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback);

		bool ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) OVERRIDE {
			CefPostTask(TID_UI, base::Bind(&GarryResourceHandler::FillRequest, this, request, callback));
			return true;
		}

		void GetResponseHeaders(CefRefPtr<CefResponse> res, int64& res_len, CefString& redirectUrl) OVERRIDE {

			res_len = response.len;
			
			if (res_len == 0) {
				res->SetStatus(400);
				return;
			}

			res->SetStatus(200);
			res->SetMimeType(response.mime);
			index = 0;
		}

		bool ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback) OVERRIDE {
			int i;
			for (i = 0; i < bytes_to_read; i++) {
				((char*)data_out)[i] = response.data[index++];
			}

			bytes_read = i;

			return true;
		}

		void Cancel() OVERRIDE {
			debug_log(__FUNCTION__);
		}
	private:
		//DataSource* source = 0;
		SimpleResponse response;
		int index = 0;
		WebView* owner;

		IMPLEMENT_REFCOUNTING(GarryResourceHandler);
	};

	class GarrySchemeHandlerFactory : public CefSchemeHandlerFactory
	{
	public:
		CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& scheme_name, CefRefPtr<CefRequest> request) OVERRIDE {
			return new GarryResourceHandler(browser_map[browser->GetIdentifier()]);
		}
	private:
		IMPLEMENT_REFCOUNTING(GarrySchemeHandlerFactory);
	};


	/*class GarryApp : public CefApp {
	public:
		GarryApp() { };

		void OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar) OVERRIDE {
			registrar->AddCustomScheme("asset", true, true, true);
		}

		// CefBrowserProcessHandler methods:
		//rtual void OnRegisterCustomSchemes() OVERRIDE;

	private:
		// Include the default reference counting implementation.
		IMPLEMENT_REFCOUNTING(GarryApp);
	};*/


	// Very important singleton!
	class DllExport WebCore
	{
	public:
		static WebCore* Initialize(const WebConfig& config)  {
			// We don't ACTUALLY have to deal with the config,
			// we'll whip up our own config for CEF. Relevant details are:

			// USERAGENT = Mozilla/5.0 (Windows; Valve Source Client) AppleWebKit/535.19 (KHTML, like Gecko) Chrome/18.0.1003.1 Safari/535.19 Awesomium/1.7.5.1 GMod/13
			// PARAMS = --allow-file-access-from-files

			if (instance_ != nullptr)
				panic("Initialized WebCore twice!");

			instance_ = new WebCore();

			//js_call_lock.lock();

			return instance_;
		};
		static WebCore* instance() {
			return instance_;
		};
		static void Shutdown() {
			CefShutdown();
		};


		//////////////////////////


		virtual WebSession* CreateWebSession(const WebString& path, const WebPreferences& prefs) {
			// ignore preferences, assume singleton
			debug_log("=====================================NEW SESSION===================================");
			if (!session)
				session = new WebSession;

			return session;
		};

		virtual WebView* CreateWebView(int width, int height, WebSession* session = 0, int type = 0) {

			WebView* webview = new WebView(session);
			webview->Resize(width, height);

			return webview;
		};


		virtual void set_surface_factory(SurfaceFactory* factory) {
			debug_log(__FUNCTION__"__LINE__");
		};
		virtual SurfaceFactory* surface_factory() const {
			debug_log(__FUNCTION__"__LINE__");
			return new SurfaceFactory();
		};


		virtual void set_resource_interceptor(ResourceInterceptor* interceptor) {
			debug_log(__FUNCTION__"__LINE__");
		};
		virtual ResourceInterceptor* resource_interceptor() const {
			debug_log(__FUNCTION__"__LINE__");
			return new ResourceInterceptor();
		};

		virtual void Update() {
			//debug_log("Update - Pre Unlock");
			//js_call_lock.unlock();
			//debug_log("Update - Post Unlock");
			CefDoMessageLoopWork();
			//debug_log("Update - Pre Lock");
			//js_call_lock.lock();
			//debug_log("Update - Post Lock");
		};

		virtual void Log(const WebString& message, int severity, const WebString& file, int line) {
			debug_log(__FUNCTION__);
		};
		virtual const char* version_string() const {
			debug_log(__FUNCTION__"__LINE__");
			return "6969__FUCK_GARRY__MARRY_CEF__KILL_AWESOMIUM";
		};


		static unsigned int used_memory() { debug_log(__FUNCTION__"__LINE__"); return 0; } // :-) If only it was this easy!
		static unsigned int allocated() { debug_log(__FUNCTION__"__LINE__"); return 128; } // :-) If only it was this easy!
		static void release_memory() { debug_log(__FUNCTION__"__LINE__"); };

		virtual ~WebCore() { debug_log(__FUNCTION__"__LINE__"); }

	private:
		WebCore() {
			CefMainArgs args;

			CefSettings settings;
			settings.command_line_args_disabled = true;
			CefString(&settings.browser_subprocess_path).FromASCII("gmod_cef.exe");
			CefString(&settings.user_agent).FromASCII("Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.103 Safari/537.36 GMod/13 (CEF, Valve Source Client)");
			
			settings.no_sandbox = true; // define CEF_USE_SANDBOX if you want this? Do we NEED this? Might ben necessary for vista.
			settings.remote_debugging_port = 8888;
			settings.windowless_rendering_enabled = true;


			//settings.multi_threaded_message_loop = true;

			//CefRefPtr<DumbApp> app_ref(new DumbApp);

			if (!CefInitialize(args, settings, new GarryApp(), nullptr))
				panic("CEF Setup Failed!");

			CefRefPtr<GarrySchemeHandlerFactory> garryFactory = new GarrySchemeHandlerFactory();
			CefRegisterSchemeHandlerFactory("asset", "", garryFactory);

			//makeThatWindow();
		};

		static WebCore* instance_;

	public:
		WebSession* session = 0;
	};

	WebCore* WebCore::instance_ = 0;

	///////////////////////////////////////////////

	void GarryResourceHandler::FillRequest(CefRefPtr<CefRequest> req, CefRefPtr<CefCallback> callback) {
		debug_stream << "HANDLE REQ " << req->GetURL().ToString() << std::endl;

		std::wstring url = req->GetURL();

		int host_start = 8;
		int host_end = url.find_first_of('/', 8);

		CefString host = url.substr(host_start, host_end - host_start);

		CefString path = url.substr(host_end + 1);

		auto sources = &(WebCore::instance()->session->data_sources);

		auto iter = sources->find(host);
		
		if (iter != sources->end()) {
			DataSource* source = iter->second;

			source->ReqSync(owner, *req, path, &response);

			callback->Continue();
		}
		else {
			//panic("################################### did not find source");
			callback->Cancel();
		}
	}


	///////////////////////////////////////////////


	void DllExport CopyBuffers(int w, int h, unsigned char* src, int r, unsigned char* dst,
		int dr, int dd, bool rgba, bool flip_y) { debug_log("COPY NEM BUFFERS"); };
}

// Fuck this.
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	return true;
}