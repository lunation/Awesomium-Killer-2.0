namespace Awesomium {

	// SHOULD be safe to resize!
	class DllExport WebSession
	{
	public:
		virtual void Release() const { debug_log(__FUNCTION__); };
		virtual bool IsOnDisk() const { debug_log(__FUNCTION__); return false; };
		virtual WebString data_path() const { debug_log(__FUNCTION__); return WebString(); };
		virtual const WebPreferences& preferences() const { debug_log(__FUNCTION__); return *(new WebPreferences()); }; // VERY NOT GOOD!!! <======================================
		virtual void AddDataSource(const WebString& asset_host, DataSource* source) {
			data_sources[asset_host.data()] = source;
		};
		virtual void SetCookie(const WebURL& url, const WebString& cookie_string, bool is_http_only, bool force_session_cookie) { debug_log(__FUNCTION__); };
		virtual void ClearCookies() { debug_log(__FUNCTION__); };
		virtual void ClearCache() { debug_log(__FUNCTION__); };
		virtual int GetZoomForURL(const WebURL& url) { debug_log(__FUNCTION__);  return 0; };
	protected:
		virtual ~WebSession() { debug_log(__FUNCTION__); }

	public:
		WebSession();

		std::unordered_map<std::wstring, DataSource*> data_sources;
		CefRefPtr<CefClient> client;
	};

	class GarryClient : public CefClient {
	public:
		GarryClient() { };
		~GarryClient() { };

		bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) OVERRIDE {
			
			if (message->GetName() == "garry_js_result_value") {
				//js_ipc_complete = true;

				debug_log("FIX ME BROKEN IPC");

				/*switch (message->GetArgumentList()->GetInt(0)) {
				case JsValueType::Object:
					js_ipc_result = JSValue(JSObject( message->GetArgumentList()->GetInt(1) ));
					break;
				case JsValueType::Undefined:
					js_ipc_result = JSValue::Undefined();
					break;

				case JsValueType::Unknown:
					debug_log("HYP UNKNOWN");
					break;
				}*/

				return true;
			}
			
			return false;
		}

		/*virtual CefRefPtr<CefRequestHandler> GetResourc() OVERRIDE {
			debug_log("fuckshitstack!!!");
			return this;
		}*/

		/*virtual CefRefPtr<CefResourceHandler> GetResourceHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request) OVERRIDE
		{
			debug_log("resource!!!");
			debug_stream << request->GetURL().ToString() << std::endl;
			return NULL;
		}*/

		/*virtual ReturnValue OnBeforeResourceLoad(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			CefRefPtr<CefRequestCallback> callback) OVERRIDE {

			debug_log("resource!!!");
			debug_stream << request->GetURL().ToString() << std::endl;

			callback

			return RV_CONTINUE_ASYNC;
		}*/

		// CefClient methods:
		/*virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {
		return this;
		}*/
		/*virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
			return this;
		}*/
		/*virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {
		return this;
		}*/

		// CefDisplayHandler methods:
		//virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
		//                           const CefString& title) OVERRIDE;

		// CefLifeSpanHandler methods:
		//virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
		//virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
		//virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

		/*virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& target_url,
			const CefString& target_frame_name,
			WindowOpenDisposition target_disposition,
			bool user_gesture,
			const CefPopupFeatures& popupFeatures,
			CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client,
			CefBrowserSettings& settings,
			bool* no_javascript_access) OVERRIDE {

			cef_log(0, 0, 0, "POPUP!");

			return false;
		}*/

		// CefLoadHandler methods:
		/*virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) OVERRIDE;*/

		// Request that all existing browser windows close.
		//void CloseAllBrowsers(bool force_close);

		//bool IsClosing() const { return is_closing_; }

	private:
		// List of existing browser windows. Only accessed on the CEF UI thread.

		// Include the default reference counting implementation.
		IMPLEMENT_REFCOUNTING(GarryClient)
	};

	WebSession::WebSession() {
		client = new GarryClient();
	}
}

/*class GarrySchemeHandlerFactory : public CefSchemeHandlerFactory
{
public:
	CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& scheme_name, CefRefPtr<CefRequest> request) OVERRIDE {

	}
};*/