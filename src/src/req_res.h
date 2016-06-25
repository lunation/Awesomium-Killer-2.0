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

	struct FilledReq {
		FilledReq(unsigned int len, unsigned char const * data, WebString const &mime) {
			this->len = len;
			
			if (data != nullptr) {
				this->data = new unsigned char[len];
				std::memcpy(this->data, data, len);
			}
			else {
				this->data = nullptr;
			}

			if (mime.data() != nullptr)
				this->mime = mime.data();
		}

		~FilledReq() {
			if (data!=nullptr)
				delete[] data;
		}

		unsigned int len;
		unsigned char* data;
		CefString mime;
	};

	// What is this?
	class DllExport DataSource
	{
	public:
		virtual ~DataSource() { debug_log(__FUNCTION__); };
		virtual void OnRequest(int id, const ResourceRequest& request, const WebString& path) { debug_log(__FUNCTION__); };
		void SendResponse(int id, unsigned int buf, unsigned char*buff, const WebString& meme) { debug_log(__FUNCTION__"1"); };
		void SendResponse(int id, unsigned int len, unsigned char const * data, WebString const &mime) {

			filled_requests[this][id] = new FilledReq(len, data, mime);

		};

		int FireReq(const CefRequest& request, const std::wstring path) {

			ResourceRequest awesome_req;

			WebString webstr_path(path.c_str());

			OnRequest(data_source_id, awesome_req, webstr_path);

			return data_source_id++;
		}

		// IT IS THE USER'S RESPONSIBILITY TO DELETE THE REQUEST!
		FilledReq* GetFilledReq(int id) {

			auto table = &filled_requests[this];

			auto iter = table->find(id);

			if (iter == table->end())
				return nullptr;

			auto result = iter->second;
			table->erase(iter);

			return result;
		}

	protected:
		DataSource() { };
		void set_session(WebSession*session, int data_source_id) { debug_log(__FUNCTION__); };
		WebSession*session_;
		int data_source_id = 0; 
		friend class WebSessionImpl;

	private:
		// we are NOT safe tacking stuff on. make static table of tables (ugh) to store filled reqs.
		static std::unordered_map<DataSource*, std::unordered_map<int, FilledReq*>> filled_requests;
	};

	std::unordered_map<DataSource*, std::unordered_map<int, FilledReq*>> DataSource::filled_requests;
}