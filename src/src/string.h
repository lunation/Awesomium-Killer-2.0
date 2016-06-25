namespace Awesomium {

    // This needs to be 4 bytes (ON WINDOOWS! CHECK OTHER PLATFORMS!)
	// So, we're storing pointers to the internal strings to cut down the size
	class DllExport WebString
	{
	public:

		explicit WebString() {
			internal_string = new CefString;
		};
		explicit WebString(const WebString& src, unsigned int pos, unsigned int n) {
			debug_log(__FUNCTION__"__LINE__");
		};
		explicit WebString(const wchar_t* data) {
			internal_string = new CefString(data);
		};
		explicit WebString(const wchar_t* data, unsigned int len) {
			internal_string = new CefString(data, len, true);
		};
		WebString(const WebString& src) {
			internal_string = new CefString(*src.internal_string);
		};
		
		~WebString() {
			delete internal_string;
		};
		
		WebString& operator=(const WebString& rhs) {
			delete internal_string;
			internal_string = new CefString(*rhs.internal_string);
			return *this;
		};

		static WebString CreateFromUTF8(const char* str, unsigned int len)
		{
			WebString newString;
			newString.internal_string->FromASCII(str); // THIS IS PROBABLY TOTALLY FUCKING WRONG!!!!
			
			return newString;
		};

		const wchar_t* data() const {
			return internal_string->c_str();
		};

		unsigned int length() const {
			debug_log(__FUNCTION__"__LINE__");
			return internal_string->length();
		};

		bool IsEmpty() const {
			return internal_string->empty();
		};

		int Compare(const WebString&src) const
		{
			debug_log(__FUNCTION__"__LINE__");
			return 1;
		};

		WebString& Assign(const WebString& src) {
			debug_log(__FUNCTION__"__LINE__");
			return *this;
		};
		WebString& Assign(const WebString& src, unsigned int pos, unsigned int n) {
			debug_log(__FUNCTION__"__LINE__");
			return *this;
		};
		WebString& Assign(const char16_t* data) {
			debug_log(__FUNCTION__"__LINE__");
			return *this;
		};
		WebString& Assign(const char16_t* data, unsigned int len) {
			debug_log(__FUNCTION__"__LINE__");
			return *this;
		};
		
		
		
		WebString& Append(const WebString& src) {
			debug_log(__FUNCTION__"__LINE__");
			return *this;
		};
		void Swap(WebString& src) {
			debug_log(__FUNCTION__"__LINE__");
			internal_string->swap(*src.internal_string);
		};

		void Clear() {
			debug_log(__FUNCTION__"__LINE__");
			internal_string->clear();
		};

		unsigned int ToUTF8(char*dest, unsigned int len) const {
			std::string byte_str = internal_string->ToString(); // THIS MAY BE WRONG AS WELL!

			int i;
			for (i = 0; i < len && i < byte_str.length(); i++)
				dest[i] = byte_str[i];

			return byte_str.length(); // AFAIK WE NEED TO RETURN THE EXPECTED SIZE OF THE BYTE STRING!
		};

		bool operator==(const WebString&other) const {
			debug_log(__FUNCTION__"__LINE__");
			return true;
		};
		bool operator!=(const WebString&other) const {
			debug_log(__FUNCTION__"__LINE__");
			return true;
		};
		bool operator<(const WebString&other) const {
			debug_log(__FUNCTION__"__LINE__");
			return true;
		};

		friend std::ostream& operator<<(std::ostream& stream, const WebString& str) {
			for (auto x : (std::string)*str.internal_string )
				stream << (char)x;
			return stream;
		}

	private:
		CefString* internal_string;

		friend class InternalHelper;
		explicit WebString(const void*internal_instance) {
			debug_log(__FUNCTION__"__LINE__");
		};
	};

	class DllExport WebStringArray {
	public:
		WebStringArray() {
			vector_ = new std::vector<WebString>();
		};
		explicit WebStringArray(unsigned int n) {
			vector_ = new std::vector<WebString>(n);
		};
		WebStringArray(const WebStringArray& rhs) {
			debug_log(__FUNCTION__"__LINE__");
		};
		~WebStringArray() {
			delete vector_;
		};

		WebStringArray& operator=(const WebStringArray& rhs) {
			debug_log(__FUNCTION__"__LINE__");
			return *this;
		};

		unsigned int size() const { return vector_->size(); };

		WebString& At(unsigned int idx) {
			return vector_->at(idx);
		};
		const WebString& At(unsigned int idx) const {
			return vector_->at(idx);
		};
		WebString& operator[](unsigned int idx) {
			return (*vector_)[idx];
		};
		const WebString& operator[](unsigned int idx) const {
			return (*vector_)[idx];
		};
		void Push(const WebString& item) {
			vector_->push_back(item);
		};

	protected:
		std::vector<WebString>* vector_;
	};

	class DllExport WebURL
	{
	public:
		WebURL() {
			instance_ = new WebString();
		};
		explicit WebURL(const WebString&str) {
			instance_ = new WebString(str);
		};
		WebURL(const WebURL&rhs) {
			instance_ = new WebString(*rhs.instance_);
		};

		~WebURL() { delete instance_; };

		WebURL& operator=(const WebURL&rhs) { debug_log(__FUNCTION__"__LINE__"); return *this; };

		bool IsValid() const { debug_log(__FUNCTION__"__LINE__"); return true; };
		bool IsEmpty() const { debug_log(__FUNCTION__"__LINE__"); return false; };
		WebString spec() const { return *instance_; }; // THIS GETS YOU THE ACTUAL URL!
		WebString scheme() const { debug_log(__FUNCTION__"__LINE__"); return WebString(); };
		WebString username() const { debug_log(__FUNCTION__"__LINE__"); return WebString(); };
		WebString password() const { debug_log(__FUNCTION__"__LINE__"); return WebString(); };
		WebString host() const { debug_log(__FUNCTION__"__LINE__"); return WebString(); };
		WebString port() const { debug_log(__FUNCTION__"__LINE__"); return WebString(); };
		WebString path() const { debug_log(__FUNCTION__"__LINE__"); return WebString(); };
		WebString query() const { debug_log(__FUNCTION__"__LINE__"); return WebString(); };
		WebString anchor() const { debug_log(__FUNCTION__"__LINE__"); return WebString(); };
		WebString filename() const { debug_log(__FUNCTION__"__LINE__"); return WebString(); };

		bool operator==(const WebURL& other) const { debug_log(__FUNCTION__"__LINE__"); return false; };
		bool operator!=(const WebURL& other) const { debug_log(__FUNCTION__"__LINE__"); return false; };
		bool operator<(const WebURL& other) const { debug_log(__FUNCTION__"__LINE__"); return false; };


	private:
		explicit WebURL(const void* internal_instance);
		WebString* instance_;
	};
}