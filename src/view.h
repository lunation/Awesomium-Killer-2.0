
namespace Awesomium
{
	class GarryClient : public CefClient, public CefLifeSpanHandler, public CefContextMenuHandler, public CefJSDialogHandler, public CefRenderHandler {
	public:
		GarryClient() { };
		~GarryClient() { };

		CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
			return this;
		}

		bool OnBeforePopup(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& target_url,
			const CefString& target_frame_name,
			WindowOpenDisposition target_disposition,
			bool user_gesture,
			const CefPopupFeatures& popupFeatures,
			CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client,
			CefBrowserSettings& settings,
			bool* no_javascript_access
		) OVERRIDE {
			// Fuck popups.
			return true;
		}

		CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE {
			return this;
		}

		bool RunContextMenu(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefContextMenuParams> params,
			CefRefPtr<CefMenuModel> model,
			CefRefPtr<CefRunContextMenuCallback> callback
		) OVERRIDE {
			// Fuck the context menu.
			callback->Cancel();
			return true;
		}

		CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() OVERRIDE {
			return this;
		}

		bool OnJSDialog(CefRefPtr<CefBrowser> browser,
			const CefString& origin_url,
			JSDialogType dialog_type,
			const CefString& message_text,
			const CefString& default_prompt_text,
			CefRefPtr<CefJSDialogCallback> callback,
			bool& suppress_message
		) OVERRIDE {
			// Fuck this. TODO see if Gmod handles this.
			callback->Continue(false,"");
			return true;
		}

		CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE {
			return this;
		}

		///
		// Called to retrieve the root window rectangle in screen coordinates. Return
		// true if the rectangle was provided.
		///
		/*--cef()--*/
		bool GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE {
			// fuck this
			return false;
		}

		///
		// Called to retrieve the view rectangle which is relative to screen
		// coordinates. Return true if the rectangle was provided.
		///
		/*--cef()--*/
		bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect);

		///
		// Called to retrieve the translation from view coordinates to actual screen
		// coordinates. Return true if the screen coordinates were provided.
		///
		/*--cef()--*/
		bool GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY) OVERRIDE {
			//screenX = viewX;
			//screenY = viewY;
			return false;
		}

		///
		// Called to allow the client to fill in the CefScreenInfo object with
		// appropriate values. Return true if the |screen_info| structure has been
		// modified.
		//
		// If the screen info rectangle is left empty the rectangle from GetViewRect
		// will be used. If the rectangle is still empty or invalid popups may not be
		// drawn correctly.
		///
		/*--cef()--*/
		bool GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screen_info) OVERRIDE;

		///
		// Called when the browser wants to show or hide the popup widget. The popup
		// should be shown if |show| is true and hidden if |show| is false.
		///
		/*--cef()--*/
		void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) OVERRIDE {
			panic(__FUNCTION__);
		}

		///
		// Called when the browser wants to move or resize the popup widget. |rect|
		// contains the new location and size in view coordinates.
		///
		/*--cef()--*/
		void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) OVERRIDE {
			panic(__FUNCTION__);
		}

		///
		// Called when an element should be painted. Pixel values passed to this
		// method are scaled relative to view coordinates based on the value of
		// CefScreenInfo.device_scale_factor returned from GetScreenInfo. |type|
		// indicates whether the element is the view or the popup widget. |buffer|
		// contains the pixel data for the whole image. |dirtyRects| contains the set
		// of rectangles in pixel coordinates that need to be repainted. |buffer| will
		// be |width|*|height|*4 bytes in size and represents a BGRA image with an
		// upper-left origin.
		///
		/*--cef()--*/
		void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) OVERRIDE;

		///
		// Called when the browser's cursor has changed. If |type| is CT_CUSTOM then
		// |custom_cursor_info| will be populated with the custom cursor information.
		///
		/*--cef()--*/
		void OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor, CursorType type, const CefCursorInfo& custom_cursor_info) OVERRIDE;

		///
		// Called when the user starts dragging content in the web view. Contextual
		// information about the dragged content is supplied by |drag_data|.
		// (|x|, |y|) is the drag start location in screen coordinates.
		// OS APIs that run a system message loop may be used within the
		// StartDragging call.
		//
		// Return false to abort the drag operation. Don't call any of
		// CefBrowserHost::DragSource*Ended* methods after returning false.
		//
		// Return true to handle the drag operation. Call
		// CefBrowserHost::DragSourceEndedAt and DragSourceSystemDragEnded either
		// synchronously or asynchronously to inform the web view that the drag
		// operation has ended.
		///
		/*--cef()--*/
		bool StartDragging(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> drag_data, DragOperationsMask allowed_ops, int x, int y) OVERRIDE {
			// fuck click-dragging
			return false;
		}

		///
		// Called when the web view wants to update the mouse cursor during a
		// drag & drop operation. |operation| describes the allowed operation
		// (none, move, copy, link).
		///
		/*--cef()--*/
		void UpdateDragCursor(CefRefPtr<CefBrowser> browser, DragOperation operation) OVERRIDE {
			panic(__FUNCTION__);
		}

		///
		// Called when the scroll offset has changed.
		///
		/*--cef()--*/
		void OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser, double x, double y) OVERRIDE {
			// At first I thought we were supposed to modify the surface using the scroll offset, but it seems to work fine as it is?
		}

	private:
		// List of existing browser windows. Only accessed on the CEF UI thread.

		// Include the default reference counting implementation.
		IMPLEMENT_REFCOUNTING(GarryClient)
	};

	class JsCallDataSource : public DataSource {
	public:
		JsCallDataSource(WebView* view) {
			this->view = view;
		}
		void OnRequest(int id, const ResourceRequest& request, const WebString& path) OVERRIDE;
	private:
		// Decodes percent encoded string. Seems to be safe.
		static std::wstring decode_url_part(const std::wstring& str) {
			std::vector<wchar_t> string_builder;
			for ( int i = 0; i < str.length(); i++ ) {
				wchar_t c = str[i];
				if (c == '%') {
					if (i == str.length()-1)
						break;
					c = std::stoul(str.substr(i+1, 2),0,16);
					i += 2;
				}

				string_builder.push_back(c);
			}
			return std::wstring(string_builder.data(),string_builder.size());
		}
		WebView* view;
	};

	WebSession::WebSession() {
		client = new GarryClient();
		//AddDataSource(WebString(L"call"), new JsCallDataSource());
	}

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
			// Need these 'cause vtables.
			virtual void OnChangeTitle(WebView* caller, const WebString& title) = 0;
			virtual void OnChangeAddressBar(WebView* caller, const WebURL& url) = 0;
			virtual void OnChangeTooltip(WebView* caller, const WebString& tooltip) = 0;
			virtual void OnChangeTargetURL(WebView* caller, const WebURL& url) = 0;

			virtual void OnChangeCursor(WebView* caller, int cursor) = 0;
		protected:
			virtual ~View() { debug_log(__FUNCTION__); };
		};

	};

#pragma pack(push,1)
	struct Rect {
		int x;
		int y;
		int width;
		int height;

		Rect(int x, int y, int width, int height) {
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}
	};
#pragma pack(pop)

	class DllExport Surface
	{
	public:
		virtual ~Surface() { };
		virtual void Paint(unsigned char*source, int span, const Rect& src_rect, const Rect& dst_rect) { debug_log(__FUNCTION__); };
		virtual void Scroll(int dx, int dy, const Rect& clip_rect) { debug_log(__FUNCTION__); };
	};

	class DllExport BitmapSurface : public Surface
	{
	public:
		BitmapSurface(int width, int height) {
			width_ = width;
			height_ = height;
			row_span_ = width_ * 4;
			buffer_ = new unsigned char[row_span_ * height_];
			is_dirty_ = false;
		};
		~BitmapSurface() {
			delete buffer_;
		};

		// THESE ARE INLINED!
		const unsigned char*buffer() const { return buffer_; };
		int width() const { return width_; };
		int height() const { debug_log(__FUNCTION__); return height_; };
		int row_span() const { debug_log(__FUNCTION__); return row_span_; };

		// THIS IS NOT!
		void set_is_dirty(bool is_dirty) { is_dirty_ = is_dirty; };
		
		// INLINED!
		bool is_dirty() const { return is_dirty_; };

		// NOT! (ALSO NOT USED)
		void CopyTo(unsigned char* dst, int dst_row, int dst_depth, bool convert, bool flipy) const { debug_log(__FUNCTION__); };
		bool SaveToPNG(const Awesomium::WebString* file_path, bool preserve_transparency = false) { debug_log(__FUNCTION__); return false; };
		bool SaveToJPEG(const Awesomium::WebString* file_path, int quality = 90) { debug_log(__FUNCTION__); return false; };
		unsigned char GetAlphaAtPoint(int x, int y) const { debug_log(__FUNCTION__); return 255; };
		
		void Paint(unsigned char*source, int span, const Rect& src_rect, const Rect& dst_rect) OVERRIDE {

			unsigned char* source_ptr = source + src_rect.x * 4 + src_rect.y * span;
			unsigned char* dest_ptr = buffer_ + (dst_rect.x + dst_rect.y * width_) * 4;

			int rect_span = src_rect.width * 4;
			int dest_span = width_ * 4;


			for (int y = 0; y < src_rect.height; y++) {
				memcpy(dest_ptr, source_ptr, rect_span);
				source_ptr += span;
				dest_ptr += dest_span;
			}

			is_dirty_ = true;
		};
		void Scroll(int dx, int dy, const Rect& clip_rect) OVERRIDE {
			debug_log(__FUNCTION__);
		};

	private:
		unsigned char* buffer_;
		int width_;
		int height_;
		int row_span_;
		bool is_dirty_;
	};

	// Uhhh?
	class DllExport WebKeyboardEvent
	{
	public:
		WebKeyboardEvent() { debug_log(__FUNCTION__); };

		enum Type {
			Down,
			Up,
			Char
		};

		Type type;
		int modifiers;
		int virtual_key_code;
		int native_key_code;
		char key_identifier[20];
		wchar_t text[4];
		wchar_t unmodified_text[4];
		bool is_system_key;
	};

	typedef HANDLE ProcessHandle;
	typedef HWND NativeWindow;

	class JSMethodHandler {
	public:
		virtual void OnMethodCall(Awesomium::WebView* caller, unsigned int remote_object_id, const Awesomium::WebString& method_name, const Awesomium::JSArray& args) = 0;
		virtual Awesomium::JSValue OnMethodCallWithReturnValue(Awesomium::WebView* caller, unsigned int remote_object_id, const Awesomium::WebString& method_name, const Awesomium::JSArray& args) = 0;
		virtual ~JSMethodHandler() {}
	};

	std::unordered_map<int, WebView*> browser_map;

	// A goddamn window
	class DllExport WebView
	{
	public:
		virtual void Destroy() {
			delete this;
		};
		virtual WebViewType type() { debug_log(__FUNCTION__); return 1; }; // is this right? prolly dont matter
		virtual int process_id() { debug_log(__FUNCTION__); return 1; };
		virtual int routing_id() { debug_log(__FUNCTION__); return 1; };
		virtual int next_routing_id() { debug_log(__FUNCTION__); return 1; };
		virtual ProcessHandle process_handle() { debug_log(__FUNCTION__); return ProcessHandle(); };
		virtual void set_parent_window(NativeWindow parent) { debug_log(__FUNCTION__); };
		virtual NativeWindow parent_window() { debug_log(__FUNCTION__); return NativeWindow(); };
		virtual NativeWindow window() { debug_log(__FUNCTION__); return NativeWindow(); };
		virtual void set_view_listener(WebViewListener::View* listener) { listener_view = listener; };
		virtual void set_load_listener(WebViewListener::Load* listener) { debug_log(__FUNCTION__); };
		virtual void set_process_listener(void* listener) { debug_log(__FUNCTION__); proclist = listener; };
		virtual void set_menu_listener(void* listener) { debug_log(__FUNCTION__); menulist = listener; };
		virtual void set_dialog_listener(void* listener) { debug_log(__FUNCTION__); dialoglist = listener; };
		virtual void set_print_listener(void* listener) { debug_log(__FUNCTION__); printlist = listener; };
		virtual void set_download_listener(void* listener) { debug_log(__FUNCTION__); downloadlist = listener; };
		virtual void set_input_method_editor_listener(void* listener) { debug_log(__FUNCTION__); inputlist = listener; };
		virtual WebViewListener::View* view_listener() { debug_log(__FUNCTION__); return listener_view; };
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
			browser->GetMainFrame()->LoadURL( url.spec().data() );
		};
		virtual void GoBack() { debug_log(__FUNCTION__); };
		virtual void GoForward() { debug_log(__FUNCTION__); };
		virtual void GoToHistoryOffset(int offset) { debug_log(__FUNCTION__); };
		virtual void Stop() { debug_log(__FUNCTION__); };
		virtual void Reload(bool ignore_cache) { debug_log(__FUNCTION__); };
		virtual bool CanGoBack() { debug_log(__FUNCTION__); return true; };
		virtual bool CanGoForward() { debug_log(__FUNCTION__); return true; };
		virtual Surface* surface() {
			return _surface;
		};
		virtual WebURL url() { debug_log(__FUNCTION__); return *(new WebURL()); };
		virtual WebString title() { debug_log(__FUNCTION__); return WebString(); };
		virtual WebSession* session() { debug_log(__FUNCTION__); return session_; }
		virtual bool IsLoading() {
			return false;	//return browser->IsLoading();
		};
		virtual bool IsCrashed() { return false; }; // ASSUME NOT CRASHED
		virtual void Resize(int width, int height) {

			if (_surface)
				delete _surface;
			_surface = new BitmapSurface(width, height);
			_width = width;
			_height = height;

			browser->GetHost()->WasResized();
		};
		virtual void SetTransparent(bool is_transparent) { debug_log(__FUNCTION__); };
		virtual bool IsTransparent() { debug_log(__FUNCTION__); return false; };
		virtual void PauseRendering() { debug_log(__FUNCTION__); };
		virtual void ResumeRendering() { debug_log(__FUNCTION__); };
		virtual void Focus() {
			//debug_log(__FUNCTION__);
			browser->GetHost()->SendFocusEvent(true);
			//browser->GetHost()->SetFocus(true);
		};
		virtual void Unfocus() {
			//debug_log(__FUNCTION__);
			browser->GetHost()->SendFocusEvent(false);
			//browser->GetHost()->SetFocus(false);
		};
		virtual int focused_element_type() { debug_log(__FUNCTION__); return 1; };
		virtual void ZoomIn() { debug_log(__FUNCTION__); };
		virtual void ZoomOut() { debug_log(__FUNCTION__); };
		virtual void SetZoom(int zoom_percent) { debug_log(__FUNCTION__); };
		virtual void ResetZoom() { debug_log(__FUNCTION__); };
		virtual int GetZoom() { debug_log(__FUNCTION__);  return 1; };
		virtual void InjectMouseMove(int x, int y) {
			mouse.x = x;
			mouse.y = y;
			browser->GetHost()->SendMouseMoveEvent(mouse, false);
		};
		virtual void InjectMouseDown(int button) {
			// add the correct flag.
			switch (button) {
			case 0:
				mouse.modifiers |= cef_event_flags_t::EVENTFLAG_LEFT_MOUSE_BUTTON;
				break;
			case 1:
				mouse.modifiers |= cef_event_flags_t::EVENTFLAG_MIDDLE_MOUSE_BUTTON;
				break;
			case 2:
				mouse.modifiers |= cef_event_flags_t::EVENTFLAG_RIGHT_MOUSE_BUTTON;
				break;
			}
			
			// don't worry about double clicks, they are broken in awesome as well.
			browser->GetHost()->SendMouseClickEvent(mouse, static_cast<cef_mouse_button_type_t>(button), false, 1);

		};
		virtual void InjectMouseUp(int button) {
			
			// remove the correct flag.
			switch (button) {
			case 0:
				mouse.modifiers &= ~cef_event_flags_t::EVENTFLAG_LEFT_MOUSE_BUTTON;
				break;
			case 1:
				mouse.modifiers &= ~cef_event_flags_t::EVENTFLAG_MIDDLE_MOUSE_BUTTON;
				break;
			case 2:
				mouse.modifiers &= ~cef_event_flags_t::EVENTFLAG_RIGHT_MOUSE_BUTTON;
				break;
			}

			browser->GetHost()->SendMouseClickEvent(mouse, static_cast<cef_mouse_button_type_t>(button), true, 0);
		};
		virtual void InjectMouseWheel(int scroll_vert, int scroll_horz) {
			browser->GetHost()->SendMouseWheelEvent(mouse, scroll_horz, scroll_vert);
		};
		virtual void InjectKeyboardEvent(const WebKeyboardEvent& key_event) {

			if (key_event.virtual_key_code == 122) {
				CefWindowInfo wininf;
				wininf.SetAsPopup(0, "Gmod CEF Dev Tools");

				CefBrowserSettings settings;

				CefPoint p;
				browser->GetHost()->ShowDevTools(wininf, browser->GetHost()->GetClient() , settings, p);
			}
			else {
				CefKeyEvent key;
				key.character = key_event.Char;
				//key.focus_on_editable_field = ?
				key.is_system_key = key_event.is_system_key;
				key.modifiers = key_event.modifiers;
				key.native_key_code = key_event.native_key_code;
				key.type = static_cast<cef_key_event_type_t>( key_event.type + 1);
				key.unmodified_character = key_event.unmodified_text[0]; // ???
				key.windows_key_code = key_event.virtual_key_code;

				browser->GetHost()->SendKeyEvent(key);
			}
		};
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

			if (!globals.HasProperty(name))
				globals.SetProperty(name, JSValue(JSObject()) );
			return globals.GetProperty(name);

		};
		virtual void ExecuteJavascript(const WebString& script, const WebString& frame_xpath) {
			debug_log("running js!");
			if (!frame_xpath.IsEmpty())
				panic("non-empty frame path for js execution!");

			browser->GetMainFrame()->ExecuteJavaScript(CefString(script.data()), CefString("gmod"), 0);
			debug_log("finished js!");
		};
		virtual JSValue ExecuteJavascriptWithResult(const WebString& script, const WebString& frame_xpath) {
			debug_log(__FUNCTION__);
			return *(new JSValue());
		};

		virtual void set_js_method_handler(JSMethodHandler* handler) {
			jshandler = handler;
		};
		virtual void* js_method_handler() { return jshandler; };

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

			window_info.SetAsWindowless(0, true);

			CefRefPtr<CefClient> client(session->client);

			std::string url; // -DISREGARD FOR NOW: if we don't have some url to navigate to, we'll end up in a deadlock when javascript IPCs run

			CefBrowserSettings browser_settings;

			browser = CefBrowserHost::CreateBrowserSync(window_info, client, url, browser_settings, NULL);

			call_source = new JsCallDataSource(this);

			globals = JSObject(this, getNextGlobalID());

			browser_map[browser->GetIdentifier()] = this;
		}
		~WebView() {
			browser_map.erase(browser->GetIdentifier());

			delete call_source;

			browser->GetHost()->CloseBrowser(true);

			if (_surface != nullptr)
				delete _surface;
		}

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
		JSMethodHandler*jshandler;

		int _width;
		int _height;

		JsCallDataSource* call_source;

	private:
		WebSession* session_;
		CefRefPtr<CefBrowser> browser;
		JSObject globals;
		int next_global_id = -1; // root object takes this!
		Surface* _surface = 0;
		CefMouseEvent mouse;

		WebViewListener::View* listener_view;
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

	void JsCallDataSource::OnRequest(int id, const ResourceRequest& request, const WebString& path) {

		debug_log("enter");

		if (path.data() == 0) {
			SendResponse(id, 0, (unsigned const char*)0, WebString(L"application/json"));
			return;
		}

		std::wstring std_path = std::wstring(path.data());

		int id_end = std_path.find_first_of('/');
		int name_end = std_path.find_first_of('?', id_end+1);

		//debug_stream << "~~~" << id_end << " " << name_end << std::endl;

		if (id_end == 0 || name_end==0) {
			SendResponse(id, 0, (unsigned const char*)0, WebString(L"application/json"));
			return;
		}

		int call_id = atoi( CefString( std_path.substr(0,id_end).c_str() ).ToString().c_str() );
		WebString call_name = WebString( decode_url_part( std_path.substr(id_end+1, name_end-id_end-1) ).c_str() );
		WebString call_arg_json = WebString( decode_url_part( std_path.substr(name_end+1) ).c_str() );

		//debug_stream << "JS THIS SHIT " << call_id << " " << call_name << " " << call_arg_json << " " << std::endl;

		const wchar_t* json_ptr = call_arg_json.data();

		if (json_ptr == 0) {
			SendResponse(id, 0, (unsigned const char*)0, WebString(L"application/json"));
			return;
		}

		JSValue x;
		std::string json_result;

		if (!JSValue::ParseJSON(json_ptr, x) || !x.IsArray()) {
			debug_log("parse args: CRITICAL FAILURE!");
		}
		else {
			//debug_log("JS - Pre Lock");
			//js_call_lock.lock();
			//debug_log("JS - Post Lock");
			JSValue result = view->jshandler->OnMethodCallWithReturnValue(view, call_id, call_name, x.ToArray());
			//debug_log("JS - Pre Unlock");
			//js_call_lock.unlock();
			//debug_log("JS - Post Unlock");
			json_result = result.ToJSON();
		}

		SendResponse(id, json_result.length() , (unsigned const char*)json_result.c_str(), WebString(L"application/json"));
	};




	bool GarryClient::GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screen_info) {
		debug_log(__FUNCTION__);

		//screen_info.depth = 32;
		//screen_info.depth_per_component = 8;
		
		//CefRect view_rect;
		//GetViewRect(browser, view_rect);

		//screen_info.rect = view_rect;
		//screen_info.available_rect = view_rect;

		//debug_log(__FUNCTION__);
		auto ptr = browser_map[browser->GetIdentifier()];

		CefRect screen_rect(0,0, ptr->_width, ptr->_height);
		screen_info.Set(1, 32, 8, false, screen_rect, screen_rect);

		return false;
	}

	bool GarryClient::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
		//debug_log(__FUNCTION__);
		auto ptr = browser_map[browser->GetIdentifier()];
		//debug_log(__FUNCTION__" x");
		rect.Set(0, 0, ptr->_width, ptr->_height);
		//cef_log(0,0,0, (__FUNCTION__" y "+std::to_string(ptr->_width)+" "+std::to_string(ptr->_height)).c_str() );
		return true;
	}

	//
	// TODO! OPTIMIZE THIS BY DOING A SINGLE MEMCOPY WHEN THE RECT SPANS THE SOURCE AND DESTINATION
	//
	// TODO! SEE IF WE CAN JUST RE-USE THE POINTER GIVEN TO US! PROBABLY NOT, BUT I DOUBT THIS BUFFER IS BEING RE-CREATED EVERY CALL!
	void GarryClient::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) {
		
		//auto x = std::chrono::high_resolution_clock::now();
		auto it = browser_map.find(browser->GetIdentifier());
		if (it == browser_map.end()) {
			// TODO, DO THESE CHECKS MORE!
			debug_log("abort paint, no browser");
			return;
		}
		auto surface = it->second->surface();
		//auto surface_width = surface->width();
		
		//auto surface_buffer = const_cast<unsigned char*>(surface->buffer());
		auto source_buffer = static_cast<unsigned char*>(const_cast<void*>(buffer));

		//debug_stream << "PAINT SIZE: ( " << width << ", " << height << " ) COUNT: " << dirtyRects.size() << " TYPE: " << type << std::endl;

		int source_span = width * 4;

		for (auto rect : dirtyRects) {
			//debug_stream << "POS: ( " << rect.x << ", " << rect.y << " ) SIZE: ( " << rect.width << ", " << rect.height << " ) " << std::endl;

			auto a_rect = Rect(rect.x, rect.y, rect.width, rect.height);

			surface->Paint(source_buffer, source_span, a_rect, a_rect);

			/*unsigned char* source_ptr =	source_buffer +	(rect.x + rect.y * width) * 4;
			unsigned char* dest_ptr = surface_buffer + (rect.x + rect.y * surface_width) * 4;

			int rect_span = rect.width * 4;
			int source_span = width * 4;
			int dest_span = surface_width * 4;

			
			for (int y = 0; y < rect.height; y++) {
				memcpy(dest_ptr, source_ptr, rect_span);
				source_ptr += source_span;
				dest_ptr += dest_span;
			}*/
			
		}

		//auto y = std::chrono::high_resolution_clock::now();

		//std::chrono::duration<double> span = std::chrono::duration_cast<std::chrono::duration<double>>(y - x);
		//} while (y < 100);
		//debug_stream << "time " << span.count() << " " << CLOCKS_PER_SEC << std::endl;

		//surface->set_is_dirty(true);
		//debug_stream << "( " << width << ", " << height << " ) " << dirtyRects.size() << std::endl;*/
	}

	void GarryClient::OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor, CursorType type, const CefCursorInfo& custom_cursor_info) {
		auto view = browser_map[browser->GetIdentifier()];
		auto listener = view->view_listener();

		if (listener != nullptr)
			listener->OnChangeCursor(view, type );
	}
}