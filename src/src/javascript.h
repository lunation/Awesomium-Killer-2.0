namespace Awesomium {

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

		~JSValue() { debug_log(__FUNCTION__); }; // this surely just releases the handle on our object!

		JSValue& operator=(const JSValue&rhs) { debug_log(__FUNCTION__); return *this; }; // we PROBABLY just want shallow copies (copy the handle only)

		// Not sure wtf I implemented these, going to tunnel through CEF anyway...
		static const JSValue& Undefined() {
			debug_log(__FUNCTION__);
			static JSValue* undef = nullptr;
			if (undef == nullptr) {
				undef = new JSValue();
			}
			return *undef;
		};
		static const JSValue& Null() {
			debug_log(__FUNCTION__);
			static JSValue* _null = nullptr;
			if (_null == nullptr) {
				_null = new JSValue();
			}
			return *_null;
		};

		bool IsBoolean() const { debug_log(__FUNCTION__); return true; };
		bool IsInteger() const { debug_log(__FUNCTION__); return true; };
		bool IsDouble() const { debug_log(__FUNCTION__); return true; };
		bool IsNumber() const { debug_log(__FUNCTION__); return true; };
		bool IsString() const { debug_log(__FUNCTION__); return true; };
		bool IsArray() const { debug_log(__FUNCTION__); return true; };
		bool IsObject() const { debug_log(__FUNCTION__); return true; };
		bool IsNull() const { debug_log(__FUNCTION__); return false; };
		bool IsUndefined() const { debug_log(__FUNCTION__); return false; };

		WebString ToString() const { debug_log(__FUNCTION__); return WebString(); };
		int ToInteger() const { debug_log(__FUNCTION__); return 1; };
		double ToDouble() const { debug_log(__FUNCTION__); return 1; };
		bool ToBoolean() const { debug_log(__FUNCTION__); return true; };

		JSArray& ToArray();
		const JSArray& ToArray() const;

		JSObject& ToObject();
		const JSObject& ToObject() const;
	};

	template<class T>
	class WebVector;

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

	class WebView;

	class DllExport JSObject
	{
	public:
		JSObject() { debug_log("NEW JSOBJ"); };
		JSObject(const JSObject &obj) { debug_log("COPY JSOBJ"); };
		JSObject &operator=(const JSObject&rhs) { debug_log("assign object"); return *this; };
		unsigned int remote_id() const { debug_log("remote id"); return 0; };
		int ref_count() const { debug_log("ref count"); return 0; };

		JSObjectType type() const { debug_log(__FUNCTION__); return JSObjectType::kJSObjectType_Local; };
		WebView* owner() const { debug_log(__FUNCTION__); return nullptr; };

		JSArray GetPropertyName() const { debug_log(__FUNCTION__); return *(new JSArray()); }; // DUMB!!!
		bool HasProperty(const WebString& name) const { debug_log(__FUNCTION__); return false; }
		JSValue GetProperty(const WebString& name) const { debug_log(__FUNCTION__); return *(new JSValue()); }; // DUMB
		void SetProperty(const WebString& name, const JSValue& value) {
			debug_log(__FUNCTION__);
			debug_log("setprop");
		};
		void SetPropertyAsync(const WebString& name, const JSValue& value) {
			debug_log(__FUNCTION__);
			debug_log("setpropa");
		};
		void RemoveProperty(const WebString& name) { debug_log(__FUNCTION__); };
		JSArray GetMethodNames() const { debug_log(__FUNCTION__); return *(new JSArray()); }; // DUMB
		bool HasMethod(const WebString& name) const { debug_log(__FUNCTION__); return false; };
		JSValue Invoke(const WebString& name, const JSArray& args) { debug_log(__FUNCTION__); return *(new JSValue()); }; // DUMB
		void InvokeAsync(const WebString& name, const JSArray& args) { debug_log(__FUNCTION__); };
		WebString ToString() const { debug_log(__FUNCTION__); return WebString(); };


		void SetCustomMethod(const WebString& name, bool has_return_value) { debug_log(__FUNCTION__); };

		Error last_error() const { debug_log(__FUNCTION__); return Error::kError_None; };
	};

	// IMPL -- SUPER DUMB!

	JSObject& JSValue::ToObject() {
		debug_log(__FUNCTION__);
		return *(new JSObject());
	}

	const JSObject& JSValue::ToObject() const {
		debug_log(__FUNCTION__);
		return *(new JSObject());
	}

	JSArray& JSValue::ToArray() {
		debug_log(__FUNCTION__);
		return *(new JSArray());
	}

	const JSArray& JSValue::ToArray() const {
		debug_log(__FUNCTION__);
		return *(new JSArray());
	};
}