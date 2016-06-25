namespace Awesomium {

	class JSObject;
	class JSArray;

	// Inheritance is for poons.
	class JSValue_Internal {
	public:
		JSValue_Internal() { }
		JSValue_Internal(const JSValue_Internal& other) {

			if (other.type == Object) {
				setObject(*other.data_object);
			}
			else {
				type = other.type;
			}

		}

		~JSValue_Internal() {
			reset();
		}

		void reset() {
			if (type == Object) {
				delete data;
			}
		}

		void setObject(const JSObject& val);
		JSObject* getObject() {
			if (type != Object)
				panic("tried to get object from nonobject");
			return data_object;
		}
		bool isObject() {
			return type == Object;
		}
		
	private:
		union {
			void* data;
			JSObject* data_object;
		};

		JsValueType type = Undefined;
	};

	class DllExport JSValue
	{
	public:

		JSValue() {
			value = new JSValue_Internal();
		};
		explicit JSValue(bool val) { debug_log("NEW JSVALUE2"); };
		explicit JSValue(int val) { debug_log("NEW JSVALUE3"); };
		explicit JSValue(double val) { debug_log("NEW JSVALUE4"); };
		
		JSValue(const WebString& val) { debug_log("COPY JSVALUE"); };
		JSValue(const JSObject& val) {
			value = new JSValue_Internal();
			value->setObject(val);
		};
		JSValue(const JSArray &val) { debug_log("COPY JSVALUE3"); };
		
		JSValue(const JSValue& orig) {
			value = new JSValue_Internal(*orig.value);
		};

		JSValue& operator=(const JSValue&rhs) {
			delete value;
			value = new JSValue_Internal(*rhs.value);
			return *this;
		}; // we PROBABLY just want shallow copies (copy the handle only)

		~JSValue() {
			//debug_log("~kill");
			delete value;
			//debug_log("~~kill");
		};


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
		bool IsObject() const { return value->isObject(); };
		bool IsNull() const { debug_log(__FUNCTION__); return false; };
		bool IsUndefined() const { debug_log(__FUNCTION__); return false; };

		WebString ToString() const { debug_log(__FUNCTION__); return WebString(); };
		int ToInteger() const { debug_log(__FUNCTION__); return 1; };
		double ToDouble() const { debug_log(__FUNCTION__); return 1; };
		bool ToBoolean() const { debug_log(__FUNCTION__); return true; };

		JSArray& ToArray();
		const JSArray& ToArray() const;

		JSObject& ToObject() {
			return *(value->getObject());
		}
		const JSObject& ToObject() const {
			return *(value->getObject());
		}
	private:
		JSValue_Internal* value;
	};

	/*bool js_ipc_complete = false;
	JSValue js_ipc_result;

	JSValue fetch_me_some_js(CefRefPtr<CefBrowser> browser, int id, const CefString& key) {
		auto msg = CefProcessMessage::Create("garry_js_index_obj");
		auto args = msg->GetArgumentList();

		args->SetInt(0, id);
		args->SetString(1, key);

		js_ipc_complete = false;

		browser->SendProcessMessage(PID_RENDERER, msg);

		while (!js_ipc_complete)
			CefDoMessageLoopWork();

		return js_ipc_result;
	}*/

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

	class JSObject_Internal {
	
	public:
		virtual JSObject_Internal* clone() = 0;
		virtual void copyTo(JSObject& target) = 0;

		virtual int getRemoteId() = 0;

		virtual JSValue get(const WebString& name) const = 0;
		virtual void set(const WebString& name, const JSValue& value, bool wait) = 0;
		virtual bool has(const WebString& name) = 0;

		virtual void setCustomMethod(const WebString& name, bool has_return_value) {
			panic("ONLY GLOBALS SUPPORT CUSTOM METHODS");
		}
	};

	class JSObject_Local : public JSObject_Internal {
	
	public:
		JSObject_Internal* clone() OVERRIDE {
			auto copy = new JSObject_Local();
			return copy;
		}

		void copyTo(JSObject& target) OVERRIDE {
			for (auto pair : table) {
				debug_log("COPY ELEMENT");
			}
		}
		
		int getRemoteId() OVERRIDE {
			return 0;
		}

		JSValue get(const WebString& name) const OVERRIDE {
			auto iter = table.find(name.data());

			if (iter != table.end()) {
				return iter->second;
			}

			return JSValue(); // return undefined
		}

		void set(const WebString& name, const JSValue& value, bool wait) OVERRIDE {
			table[name.data()] = value;
		}

		bool has(const WebString& name) OVERRIDE {
			return table.find(name.data()) != table.end();
		}
	private:
		std::unordered_map<std::wstring, JSValue> table;
	};

	class JSObject_Global : public JSObject_Local {
	
	public:
		JSObject_Global(WebView* browser, int n) {
			this->browser = browser;
			index = n;
		}

		JSObject_Internal* clone() OVERRIDE {
			auto copy = new JSObject_Global(browser, index);
			return copy;
		}
		
		int getRemoteId() OVERRIDE {
			return index;
		}

		JSValue get(const WebString& name) const OVERRIDE {
			return JSObject_Local::get(name);
		}

		void set(const WebString& name, const JSValue& value, bool wait) OVERRIDE;

		void setCustomMethod(const WebString& name, bool has_return_value) OVERRIDE;
	private:
		WebView* browser;
		int index;
	};

	class DllExport JSObject
	{
	public:
		JSObject() {
			object = new JSObject_Local();
		};
		JSObject(const JSObject &other) {
			object = other.object->clone();
		};
		JSObject &operator=(const JSObject& other) {
			delete object;
			object = other.object->clone();
			return *this;
		};

		unsigned int remote_id() const {
			debug_log("remote id");
			return object->getRemoteId();
		};

		int ref_count() const { debug_log("ref count"); return 0; };

		JSObjectType type() const { debug_log(__FUNCTION__); return JSObjectType::kJSObjectType_Local; };
		WebView* owner() const { debug_log(__FUNCTION__); return nullptr; };

		JSArray GetPropertyName() const { debug_log(__FUNCTION__); return *(new JSArray()); }; // DUMB!!!
		bool HasProperty(const WebString& name) const {
			return object->has(name);
		}
		JSValue GetProperty(const WebString& name) const {
			return object->get(name);
		 };
		void SetProperty(const WebString& name, const JSValue& value) {
			object->set(name, value, true);
		};
		void SetPropertyAsync(const WebString& name, const JSValue& value) {
			object->set(name, value, false);
		};
		void RemoveProperty(const WebString& name) { debug_log(__FUNCTION__); };
		JSArray GetMethodNames() const { debug_log(__FUNCTION__); return *(new JSArray()); }; // DUMB
		bool HasMethod(const WebString& name) const { debug_log(__FUNCTION__); return false; };
		JSValue Invoke(const WebString& name, const JSArray& args) { debug_log(__FUNCTION__); return *(new JSValue()); }; // DUMB
		void InvokeAsync(const WebString& name, const JSArray& args) { debug_log(__FUNCTION__); };
		WebString ToString() const { debug_log(__FUNCTION__); return WebString(); };

		void SetCustomMethod(const WebString& name, bool has_return_value) {
			object->setCustomMethod(name, has_return_value);
		};

		Error last_error() const { debug_log(__FUNCTION__); return Error::kError_None; };

		void copyTo(JSObject& target) const {
			object->copyTo(target);
		}

		JSObject(WebView* browser, int n) {
			object = new JSObject_Global(browser, n);
		};
	private:
		JSObject_Internal* object;
	};

	// IMPL -- SUPER DUMB!

	void JSValue_Internal::setObject(const JSObject& val) {
		reset();
		type = Object;
		data_object = new JSObject(val);
	}

	////

	// TODO SHITCAN THESE
	//	VVVVVVVVVVVVV

	JSArray& JSValue::ToArray() {
		debug_log(__FUNCTION__);
		return *(new JSArray());
	}

	const JSArray& JSValue::ToArray() const {
		debug_log(__FUNCTION__);
		return *(new JSArray());
	};
}