#define DllExport   __declspec( dllexport )

#include <vector>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define OS_WIN
#include "include/cef_base.h"
#include "include/cef_app.h"
#include "include/cef_browser.h"

#include "DumbApp.h" // TODO REMOVE

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

// Some constants which are totally not fucking dumb
char* ver = "1337_NOAWESOMIUMFUCKER";

char* holycow = "WOWHOLYCOW";

char* shitbuf = "COCK";

namespace Awesomium
{
	// Enums

	enum LogLevel {
		kLogLevel_None = 0,  ///< No log is created
		kLogLevel_Normal,    ///< Logs only errors
		kLogLevel_Verbose,   ///< Logs everything
	};

	enum Error {
		kError_None = 0,
		kError_BadParameters,
		kError_ObjectGone,
		kError_ConnectionGone,
		kError_TimedOut,
		kError_WebViewGone,
		kError_Generic,
	};

	enum JSObjectType {
		kJSObjectType_Local,
		kJSObjectType_Remote,
		kJSObjectType_RemoteGlobal,
	};


	//class JSValue;

	namespace WebViewListener
	{

		class DllExport Load
		{
		public:
			virtual ~Load() { debug_log("DEL WEBVIEWLISTENER.LOAD"); };
		};

		class DllExport View
		{
		public:
			virtual ~View() { debug_log("DEL WEBVIEWLISTENER.VIEW"); };
		};

	};

}

#include "webstring.h"

namespace Awesomium
{
	#pragma pack(push,1)

	struct DllExport WebConfig
	{
	public:
		WebConfig() {};

		LogLevel log_level;						// 0
		WebString package_path;					// 4
		WebString plugin_path;					// 8
		WebString log_path;						// 12
		WebString child_process_path;			// 16
		WebString user_agent;					// 20
		int remote_debugging_port = 0;			// 24
		WebString remote_debugging_host;		// 28
		bool reduce_memory_usage_on_navigation = false;	// 32
		WebString user_script;					// 36?
		WebString user_stylesheet;				// 40?
		WebString asset_protocol;				// 44?
		WebStringArray additional_options;		// 48? (Doesn't really matter...)
	};

	struct DllExport WebPreferences
	{
		WebPreferences() { };
		int max_http_cache_storage;
		bool enable_javascript;
		bool enable_dart;
		bool enable_plugins;
		bool enable_local_storage;
		bool enable_databases;
		bool enable_app_cache;
		bool enable_web_audio;
		bool enable_web_gl;
		bool enable_web_security;
		bool enable_remote_fonts;
		bool enable_smooth_scrolling;
		bool enable_gpu_acceleration;
		WebString user_stylesheet;
		WebString user_script;
		WebString proxy_config;
		WebString accept_language;
		WebString accept_charset;
		WebString default_encoding;
		bool shrink_standalone_images_to_fit;
		bool load_images_automatically;
		bool allow_scripts_to_open_windows;
		bool allow_scripts_to_close_windows;
		bool allow_scripts_to_access_clipboard;
		bool allow_universal_access_from_file_url;
		bool allow_file_access_from_file_url;
		bool allow_running_insecure_content;
	};

	#pragma pack(pop)


	class WebSession;
	class ResourceRequest;

	class DllExport DataSource
	{
	public:
		virtual ~DataSource() { };
		virtual void OnRequest(int id, const ResourceRequest& request, const WebString& path) {};
		void SendResponse(int id, unsigned int buf, unsigned char*buff, const WebString& meme) {};
		void SendResponse(int id, unsigned int buf, unsigned char const * buff, class Awesomium::WebString const &meme) {};

	protected:
		DataSource() {};
		void set_session(WebSession*session, int data_source_id) {};
		WebSession*session_;
		int data_source_id;
		friend class WebSessionImpl;
	};

	// There are functions to dynamically create these! Assume they need to take care of deleting them?
	class DllExport ResourceResponse {
	public:
		ResourceResponse() { debug_log("NEW RESRESPONSE"); };
		static ResourceResponse* Create(unsigned int num_bytes, unsigned char* buffer, const WebString& mime_type) { debug_log("CREATE RESRESPONSE 1"); return new ResourceResponse(); };
		static ResourceResponse* Create(const WebString& file_path) { debug_log("CREATE RESRESPONSE 2"); return new ResourceResponse(); };
	protected:
		ResourceResponse(unsigned int num_bytes, unsigned char* buffer, const WebString& mime_type) {};
		ResourceResponse(const WebString& file_path) {};
		~ResourceResponse() {};
		unsigned int num_bytes_;
		unsigned char* buffer_;
		WebString mime_type_;
		WebString file_path_;
		friend class WebCoreImpl;
	};

	class DllExport UploadElement {
	public:
		virtual bool IsFilePath() const { return false; };
		virtual bool IsBytes() { return false; };
		virtual unsigned int num_bytes() { return 1; };
		virtual const unsigned char* bytes() { return 0; };
		virtual WebString file_path() { return path; };
	protected:
		virtual ~UploadElement() {}
	private:
		WebString path;
	};

	class DllExport WebURL
	{
	public:
		WebURL() { debug_log("NEW WEBURL"); };
		explicit WebURL(const WebString&str) {
			debug_stream << "URL: " << str << std::endl;
			//url_string = str;
		};
		WebURL(const WebURL&rhs) {};

		~WebURL() {};

		WebURL& operator=(const WebURL&rhs) { return *this; };

		bool IsValid() const { return true; };
		bool IsEmpty() const { return false; };
		WebString spec() const { return WebString(); };
		WebString scheme() const { return WebString(); };
		WebString username() const { return WebString(); };
		WebString password() const { return WebString(); };
		WebString host() const { return WebString(); };
		WebString port() const { return WebString(); };
		WebString path() const { return WebString(); };
		WebString query() const { return WebString(); };
		WebString anchor() const { return WebString(); };
		WebString filename() const { return WebString(); };

		bool operator==(const WebURL& other) const { return false; };
		bool operator!=(const WebURL& other) const { return false; };
		bool operator<(const WebURL& other) const { return false; };

	private:
		explicit WebURL(const void* internal_instance);
		void* instance_;
		friend class InternalHelper;

		//WebString url_string;
	};

	// HOW ARE THESE MADE?
	class DllExport ResourceRequest {
	public:
		virtual void Cancel() {};
		virtual int origin_process_id() { return 1; };
		virtual int origin_routing_id() { return 1; };
		virtual WebURL url() { return WebURL(); };
		virtual WebString method() { return WebString(); };
		virtual void set_method(const WebString& method) {};
		virtual WebString referrer() const { return WebString(); };
		virtual void set_referrer(const WebString& referrer) {};
		virtual WebString extra_headers() const { return WebString(); };
		virtual void set_extra_headers(const WebString& headers) {};
		virtual void AppendExtraHeader(const WebString& name, const WebString& value) {};
		virtual unsigned int num_upload_elements() const { return 0; };
		virtual const UploadElement* GetUploadElement(unsigned int idx) const { return nullptr; }; // <=========================== RETURNING NULLPTR
		virtual void ClearUploadElements() {};
		virtual void AppendUploadFilePath(const WebString& path) {};
		virtual void AppendUploadBytes(const char* bytes, unsigned int num_bytes) {};
		virtual void set_ignore_data_source_handler(bool ignore) {};

	protected:
		virtual ~ResourceRequest() {}
	};

	class DllExport WebSession
	{
	public:
		virtual void Release() const {};
		virtual bool IsOnDisk() const { return false; };
		virtual WebString data_path() const { return WebString(); };
		virtual const WebPreferences& preferences() const { return *(new WebPreferences()); }; // VERY NOT GOOD!!! <======================================
		virtual void AddDataSource(const WebString& asset_host, DataSource* source) {};
		virtual void SetCookie(const WebURL& url, const WebString& cookie_string, bool is_http_only, bool force_session_cookie) {};
		virtual void ClearCookies() {};
		virtual void ClearCache() {};
		virtual int GetZoomForURL(const WebURL& url) { return 0; };
	protected:
		virtual ~WebSession() {}
	};

	typedef int WebViewType;
	typedef HANDLE ProcessHandle;
	typedef HWND NativeWindow;

	ProcessHandle PROC_HANDLE;
	NativeWindow WINDOW;

	class WebView;

	class JSObject;
	class JSArray;

	class DllExport JSValue
	{
	public:
		JSValue() { debug_log("NEW JSVALUE"); };
		explicit JSValue(bool val) { debug_log("NEW JSVALUE2"); };
		explicit JSValue(int val) { debug_log("NEW JSVALUE3"); };
		explicit JSValue(double val) { debug_log("NEW JSVALUE4"); };
		JSValue(const WebString& val) { debug_log("COPY JSVALUE"); };
		JSValue(const JSObject& val) { debug_log("COPY JSVALUE2"); };
		JSValue(const JSArray &val) { debug_log("COPY JSVALUE3"); };
		JSValue(const JSValue& orig) { debug_log("COPY JSVALUE4"); };

		~JSValue() {};

		JSValue& operator=(const JSValue&rhs) { return *this; };

		// Not sure wtf I implemented these, going to tunnel through CEF anyway...
		static const JSValue& Undefined()  {
			static JSValue* undef = nullptr;
			if (undef == nullptr) {
				undef = new JSValue();
			}
			return *undef;
		};
		static const JSValue& Null() {
			static JSValue* _null = nullptr;
			if (_null == nullptr) {
				_null = new JSValue();
			}
			return *_null;
		};

		bool IsBoolean() const { return true; };
		bool IsInteger() const { return true; };
		bool IsDouble() const { return true; };
		bool IsNumber() const { return true; };
		bool IsString() const { return true; };
		bool IsArray() const { return true; };
		bool IsObject() const { return true; };
		bool IsNull() const { return false; };
		bool IsUndefined() const { return false; };

		WebString ToString() const { return WebString(); };
		int ToInteger() const { return 1; };
		double ToDouble() const { return 1; };
		bool ToBoolean() const { return true; };
		
		JSArray& ToArray();
		const JSArray& ToArray() const;

		JSObject& ToObject();
		const JSObject& ToObject() const;
	};

	class DllExport JSArray
	{
	public:
		explicit JSArray() { debug_log("NEW JSARRAY"); };
		JSArray(unsigned int n) { debug_log("NEW JSARRAY2"); };
		JSArray(const JSArray &rhs) { debug_log("COPY JSARRAY"); };
		JSArray & operator=(const JSArray&rhs) {
			return *this;
		};

		unsigned int size() const { return 1; };
		unsigned int capacity() const { return 1; };
		JSValue & At(unsigned int idx) { return value; };
		const JSValue & At(unsigned int idx) const { return value; };

		JSValue & operator[](unsigned int idx) {
			return value;
		};

		const JSValue & operator[](unsigned int idx) const {
			return value;
		}

		void Push(const JSValue&item) {};
		void Pop() {};
		void Insert(const JSValue&item, unsigned int idx) {};
		void Erase(unsigned int idx) {};
		void Clear() {};

	protected:
		WebVector<JSValue>* vector_;
		JSValue value;
	};

	class DllExport JSObject
	{
	public:
		JSObject() { debug_log("NEW JSOBJ"); };
		JSObject(const JSObject &obj) { debug_log("COPY JSOBJ"); };
		JSObject &operator=(const JSObject&rhs) { return *this; };
		unsigned int remote_id() const { return 0; };
		int ref_count() const { return 0; };

		JSObjectType type() const { return JSObjectType::kJSObjectType_Local; };
		WebView* owner() const { return nullptr; };

		JSArray GetPropertyName() const { return *(new JSArray()); }; // DUMB!!!
		bool HasProperty(const WebString& name) const { return false; }
		JSValue GetProperty(const WebString& name) const { return *(new JSValue()); }; // DUMB
		void SetProperty(const WebString& name, const JSValue& value) {
			debug_log("setprop");
		};
		void SetPropertyAsync(const WebString& name, const JSValue& value) {
			debug_log("setpropa");
		};
		void RemoveProperty(const WebString& name) {};
		JSArray GetMethodNames() const { return *(new JSArray()); }; // DUMB
		bool HasMethod(const WebString& name) const { return false; };
		JSValue Invoke(const WebString& name, const JSArray& args) { return *(new JSValue()); }; // DUMB
		void InvokeAsync(const WebString& name, const JSArray& args) {};
		WebString ToString() const { return WebString(); };


		void SetCustomMethod(const WebString& name, bool has_return_value) {};

		Error last_error() const;
	};

	// IMPL -- SUPER DUMB!

	JSObject& JSValue::ToObject() {
		return *(new JSObject());
	}

	const JSObject& JSValue::ToObject() const {
		return *(new JSObject());
	}

	JSArray& JSValue::ToArray() {
		return *(new JSArray());
	}
	
	const JSArray& JSValue::ToArray() const {
		return *(new JSArray());
	};


	class DllExport Surface
	{
	public:
		virtual ~Surface() {};
		virtual void Paint(unsigned  char*shit, int span, void*&srcrect, void*&destrect) { memset(shit, 1, span); };
		virtual void Scroll(unsigned  char*shit, int span, void*&srcrect, void*&destrect) { memset(shit, 1, span); };

	};

	WebViewListener::View view;
	WebViewListener::Load load;

	// Uhhh?
	class DllExport WebKeyboardEvent
	{
	public:
		WebKeyboardEvent() { debug_log("NEW KBEVENT"); };

	};

	// A goddamn window
	class DllExport WebView
	{
	public:
		virtual void Destroy() { };
		virtual WebViewType type() { return type_; };
		virtual int process_id() { return 1; };
		virtual int routing_id() { return 1; };
		virtual int next_routing_id() { return 1; };
		virtual ProcessHandle process_handle() { return PROC_HANDLE; };
		virtual void set_parent_window(NativeWindow parent) { };
		virtual NativeWindow parent_window() { return WINDOW; };
		virtual NativeWindow window() { return WINDOW; };
		virtual void set_view_listener(WebViewListener::View* listener) { };
		virtual void set_load_listener(WebViewListener::Load* listener) { };
		virtual void set_process_listener(void* listener) { proclist = listener; };
		virtual void set_menu_listener(void* listener) { menulist = listener; };
		virtual void set_dialog_listener(void* listener) { dialoglist = listener; };
		virtual void set_print_listener(void* listener) { printlist = listener; };
		virtual void set_download_listener(void* listener) { downloadlist = listener; };
		virtual void set_input_method_editor_listener(void* listener) { inputlist = listener; };
		virtual WebViewListener::View* view_listener() { return &view; };
		virtual WebViewListener::Load* load_listener() { return &load; };
		virtual void* process_listener() {
			return proclist;
		};
		virtual void* menu_listener() {
			return menulist;
		};
		virtual void* dialog_listener() {
			return dialoglist;
		};
		virtual void* print_listener() {
			return printlist;
		};
		virtual void* download_listener() {
			return downloadlist;
		};
		virtual void* input_method_editor_listener() {
			return inputlist;
		};
		virtual void LoadURL(const WebURL& url) {
			debug_log("load url!");
			//debug_stream << url << endl;
		};
		virtual void GoBack() { };
		virtual void GoForward() { };
		virtual void GoToHistoryOffset(int offset) { };
		virtual void Stop() { };
		virtual void Reload(bool ignore_cache) { };
		virtual bool CanGoBack() { return true; };
		virtual bool CanGoForward() { return true; };
		virtual Surface* surface() {
			return 0;//oursurface;
		};
		virtual WebURL url() { return *(new WebURL()); };
		virtual WebString title() { return WebString(); };
		virtual WebSession* session() { return session_; }
		virtual bool IsLoading() { return 1; };
		virtual bool IsCrashed() { return 0; };
		virtual void Resize(int width, int height) {
			debug_log("RESIZE");
			debug_stream << width << ", " << height << std::endl;
		};
		virtual void SetTransparent(bool is_transparent) { };
		virtual bool IsTransparent() { return false; };
		virtual void PauseRendering() { };
		virtual void ResumeRendering() { };
		virtual void Focus() { };
		virtual void Unfocus() { };
		virtual int focused_element_type() { return 1; };
		virtual void ZoomIn() { };
		virtual void ZoomOut() { };
		virtual void SetZoom(int zoom_percent) { };
		virtual void ResetZoom() { };
		virtual int GetZoom() { return 1; };
		virtual void InjectMouseMove(int x, int y) { };
		virtual void InjectMouseDown(int button) { };
		virtual void InjectMouseUp(int button) { };
		virtual void InjectMouseWheel(int scroll_vert, int scroll_horz) { };
		virtual void InjectKeyboardEvent(const WebKeyboardEvent& key_event) { };
		virtual void InjectTouchEvent(const int& touch_event) { };
		virtual void ActivateIME(bool activate) { };
		virtual void SetIMEComposition(const WebString& input_string, int cursor_pos, int target_start, int target_end) { };
		virtual void ConfirmIMEComposition(const WebString& input_string) { };
		virtual void CancelIMEComposition() { };
		virtual void Undo() { };
		virtual void Redo() { };
		virtual void Cut() { };
		virtual void Copy() { };
		virtual void CopyImageAt(int x, int y) { };
		virtual void Paste() { };
		virtual void PasteAndMatchStyle() { };
		virtual void SelectAll() { };
		virtual int PrintToFile(const WebString& output_directory, const int& config) { return 1; };
		virtual int last_error() const { return 0; };
		virtual JSValue CreateGlobalJavascriptObject(const WebString& name) {
			debug_log("create js obj");
			debug_stream << name << std::endl;
			return *(new JSValue());
		};
		virtual void ExecuteJavascript(const WebString& script, const WebString& frame_xpath) { };
		virtual JSValue ExecuteJavascriptWithResult(const WebString& script, const WebString& frame_xpath) {
			debug_log("exec js");
			return *(new JSValue());
		};
		virtual void set_js_method_handler(void* handler) { jshandler = handler; };
		virtual void* js_method_handler() { return jshandler; };
		virtual void set_sync_message_timeout(int timeout_ms) { };
		virtual int sync_message_timeout() { return 0; };
		virtual void DidSelectPopupMenuItem(int item_index) { };
		virtual void DidCancelPopupMenu() { };
		virtual void DidChooseFiles(const void*& files, bool should_write_files) { };
		virtual void DidLogin(int request_id, const WebString& username, const WebString& password) { };
		virtual void DidCancelLogin(int request_id) { };
		virtual void DidChooseDownloadPath(int download_id, const WebString& path) { };
		virtual void DidCancelDownload(int download_id) { };
		virtual void DidOverrideCertificateError() { };

		virtual void RequestPageInfo() { };

		virtual void ReduceMemoryUsage() { };

		WebView() { debug_log("NEW WEB VIEW FROM CTOR, WTF!"); }
		WebView(WebSession* session, int type) {
			session_ = session;
			type_ = type;
		}
		~WebView() {}

		void*proclist;
		void*menulist;
		void*dialoglist;
		void*printlist;
		void*downloadlist;
		void*inputlist;
		void*jshandler;
	private:
		WebSession* session_;
		int type_;
	};

	// Empty
	class DllExport SurfaceFactory
	{
	public:
	};

	// No CTOR -- What makes these?
	class DllExport ResourceInterceptor
	{
	public:
		virtual Awesomium::ResourceResponse* OnRequest(void* request) {
			return 0;
		}
		virtual bool OnFilterNavigation(int origin_process_id, int origin_routing_id, const Awesomium::WebString& method, const Awesomium::WebURL& url, bool is_main_frame) {
			return false;
		}
		virtual void OnWillDownload(int origin_process_id, int origin_routing_id, const Awesomium::WebURL& url) {

		}
		virtual ~ResourceInterceptor() {}
	};

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
		static void Shutdown() {
			CefShutdown();
		};
		static WebCore* instance() {
			return instance_;
		};

		virtual WebSession* CreateWebSession(const WebString& path, const WebPreferences& prefs) {
			// ignore preferences like we do with  the config.
			// empty string (all we've seen so far) is for an in-memory store of user data. this is fine with me.
			return new WebSession();
		};
		virtual WebView* CreateWebView(int width, int height, WebSession* session = 0, int type = 0) {
			debug_log("GET WEB VIEW");

			WebView* webview = new WebView(session, type);
			webview->Resize(width, height);

			return webview;
		};
		virtual void set_surface_factory(SurfaceFactory* factory) {
			debug_log("SET SURFACE FACTORY");
		};
		virtual SurfaceFactory* surface_factory() const {
			debug_log("GET SURFACE FACTORY");
			return new SurfaceFactory();
		};
		virtual void set_resource_interceptor(ResourceInterceptor* interceptor) {
			debug_log("SET RESOURCE INTERCEPTOR");
		};
		virtual ResourceInterceptor* resource_interceptor() const {
			debug_log("GET RESOURCE INTERCEPTOR");
			return new ResourceInterceptor();
		};
		virtual void Update() {
			CefDoMessageLoopWork();
		};
		virtual void Log(const WebString& message, int severity, const WebString& file, int line) {
			debug_log("LOG!!!");
		};
		virtual const char* version_string() const {
			debug_log("GET VERSION");
			return ver;
		};

		static unsigned int used_memory() { debug_log("Get used memes"); return 0; } // :-) If only it was this easy!
		static unsigned int allocated() { debug_log("Get allocated memes"); return 128; } // :-) If only it was this easy!
		static void release_memory() { debug_log("Release memes"); };

		virtual ~WebCore() { debug_log("Delete webcore!"); }

	private:
		WebCore() {
			CefMainArgs args;

			CefSettings settings;
			settings.command_line_args_disabled = true;
			CefString(&settings.browser_subprocess_path).FromASCII("gmod_cef.exe");
			settings.no_sandbox = true; // define CEF_USE_SANDBOX if you want this?

			//settings.multi_threaded_message_loop = true;

			//CefRefPtr<DumbApp> app_ref(new DumbApp);

			bool cef_magic = CefInitialize(args, settings, nullptr, nullptr);

			if (cef_magic)
				debug_log("CEF GOOD");
			else
				debug_log("CEF BAD"); //*/
			
			//CefBrowser::Creat

			makeThatWindow();

			//CefRunMessageLoop();

			//CefSettings settings;
			//CefBrowserSettings browserSettings;
		};
		static WebCore* instance_;
	};

	WebCore* WebCore::instance_ = 0;

	class DllExport BitmapSurface
	{
	public:
		BitmapSurface(int width, int height) { debug_log("new bitmap surface"); };
		~BitmapSurface() {};

		const unsigned char*buffer() const { return (unsigned char*)shitbuf; };
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

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	//debug_log("DLLMAIN RUNNING!");
	
	/*if (reason == DLL_PROCESS_ATTACH)
	{
		HANDLE thread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DllInit, NULL, 0, 0);
	}*/

	return true;
}