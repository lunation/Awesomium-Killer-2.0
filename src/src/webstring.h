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
			debug_log(__FUNCTION__"__LINE__");
			return internal_string->c_str();
		};

		unsigned int length() const {
			debug_log(__FUNCTION__"__LINE__");
			return internal_string->length();
		};

		bool IsEmpty() const {
			debug_log(__FUNCTION__"__LINE__");
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
			debug_log(__FUNCTION__"__LINE__");
			return 1;
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

	template<class T>
	class WebVector {
	public:
		std::vector<T> vector;
	};

	class DllExport WebStringArray {
	public:
		WebStringArray() {
			vector_ = new WebVector<WebString>();
		};
		explicit WebStringArray(unsigned int n) {
			vector_ = new WebVector<WebString>();
			vector_->vector.resize(n);
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

		unsigned int size() const { return vector_->vector.size(); };

		WebString& At(unsigned int idx) {
			return vector_->vector.at(idx);
		};
		const WebString& At(unsigned int idx) const {
			return vector_->vector.at(idx);
		};
		WebString& operator[](unsigned int idx) {
			return vector_->vector[idx];
		};
		const WebString& operator[](unsigned int idx) const {
			return vector_->vector[idx];
		};
		void Push(const WebString& item) {
			vector_->vector.push_back(item);
		};

		friend std::ostream& operator<<(std::ostream& stream, const WebStringArray& arr) {
			for (auto x : arr.vector_->vector)
				stream << " -> " << x << std::endl;
			return stream;
		}

	protected:
		WebVector<WebString>* vector_;  // <- We just need to leave this in as a dummy. Hopefully.
	};
}