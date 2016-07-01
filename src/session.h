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
			source->set_session(this, 0);
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

}