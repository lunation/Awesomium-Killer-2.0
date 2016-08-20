namespace Awesomium {
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

	// HOW ARE THESE MADE?
	class DllExport ResourceRequest {
	public:
		virtual void Cancel() { debug_log(__FUNCTION__); };
		virtual int origin_process_id() { debug_log(__FUNCTION__);  return 1; };
		virtual int origin_routing_id() { debug_log(__FUNCTION__);  return 1; };
		virtual WebURL url() { debug_log(__FUNCTION__); return WebURL(); };
		virtual WebString method() { debug_log(__FUNCTION__); return WebString(); };
		virtual void set_method(const WebString& method) { debug_log(__FUNCTION__); };
		virtual WebString referrer() const { debug_log(__FUNCTION__); return WebString(); };
		virtual void set_referrer(const WebString& referrer) { debug_log(__FUNCTION__); };
		virtual WebString extra_headers() const { debug_log(__FUNCTION__); return WebString(); };
		virtual void set_extra_headers(const WebString& headers) { debug_log(__FUNCTION__); };
		virtual void AppendExtraHeader(const WebString& name, const WebString& value) { debug_log(__FUNCTION__); };
		virtual unsigned int num_upload_elements() const { debug_log(__FUNCTION__); return 0; };
		virtual const UploadElement* GetUploadElement(unsigned int idx) const { debug_log(__FUNCTION__); return nullptr; }; // <=========================== RETURNING NULLPTR
		virtual void ClearUploadElements() { debug_log(__FUNCTION__); };
		virtual void AppendUploadFilePath(const WebString& path) { debug_log(__FUNCTION__); };
		virtual void AppendUploadBytes(const char* bytes, unsigned int num_bytes) { debug_log(__FUNCTION__); };
		virtual void set_ignore_data_source_handler(bool ignore) { debug_log(__FUNCTION__); };

	//protected:
		virtual ~ResourceRequest() {}
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

	class WebSession;

	struct SimpleResponse {
		void set(unsigned int new_len, unsigned char const * new_data, WebString const &new_mime) {
			len = new_len;
			
			if (data!=nullptr)
				delete[] data;

			if (new_data != nullptr) {
				data = new unsigned char[len];
				std::memcpy(data, new_data, len);
			}
			else {
				data = nullptr;
			}

			if (new_mime.data() != nullptr)
				mime = new_mime.data();
			else
				mime = "";
		}

		~SimpleResponse() {
			if (data!=nullptr)
				delete[] data;
		}

		unsigned int len;
		unsigned char* data = 0;
		CefString mime;
	};

	class WebView;

	class DllExport DataSource
	{
	public:
		virtual ~DataSource() { debug_log(__FUNCTION__); };
		virtual void OnRequest(int id, const ResourceRequest& request, const WebString& path) { debug_log(__FUNCTION__); };
		void SendResponse(int id, unsigned int buf, unsigned char*buff, const WebString& meme) { debug_log(__FUNCTION__"1"); };
		void SendResponse(int id, unsigned int len, unsigned char const * data, WebString const &mime) {

			if (id != data_source_id)
				panic("Our datasources are still fucked god damn it.");
			//debug_log("response");
			//debug_log(std::to_string(len).c_str());
			response->set(len, data, mime);
			ready = true;
		};

		// CALL FROM THE GODDAMNED IO THREAD
		void ReqSync(WebView* view, const CefRequest& req, const std::wstring path, SimpleResponse* res) {

			ResourceRequest awesome_req;
			WebString webstr_path(path.c_str());
			//awesome_req.set_method(WebString(L"GET"));
			//awesome_req.AppendExtraHeader

			//req_view = view;
			ready = false;
			response = res;

			//debug_log("fetch");
			//debug_log(CefString(path).ToString().c_str());
			OnRequest(data_source_id, awesome_req, webstr_path);

			if (!ready) panic("low quality meme");
		}

	protected:
		DataSource() { };
	public:
		void set_session(WebSession*session, int data_source_id) {
			session_ = session;
		};
	protected:
		WebSession*session_;
		int data_source_id = 0; 
		friend class WebSessionImpl;

	protected:
		//static WebView* req_view;

	private:
		// Hopefully there's never more than one IO thread lol.
		// Possibly replace this with an actual mutex
		static bool ready;
		static SimpleResponse* response;
		// we are NOT safe tacking stuff on. make static table of tables (ugh) to store filled reqs.
		//static std::unordered_map<DataSource*, std::unordered_map<int, FilledReq*>> filled_requests;
	};

	//WebView* DataSource::req_view = nullptr;

	bool DataSource::ready = false;
	SimpleResponse* DataSource::response = nullptr;

	//std::unordered_map<DataSource*, std::unordered_map<int, FilledReq*>> DataSource::filled_requests;
}