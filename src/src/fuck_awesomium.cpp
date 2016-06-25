#define DllExport   __declspec( dllexport )

#include <unordered_map>
#include <vector>
#include <string>
#include <cstring>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define OS_WIN
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

#include "enums.h"

#include "string.h"

#include "structs.h"

#include "req_res.h"

#include "js_interface.h"
#include "javascript.h"

#include "session.h"

#include "view.h"

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

		bool ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) OVERRIDE;

		void GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl) OVERRIDE {
			filled = source->GetFilledReq(id);

			if (filled == nullptr) // garry's handers seem to complete immidately, so hopefully this won't be a problem
				panic("Headers not ready. This really needs de-fucked..."); // if it is, we'll need to redesign some shit.

			response_length = filled->len;
			
			if (response_length == 0) {
				delete filled;
				return;
			}

			response->SetMimeType(filled->mime);
			index = 0;
		}

		bool ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback) OVERRIDE {

			int i;
			for (i = 0; i < bytes_to_read; i++) {
				((char*)data_out)[i] = filled->data[index++];
			}

			bytes_read = i;

			if (index == filled->len) {
				delete filled;
			}

			return true;
		}

		void Cancel() OVERRIDE {
			debug_log(__FUNCTION__);
		}
	private:
		DataSource* source;
		FilledReq* filled = 0;
		int id;
		int index;

		IMPLEMENT_REFCOUNTING(GarryResourceHandler);
	};

	class GarrySchemeHandlerFactory : public CefSchemeHandlerFactory
	{
	public:
		CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& scheme_name, CefRefPtr<CefRequest> request) OVERRIDE {
			debug_stream << request->GetURL().ToString() << std::endl;
			return new GarryResourceHandler();
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
			CefDoMessageLoopWork();
		};

		virtual void Log(const WebString& message, int severity, const WebString& file, int line) {
			debug_log(__FUNCTION__"__LINE__");
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
			settings.no_sandbox = true; // define CEF_USE_SANDBOX if you want this?
			settings.remote_debugging_port = 8888;

			//settings.multi_threaded_message_loop = true;

			//CefRefPtr<DumbApp> app_ref(new DumbApp);

			if (!CefInitialize(args, settings, nullptr, nullptr))
				panic("CEF Setup Failed!");

			CefRefPtr<GarrySchemeHandlerFactory> handlerFactory = new GarrySchemeHandlerFactory();
			CefRegisterSchemeHandlerFactory("asset", "", handlerFactory);

			//makeThatWindow();
		};

		static WebCore* instance_;

	public:
		WebSession* session = 0;
	};

	WebCore* WebCore::instance_ = 0;

	///////////////////////////////////////////////

	bool GarryResourceHandler::ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) {
		std::wstring url = request->GetURL();

		int host_start = 8;
		int host_end = url.find_first_of('/', 8);

		std::wstring host = url.substr(host_start, host_end - host_start);

		auto sources = &(WebCore::instance()->session->data_sources);

		auto iter = sources->find(host);

		if (iter != sources->end()) {
			source = iter->second;

			std::wstring path = url.substr(host_end+1);

			// Double slashes can and will crash handlers. I AM AN IDIOT IGNORE THIS.
			/*int pos;
			while ( (pos = path.find(L"//")) != std::string::npos ) {
				path = path.replace(pos,2,L"/");
				debug_log("GOT 1");
			}*/

			id = source->FireReq(*request, path);

			callback->Continue();
			return true;
		}
		else {
			debug_log("################################### did not found source");
			return false;
		}
	}


	///////////////////////////////////////////////


	// These seem somehwat unrelated!
	class DllExport BitmapSurface
	{
	public:
		BitmapSurface(int width, int height) { debug_log("new bitmap surface"); };
		~BitmapSurface() {};

		const unsigned char*buffer() const { return (unsigned char*)"~~~~"; };
		int width() const { return 2; };
		int height() const { return 2; };
		int row_span() const { return 2; };

		void set_is_dirty(bool is_dirty) { };
		bool is_dirty() const { return false; };

		void CopyTo(unsigned char* dst, int dst_row, int dst_depth, bool convert, bool flipy) const {};
		bool SaveToPNG(const Awesomium::WebString* file_path, bool preserve_transparency = false) { return false; };
		bool SaveToJPEG(const Awesomium::WebString* file_path, int quality = 90) { return false; };
		unsigned char GetAlphaAtPoint(int x, int y) const { return 255; };
		void Paint(unsigned char *src, int src_row_span, const void*& src_rect, const void*& dst_rect) {};
		void Scroll(int dx, int dy, const void*& clip_rect);

	private:
		unsigned char* buffer_;
		int width_;
		int height_;
		int row_span_;
		bool is_dirty_;
	};

	void DllExport CopyBuffers(int w, int h, unsigned char* src, int r, unsigned char* dst,
		int dr, int dd, bool rgba, bool flip_y) { debug_log("COPY NEM BUFFERS"); };
}

// Fuck this.
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	return true;
}