namespace Awesomium
{
	namespace WebViewListener
	{

		class DllExport Load
		{
		public:
			virtual ~Load() { debug_log(__FUNCTION__); };
		};

		class DllExport View
		{
		public:
			virtual ~View() { debug_log(__FUNCTION__); };
		};

	};

	class DllExport Surface
	{
	public:
		virtual ~Surface() { debug_log(__FUNCTION__); };
		virtual void Paint(unsigned  char*shit, int span, void*&srcrect, void*&destrect) { debug_log(__FUNCTION__); memset(shit, 1, span); };
		virtual void Scroll(unsigned  char*shit, int span, void*&srcrect, void*&destrect) { debug_log(__FUNCTION__); memset(shit, 1, span); };

	};

	// Uhhh?
	class DllExport WebKeyboardEvent
	{
	public:
		WebKeyboardEvent() { debug_log(__FUNCTION__); };

	};

	typedef HANDLE ProcessHandle;
	typedef HWND NativeWindow;

	// A goddamn window
	class DllExport WebView
	{
	public:
		virtual void Destroy() { debug_log(__FUNCTION__); };
		virtual WebViewType type() { debug_log(__FUNCTION__); return 1; }; // is this right? prolly dont matter
		virtual int process_id() { debug_log(__FUNCTION__); return 1; };
		virtual int routing_id() { debug_log(__FUNCTION__); return 1; };
		virtual int next_routing_id() { debug_log(__FUNCTION__); return 1; };
		virtual ProcessHandle process_handle() { debug_log(__FUNCTION__); return ProcessHandle(); };
		virtual void set_parent_window(NativeWindow parent) { debug_log(__FUNCTION__); };
		virtual NativeWindow parent_window() { debug_log(__FUNCTION__); return NativeWindow(); };
		virtual NativeWindow window() { debug_log(__FUNCTION__); return NativeWindow(); };
		virtual void set_view_listener(WebViewListener::View* listener) { debug_log(__FUNCTION__); };
		virtual void set_load_listener(WebViewListener::Load* listener) { debug_log(__FUNCTION__); };
		virtual void set_process_listener(void* listener) { debug_log(__FUNCTION__); proclist = listener; };
		virtual void set_menu_listener(void* listener) { debug_log(__FUNCTION__); menulist = listener; };
		virtual void set_dialog_listener(void* listener) { debug_log(__FUNCTION__); dialoglist = listener; };
		virtual void set_print_listener(void* listener) { debug_log(__FUNCTION__); printlist = listener; };
		virtual void set_download_listener(void* listener) { debug_log(__FUNCTION__); downloadlist = listener; };
		virtual void set_input_method_editor_listener(void* listener) { debug_log(__FUNCTION__); inputlist = listener; };
		virtual WebViewListener::View* view_listener() { debug_log(__FUNCTION__); return new WebViewListener::View(); };
		virtual WebViewListener::Load* load_listener() { debug_log(__FUNCTION__); return new WebViewListener::Load(); };
		virtual void* process_listener() {
			debug_log(__FUNCTION__);
			return proclist;
		};
		virtual void* menu_listener() {
			debug_log(__FUNCTION__);
			return menulist;
		};
		virtual void* dialog_listener() {
			debug_log(__FUNCTION__);
			return dialoglist;
		};
		virtual void* print_listener() {
			debug_log(__FUNCTION__);
			return printlist;
		};
		virtual void* download_listener() {
			debug_log(__FUNCTION__);
			return downloadlist;
		};
		virtual void* input_method_editor_listener() {
			debug_log(__FUNCTION__);
			return inputlist;
		};
		virtual void LoadURL(const WebURL& url) {
			debug_log(__FUNCTION__);
			browser->GetMainFrame()->LoadURL( url.spec().data() );
			
			//LoadStringW(url.spec().data(), "http://google.com" );
			//debug_stream << url << endl;
		};
		virtual void GoBack() { debug_log(__FUNCTION__); };
		virtual void GoForward() { debug_log(__FUNCTION__); };
		virtual void GoToHistoryOffset(int offset) { debug_log(__FUNCTION__); };
		virtual void Stop() { debug_log(__FUNCTION__); };
		virtual void Reload(bool ignore_cache) { debug_log(__FUNCTION__); };
		virtual bool CanGoBack() { debug_log(__FUNCTION__); return true; };
		virtual bool CanGoForward() { debug_log(__FUNCTION__); return true; };
		virtual Surface* surface() {
			debug_log(__FUNCTION__);
			return 0;//oursurface;
		};
		virtual WebURL url() { debug_log(__FUNCTION__); return *(new WebURL()); };
		virtual WebString title() { debug_log(__FUNCTION__); return WebString(); };
		virtual WebSession* session() { debug_log(__FUNCTION__); return session_; }
		virtual bool IsLoading() { return browser->IsLoading(); };
		virtual bool IsCrashed() { return false; }; // ASSUME NOT CRASHED
		virtual void Resize(int width, int height) { // NEED TO TAKE CARE OF THIS ONCE WE DITCH NATIVE WINDOWS
			debug_log(__FUNCTION__);
			debug_stream << width << ", " << height << std::endl;
		};
		virtual void SetTransparent(bool is_transparent) { debug_log(__FUNCTION__); };
		virtual bool IsTransparent() { debug_log(__FUNCTION__); return false; };
		virtual void PauseRendering() { debug_log(__FUNCTION__); };
		virtual void ResumeRendering() { debug_log(__FUNCTION__); };
		virtual void Focus() { debug_log(__FUNCTION__); };
		virtual void Unfocus() { debug_log(__FUNCTION__); };
		virtual int focused_element_type() { debug_log(__FUNCTION__); return 1; };
		virtual void ZoomIn() { debug_log(__FUNCTION__); };
		virtual void ZoomOut() { debug_log(__FUNCTION__); };
		virtual void SetZoom(int zoom_percent) { debug_log(__FUNCTION__); };
		virtual void ResetZoom() { debug_log(__FUNCTION__); };
		virtual int GetZoom() { debug_log(__FUNCTION__);  return 1; };
		virtual void InjectMouseMove(int x, int y) { debug_log(__FUNCTION__); };
		virtual void InjectMouseDown(int button) { debug_log(__FUNCTION__); };
		virtual void InjectMouseUp(int button) { debug_log(__FUNCTION__); };
		virtual void InjectMouseWheel(int scroll_vert, int scroll_horz) { debug_log(__FUNCTION__); };
		virtual void InjectKeyboardEvent(const WebKeyboardEvent& key_event) { debug_log(__FUNCTION__); };
		virtual void InjectTouchEvent(const int& touch_event) { debug_log(__FUNCTION__); };
		virtual void ActivateIME(bool activate) { debug_log(__FUNCTION__); };
		virtual void SetIMEComposition(const WebString& input_string, int cursor_pos, int target_start, int target_end) { debug_log(__FUNCTION__); };
		virtual void ConfirmIMEComposition(const WebString& input_string) { debug_log(__FUNCTION__); };
		virtual void CancelIMEComposition() { debug_log(__FUNCTION__); };
		virtual void Undo() { debug_log(__FUNCTION__); };
		virtual void Redo() { debug_log(__FUNCTION__); };
		virtual void Cut() { debug_log(__FUNCTION__); };
		virtual void Copy() { debug_log(__FUNCTION__); };
		virtual void CopyImageAt(int x, int y) { debug_log(__FUNCTION__); };
		virtual void Paste() { debug_log(__FUNCTION__); };
		virtual void PasteAndMatchStyle() { debug_log(__FUNCTION__); };
		virtual void SelectAll() { debug_log(__FUNCTION__); };
		virtual int PrintToFile(const WebString& output_directory, const int& config) { debug_log(__FUNCTION__); return 1; };
		virtual int last_error() const { debug_log(__FUNCTION__); return 0; };
		virtual JSValue CreateGlobalJavascriptObject(const WebString& name) {
			debug_stream << "GET " << name << std::endl;

			if (!globals.HasProperty(name))
				globals.SetProperty(name, JSValue(JSObject()) );
			return globals.GetProperty(name);


			//CefRefPtr<CefV8Value> globals =	browser->GetMainFrame()->GetV8Context()->GetGlobal();
						/*debug_stream << "Have globals!" << std::endl;

			CefRefPtr<CefV8Value> val = globals->GetValue(name.data());

			debug_stream << "Have value!" << std::endl;

			if (val->IsNull())
				debug_stream << "NULL" << std::endl;
			else
				debug_stream << "OKAY" << std::endl;
				*/
			//return *(new JSValue());
		};
		virtual void ExecuteJavascript(const WebString& script, const WebString& frame_xpath) { debug_log(__FUNCTION__); };
		virtual JSValue ExecuteJavascriptWithResult(const WebString& script, const WebString& frame_xpath) {
			debug_log(__FUNCTION__);
			return *(new JSValue());
		};
		virtual void set_js_method_handler(void* handler) { debug_log(__FUNCTION__); jshandler = handler; };
		virtual void* js_method_handler() { debug_log(__FUNCTION__); return jshandler; };
		virtual void set_sync_message_timeout(int timeout_ms) { debug_log(__FUNCTION__); };
		virtual int sync_message_timeout() { debug_log(__FUNCTION__); return 0; };
		virtual void DidSelectPopupMenuItem(int item_index) { debug_log(__FUNCTION__); };
		virtual void DidCancelPopupMenu() { debug_log(__FUNCTION__); };
		virtual void DidChooseFiles(const void*& files, bool should_write_files) { debug_log(__FUNCTION__); };
		virtual void DidLogin(int request_id, const WebString& username, const WebString& password) { debug_log(__FUNCTION__); };
		virtual void DidCancelLogin(int request_id) { debug_log(__FUNCTION__); };
		virtual void DidChooseDownloadPath(int download_id, const WebString& path) { debug_log(__FUNCTION__); };
		virtual void DidCancelDownload(int download_id) { debug_log(__FUNCTION__); };
		virtual void DidOverrideCertificateError() { debug_log(__FUNCTION__); };

		virtual void RequestPageInfo() { debug_log(__FUNCTION__); };

		virtual void ReduceMemoryUsage() { debug_log(__FUNCTION__); };

		WebView() { debug_log("NEW WEB VIEW FROM CTOR, WTF!"); }
		WebView(WebSession* session) {
			session_ = session;
			
			CefWindowInfo window_info;

			#if defined(OS_WIN)
				// On Windows we need to specify certain flags that will be passed to
				// CreateWindowEx().
				window_info.SetAsPopup(NULL, "Ain't gonna happen, huh?");
			#endif

			CefRefPtr<CefClient> client(session->client); // TODO REPLACE WITH CLIENT FROM SESSION!

			std::string url("xyzzy"); // if we don't have some url to navigate to, we'll end up in a deadlock when javascript IPCs run

			CefBrowserSettings browser_settings;

			browser = CefBrowserHost::CreateBrowserSync(window_info, client, url, browser_settings, NULL);

			globals = JSObject(this, getNextGlobalID());
		}
		~WebView() { debug_log(__FUNCTION__); }

		int getNextGlobalID() {
			return next_global_id++;
		}

		CefRefPtr<CefBrowser> getCefBrowser() {
			return browser;
		}

		void*proclist;
		void*menulist;
		void*dialoglist;
		void*printlist;
		void*downloadlist;
		void*inputlist;
		void*jshandler;
	private:
		WebSession* session_;
		CefRefPtr<CefBrowser> browser;
		JSObject globals;
		int next_global_id = -1; // global object takes this!
	};


	void JSObject_Global::set(const WebString& name, const JSValue& value, bool wait) {
		auto msg = CefProcessMessage::Create("garry_js_set_global");
		auto args = msg->GetArgumentList();
		args->SetInt(0, index);
		args->SetString(1, name.data());
		
		// always grab a new id -- everything in the global table uses one!
		int new_id = browser->getNextGlobalID();

		if (value.IsObject()) {
			

			args->SetInt(2, JsValueType::Object);
			browser->getCefBrowser()->SendProcessMessage(PID_RENDERER, msg);

			auto newObj = JSObject(browser, new_id);
			value.ToObject().copyTo(newObj);
			auto newValue = JSValue(newObj);

			JSObject_Local::set(name, newValue, wait);
		}
		else {
			panic("unknown child for global");

			JSObject_Local::set(name, value, wait);
		}

		

		//JSObject_Local::set(name, value, wait);
	}

	void JSObject_Global::setCustomMethod(const WebString& name, bool has_return_value) {
		if (!has_return_value)
			panic("custom methods usually have a return value, one didnt!");

		// always grab a new id -- everything in the global table uses one!
		int new_id = browser->getNextGlobalID();
		
		auto msg = CefProcessMessage::Create("garry_js_set_global");
		auto args = msg->GetArgumentList();
		args->SetInt(0, index);
		args->SetString(1, name.data());
		args->SetInt(2, JsValueType::CustomMethod);
		
		browser->getCefBrowser()->SendProcessMessage(PID_RENDERER, msg);
	}
}